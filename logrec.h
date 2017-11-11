#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct 
{
	unsigned char openflag;
	FILE *logfile;
}CANGETSTU;
#define filenamelen 40
void canwrite(const char dd[]);
void canlogcreat();
void canlogclose();

