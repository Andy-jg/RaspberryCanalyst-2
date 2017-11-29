#ifndef CANFUN_H
#define CANFUN_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "controlcan.h"

#include <sys/select.h>
#include <time.h>

#include <ctime>
#include <ctime>
#include <cstdlib>
#include "unistd.h"
#include "logrec.h"

#define GSM_ID          0x1a3
#define TCU_ID          0x1a1
#define Cali_ID          0x133

#define ERROR_RES       0xffffffff

#define GEAR_RP     1
#define GEAR_PR     2
#define GEAR_RN     3
#define GEAR_ND     4
#define GEAR_DN     5
#define GEAR_NR    6

#define CandevIndex 0
#define Can0        0

#define Cdelay2000ms 200
#define Cidelay 12000 

void *receive_func(void* param);  //接收线程的处理函数
void *Send_func(void* param);
void StartCan0();


#endif
