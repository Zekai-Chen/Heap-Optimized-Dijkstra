#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

int n;
int m;
long long K;

class Vertex
{
public:
	vector<int> nexts;
	vector<long long> weights;
};

Vertex a[1000005];    //According to Constraints, n <= 10^5 for all ten cases
					  //Then 10^6 must enough
					  //The first n index reserve for u
					  //The next resevrve for index in Edge
class Edge
{
public:
	int u;
	int v;
	long long w;
	int index;        //index as an intermediary between u and v
	Edge(int u, int v, long long w, int index)
	{
		this->u = u;
		this->v = v;
		this->w = w;
		this->index = index;
	}
};

vector<Edge> edges;

vector<Edge> goin[1000005];    //The goin list
vector<Edge> goout[1000005];   //The goout list

long long dp[1000005];         //The distance used in Dijkstra algorithm in main()

void add(int u, int v, long long w)
{
	edges.push_back(Edge(u, v, w, edges.size()));  //Adds a new Edge at the end of the vector edges

	int index = edges.size() + n;                  //The number of elements in the vector + n nodes(representing different places)
												   
	for (unsigned int i = 0; i < goin[u].size(); i++)
	{
		long long ww = goin[u][i].w;
		int ii = goin[u][i].index;
		//Specially, if li = K · li−1 
		if (ww * K == w) 
		{
			a[ii].nexts.push_back(index);          //Update the next index
			a[ii].weights.push_back(ww * (K - 1)); 
			//Then passing this edge will only cost (K − 1) · li−1 units of energy.
		}
	}

	for (unsigned int i = 0; i < goout[v].size(); i++) //Similar as above
	{
		long long ww = goout[v][i].w;
		int ii = goout[v][i].index;
		if (w * K == ww)
		{
			a[index].nexts.push_back(ii);
			a[index].weights.push_back(w * (K - 1));
		}
	}
	//Key idea is to convert the content based on the K condition

	goin[v].push_back(Edge(u, v, w, index));
	goout[u].push_back(Edge(u, v, w, index));

	a[u].nexts.push_back(index);
	a[u].weights.push_back(w);          //index in between

	a[index].nexts.push_back(v);
	a[index].weights.push_back(w);
}

int main()
{
	int ret = 0;
	ret = scanf("%d%d%lld", &n, &m, &K); //The first line consists of three integer numbers n,m,K.
	if (ret == -1)                       //ret to reduce warning message while compiling
		return 0;						 //n nodes, representing different places.
										 //m edges between these nodes
										 //K is the specific coefficient
	for (int i = 0; i < m; i++) //The following m lines each consists of three integer numbers 
								//u, v, w to describe a bidirectional trail.
	{
		int u;
		int v;
		long long w;
		ret = scanf("%d%d%lld", &u, &v, &w);
		if (ret == -1)
			return 0;
		add(u, v, w);			//bidirectional
		add(v, u, w);			//Storge the edge but change the direction
	}

	//I use Dijkstra algorithm. Also,I use heap to optimize the running time.
	priority_queue <long long, vector<long long>, greater<long long> > q;
	//Make it a min heap with priority_queue
	for (int i = 0; i < 1000005; i++)
		dp[i] = -2;
	dp[1] = 0;

	long long MOD = 1000000;
	q.push(1);

	while (!q.empty())
	{
		long long x = q.top();        //Start with Node 1
		q.pop();
		int index = x % MOD;          //dp[a[index].nexts[i]] * MOD + a[index].nexts[i] below, we need to get a[index].nexts[i]
		//printf("%lld %d\n", value, index);
		for (unsigned int i = 0; i < a[index].nexts.size(); i++) // Traverse the path start from the 1
		{
			if (dp[a[index].nexts[i]] == -2 ||
				dp[a[index].nexts[i]] > dp[index] + a[index].weights[i])
			{
				dp[a[index].nexts[i]] = dp[index] + a[index].weights[i];      //We need the shortest path
				q.push(dp[a[index].nexts[i]] * MOD + a[index].nexts[i]);      //Finally we will reach a point with nothing pushed
				                                                              //Then q.empty() and stop while
			}
		}
	}
	//Output the result
	for (int i = 1; i <= n; i++)
	{
		if (i == 1)
			printf("%lld", dp[i] / 2);    //Adjust the first output
		else
			printf(" %lld", dp[i] / 2);
	}
	printf("\n");
}

