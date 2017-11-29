#include<stdio.h>
#include<stdlib.h>
#include "logrec.h"	

CANGETSTU stucanget;
time_t t;
struct tm * lt;

//printf ( "%d/%d/%d %d:%d:%d\n",lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);//输出结果
void canwrite(const char dd[])
{	if(1==stucanget.openflag)
	{
		fprintf(stucanget.logfile,"%s",dd);
	}
}
void canlogcreat()
{	
	char filenamechar[filenamelen];
	time (&t);//获取Unix时间戳。
	lt = localtime (&t);//转为时间结构。
	sprintf(filenamechar,"canlog%04d%2d%2d%2d%2d.csv",lt->tm_year+1900,lt->tm_mon,lt->tm_mday,lt->tm_hour, lt->tm_min);
	if((stucanget.logfile=fopen(filenamechar,"a"))==NULL)
	{
		printf("Can`t open file\n");
		exit(0);
	}	
	else 
	{
		stucanget.openflag=1;
	}
}
void canlogclose()
{	
	if(1==stucanget.openflag)
	{
		fclose(stucanget.logfile);	
		stucanget.openflag=0;
	}
	
}

