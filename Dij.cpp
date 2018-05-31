#include <iostream>  
#include <fstream>  
#include <vector>  
#include <list> 
#include <stdlib.h>
#include <limits.h>
#include "Dijkstra.h"
#include "io.h"

using namespace std;  

#define ADDITION 0.05
#define MAX_SWITCH_VEX 16
//#define PRINTINFO

#ifdef PRINTINFO
#define INFO do{printf("%6d %s\t%s\n", __LINE__, __func__, __FILE__);}while(0)
#else
#define INFO do{;}while(0)
#endif

extern vector<Switch> switches;

void Traverse(int);
void Print(int, int);
float maxcost = 0.0;
vector<int> pathtmp;

int stmp, dtmp;

//邻接表中节点，每个节点与该节点对应的索引号指定一条边  
struct Node  
{  
    int u; //边终点节点号  
    float w; //边权值  
    Node(int a, float b) :u(a), w(b){}  
};  
struct Record  
{  
    int pre; //路径中当前节点的前一节点  
    float cost; //当前节点到源节点的最短路径代价  
};  
int n, m; //n表示图中节点个数，m表示图中边数，s表示源节点 
vector<vector<Node> >Adj;
//vector<list<Node>> Adj; //图的邻接表  
vector<Record> Path; //采用双亲表示法存储源节点到其他所有节点的最短路径信息 

void Dijkstra(int s, int d)  
{  
	//cout << Adj[s][1].u <<"+++++" << endl;
    vector<bool> isUsed(n, false); //向量某索引号对应的值为true，表示该索引号对应的节点  
    //在S集合中  
    list<int> Assi; //Assi中存储着当前的候选节点  
    Path.assign(n, Record());  
    //路径信息初始化  
    for (int i = 0; i < n; i++)  
    {  
        Path[i].pre = i;  
        Path[i].cost = INT_MAX;  
    }
    isUsed[s] = true;  

    for (auto it = Adj[s].begin(); it != Adj[s].end(); it++)  
    { 
        Path[it->u].pre = s;  
        Path[it->u].cost = it->w;  
        Assi.push_back(it->u);  
    }
    while (!Assi.empty())  
    {  
        list<int>::iterator It;  
        int minCost = INT_MAX;
	    for (auto it = Assi.begin(); it != Assi.end(); it++)  
        { 
		    if (minCost > Path[*it].cost)  
            {  
                minCost = Path[*it].cost;  
                It = it;
				
			}  
        }  
        int u = *It;  
        Assi.erase(It);  
        isUsed[u] = true;  
        //对与选中节点直接相连，并且不在S集合中的节点进行松弛操作  
        //同时更新Assi的内容  
        for (auto it = Adj[u].begin(); it != Adj[u].end(); it++)  
        {  
            if (isUsed[it->u]) continue;  
            if (Path[it->u].cost == INT_MAX) Assi.push_back(it->u);  
            if (Path[it->u].cost > minCost + it->w)  
            {  
                Path[it->u].cost = minCost + it->w;  
                Path[it->u].pre = u;  
            }  
        }
    }
	Print(s, d);
}

int s,d;
void Traverse(int k)  
{
	float w_add = ADDITION;
	INFO;
	int pretmp = Path[k].pre;
    if (pretmp == k) { s = k; cout << s;  return; } 
	INFO;
    Traverse(pretmp);
	INFO;
	d = k;
	//cout << "s = "<< s << " d = " << d << " w " << w_add << endl;
	for(auto it = Adj[s].begin(); it != Adj[s].end(); it++)
	{
		//cout << "before s = " << s << " u " << it->u << " w "<< it->w << endl; 
		if(it->u == d)
		{
			it->w += w_add; 
			//cout << "after s = " << s << " u " << it->u << " w "<< it->w << endl; 
			
		}
	}
	s = d;
    cout << "->" << d;
	pathtmp.push_back(d);
	INFO;
}  


void Print(int s, int d)  
{ 
	float CurrentCost;
	if(s != stmp || d != dtmp)
	{
    	cout << "Result:\n";  
    	cout << "From " << s << " to " << d << ": "<<endl;  
	}
	CurrentCost = Path[d].cost;
    if (CurrentCost == INT_MAX){ cout << "No path\n"; }  
    Traverse(d);  
    cout << endl;
	if(CurrentCost != INT_MAX)
	{
    	//cout << "Minimal Cost: " << CurrentCost << endl << endl;
		if( maxcost < CurrentCost )
			maxcost = CurrentCost;
	}
	stmp = s;
	dtmp = d;
} 

void SwitchTopo_Dij()  
{ 
	int src, dst;
	INFO;
    ifstream in("switchtopo.txt"); //从文件中读取图的信息 
	INFO;
    in >> n >> m ;  
	printf("vexnum: %d, edgenum: %d\n", n, m);
    int u, v;
	float w;
	INFO;
    Adj.assign(n, vector<Node>());
	INFO;
	int edgecnt = 0;
    while (edgecnt < m)  
    {  
        in >> u >> v >> w;
		int utmp = u;
		int vtmp = v;
		INFO;
        Adj[utmp].push_back(Node( vtmp, w)); 
		INFO;
		edgecnt++;
    }
	INFO;
    in.close();
	ofstream out("result.txt");
	int epcho;
	for(int i = 0; i < n; i++)
	{
		for(int j = i; j < n; j++)
		{
			src = i;
			dst = j;
		
		//	printf("switches[%d].dsp %d, switches[%d].dsp %d\n", i, switches[i].dspnum, j, switches[j].dspnum);
			epcho = switches[i].dspnum * switches[j].dspnum;
			if(i == j || epcho == 0) continue;
		//	printf("echo = %d\n", epcho);
			while(epcho > 0)
			{
				Dijkstra(src, dst);	
				out << src;
				for(auto a:pathtmp)
				{
					out << "->" << a; 
				}
				out << endl;
#if 0
				printf("==================\n");
				for(auto a:pathtmp){
					cout << "a " << a ;
					a++;
				}
				printf("==================\n");
#endif 
				pathtmp.clear();
				epcho--;
			}
		}
		
	}
	out << "MaxCost = " << maxcost << endl;
	printf("MaxCost = %d\n", int(maxcost));

}  
