#include "Dijkstra.h"
#include "io.h"
#include <iostream>
using namespace std;
#define MAX 512

//#define PRINTINFO
 
#ifdef PRINTINFO
#define INFO do{printf("%6d %s\t%s\n", __LINE__, __func__, __FILE__);}while(0)
#else
#define INFO do{;}while(0)
#endif

#if 0
//检验输入边数和顶点数的值是否有效，可以自己推算为啥：
//顶点数和边数的关系是：((Vexnum*(Vexnum - 1)) / 2) < edge
bool check(int Vexnum, int edge) {
    if (Vexnum <= 0 || edge <= 0 || ((Vexnum*(Vexnum - 1)) / 2) < edge)
        return false;
    return true;
}
#endif

int main(int argc, char *argv[]) {
	char *filename = argv[1];
	char *data[MAX];
	int linesum;
	linesum = Read_File(data,filename);
	INFO;
	Get_Topo(data, linesum);
	INFO;
	SwitchTopo_Dij();
	Release_Buff( data, linesum);
    return 0;
}
