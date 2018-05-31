#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include "io.h"

using std::istringstream;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ofstream;

//#define PRINTINFO
#ifdef PRINTINFO
#define INFO do{printf("%6d %s\n",__LINE__, __func__);}while(0)
#else 
#define INFO do{;}while(0)
#endif
#define INFI 4096

int SwitchSum;
int DSPSum;
int portsum;
vector<Switch> switches;
vector<SEdge> switchedges;//edges
int vexnum;
int edgenum;
int srcvex;

int Read_File(char **buff,const char *filename)
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("faile to open %s!\n",filename);
		exit(0);
	}else printf("open %s successed!\n", filename);
	
	char line[MAX_LEN + 2];
	INFO;
	int cnt = 0;
	while((cnt < MAX_FILE_LINE) && !feof(fp))
	{
		INFO;
		line[0] = 0;
		if(fgets(line, MAX_LEN + 2, fp) == NULL) continue;
		if(line[0] == 0) continue;
		INFO;
		buff[cnt] = (char *) malloc(MAX_LEN + 2);
		INFO;
		strncpy(buff[cnt], line, MAX_LEN + 2 - 1);
		buff[cnt][MAX_LEN + 1] = 0; //set the end of line as '0'
		//printf("%s\n",buff[cnt]);
		cnt++;
	}
	printf("there are %d lines in %s.\n", cnt, filename);
	INFO;
	fclose(fp);
	return cnt;
}

void Get_Topo(char **buff, int linesum)
{
	INFO;
	Switch switchtemp;
	memset(&switchtemp, 0, sizeof(switchtemp));
	int switchidtmp = 0;//switch编号(自定义编号)
	int typetmp = 0; //连接点类型1 dsp; 2 switch
	int portmp = 0; //连接端口号
	int endtmp = 0; //连接点编号(自定义编号)
	int tmpcnt = 0;
	int preid = -1;
	INFO;
	while(tmpcnt < linesum)
	{
		INFO;
		int wtmp = 0;
		memset( &switchtemp, 0, sizeof(switchtemp));
		istringstream line(buff[tmpcnt]);
		if(tmpcnt == 0){
			line >> SwitchSum >> DSPSum >> portsum;
			printf("File info: Switch %d, DSP %d, port %d\n", SwitchSum, DSPSum, portsum);
		}
		else
		{
			line >> switchidtmp >> typetmp >> portmp >> endtmp;
			//printf("%d %d %d %d \n", switchidtmp, typetmp,  portmp, endtmp);

			if(preid == switchidtmp){
				switches.back().adj[portmp].type = typetmp;
				switches.back().adj[portmp].id = endtmp;
				if(typetmp == 1){
					wtmp++;
					switches.back().dspnum += wtmp;
				}
				preid = switchidtmp;
			}else{
				switchtemp.ID = switchidtmp;
				switchtemp.adj[portmp].type = typetmp;
				switchtemp.adj[portmp].id = endtmp;
				if(typetmp == 1){
					wtmp++;
					switchtemp.dspnum += wtmp;
				}
				switches.push_back(switchtemp);
				preid = switchidtmp;
			}
		}
		tmpcnt++;	
	}
#if 0
	for(int i = 0; i < SwitchSum; i++)
	{
		printf("switch %d, dspnum %d\n", switches[i].ID, switches[i].dspnum);
	}
#endif 
	Write_Switch();
}


void Write_Switch()
{
	SEdge edgetmp;
	memset(&edgetmp, 0, sizeof(edgetmp));
	string switchtopo = "switchtopo.txt";
	int edgecnt = 0;
	int sidtmp = 0;
	int didtmp = 0;
	int weightmatrix[MAX_SWITCH_NUM][MAX_SWITCH_NUM];
	for(int i = 0; i < MAX_SWITCH_NUM; i++){
		for(int j = 0; j < MAX_SWITCH_NUM; j++)
		{
			weightmatrix[i][j] = INFI;
		}
	}
	INFO;
	for(int i = 0; i < SwitchSum; i++)
	{
		INFO;
		weightmatrix[i][i] = 0;
		sidtmp = switches[i].ID;
		INFO;
		if(sidtmp != 0){
			for(int j = 0; j < MAX_PORT; j++)
			{
				if(switches[i].adj[j].type == 2)
				{
					didtmp = switches[i].adj[j].id;
					weightmatrix[sidtmp][didtmp] = 1;
					weightmatrix[didtmp][sidtmp] = 1;
				}else continue;
			}

		}
	}

#if 0 //testprint
	for(int k = 0; k < MAX_SWITCH_NUM; k++)
	{
		for(int m = 0; m < MAX_SWITCH_NUM; m++)
		{
			printf("%d ",weightmatrix[k][m]);
		}
		printf("\n");
	}
#endif 

	ofstream out(switchtopo);
	for(int i = 0; i < SwitchSum; i++)
	{
		for(int j = 0; j < SwitchSum; j++)
		{
			int srctmp = i;
			int desttmp = j;
			int weighttmp = weightmatrix[i][j];
			if(weighttmp != INFI && weighttmp != 0){
			edgetmp.src = srctmp;
			edgetmp.dest = desttmp;
			edgetmp.weight = weighttmp;
			switchedges.push_back(edgetmp);
			edgecnt++;
			//printf("%6d %6d %6d\n", srctmp, desttmp, weighttmp);
		}else continue;
			//out << srctmp << ' ' << desttmp << ' ' << weighttmp << endl;
		}
 	//	printf("there are %d edgeds\n",edgecnt);
	}
	out << SwitchSum << ' ' << edgecnt << ' ' << endl;
	for(auto a:switchedges)
	{
		out << a.src << ' ' << a.dest << ' ' << a.weight << endl;
	}
	out << EOF;
}	
void Release_Buff(char **buff, int valid_line_num){
	for(int i = 0; i < valid_line_num; i++){
		free(buff[i]);
	}
}
