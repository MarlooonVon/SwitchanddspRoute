#ifndef __IO_H_
#define __IO_H_

#include <vector>
using std::vector;

#define MAX_FILE_LINE 1024
#define MAX_LEN 64

#define MAX_ROUTE_NUM 1024
#define MAX_SWITCH_NUM 16
#define MAX_PORT 8


typedef struct{
	int src;
	int dest;
	int weight;
}SEdge;

typedef struct{
	int type;
	int id;
}Adjnode;

typedef struct{
	int rtbnum;
	int rioid[MAX_ROUTE_NUM];
	int port[MAX_ROUTE_NUM];
}RouteTable;

typedef struct{
	int ID;//编号
	int dspnum;
	Adjnode adj[MAX_PORT];
	RouteTable routetable;
}Switch;

extern vector<Switch> switches;

int Read_File(char**, const char *);
void Get_Topo(char **, int);
void Release_Buff(char **, int);
void Write_Switch();
#endif




 
