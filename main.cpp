#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "controlcan.h"

#include <ctime>
#include <cstdlib>
#include "unistd.h"
#include"logrec.h"
VCI_BOARD_INFO pInfo;

void *receive_func(void* param)  //接收线程的处理函数
{
	int reclen=0;
	VCI_CAN_OBJ rec[100];
	int i;
	
	int *run=(int*)param;
	int ind=((*run)>>4);
	
	while((*run)&0x0f)
	{
//		printf("running....%d\n",ind);
		//char *testlog;
		//char *svar;
		char buf[200];
		char bufindex;
		//char times[100];
		if((reclen=VCI_Receive(VCI_USBCAN2,0,ind,rec,100,100))>0)
		{
			for(bufindex=0;bufindex<reclen;bufindex++)
			{
				printf("IND:%d Receive: %08X", ind, rec[bufindex].ID);
			
				for(i = 0; i < rec[bufindex].DataLen; i++)
				{
					printf(" %02X", rec[bufindex].Data[i]);								
				}
				sprintf(buf,"ID,%04x,rectime,%10d,data,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x",rec[bufindex].ID,rec[bufindex].TimeStamp,rec[bufindex].Data[0],
					rec[bufindex].Data[1],rec[bufindex].Data[2],rec[bufindex].Data[3],rec[bufindex].Data[4],rec[bufindex].Data[5],rec[bufindex].Data[6],rec[bufindex].Data[7]);
				canwrite(buf);
				
				printf("\n");
				canwrite("\n");
			}
			
			
		}
		else if((-1)==reclen)
		{
			canlogclose();
			break;

		}	
		sleep(0.01);// wait 10ms
	}
	printf("run thread exit\n");
	
	pthread_exit(0);
}


main()
{

	printf(">>this is hello !\r\n");

	if(VCI_OpenDevice(VCI_USBCAN2,0,0)==1)
	{
		printf(">>open deivce success!\n");
	}else
	{
		printf(">>open deivce error!\n");
		exit(1);
	}


	if(VCI_ReadBoardInfo(VCI_USBCAN2,0,&pInfo)==1)
	{
                printf(">>Get VCI_ReadBoardInfo success!\n");
		
		//printf(" %08X", pInfo.hw_Version);printf("\n");
		//printf(" %08X", pInfo.fw_Version);printf("\n");
		//printf(" %08X", pInfo.dr_Version);printf("\n");
		//printf(" %08X", pInfo.in_Version);printf("\n");
		//printf(" %08X", pInfo.irq_Num);printf("\n");
		//printf(" %08X", pInfo.can_Num);printf("\n");
		printf(">>Serial_Num:%c", pInfo.str_Serial_Num[0]);
		printf("%c", pInfo.str_Serial_Num[1]);
		printf("%c", pInfo.str_Serial_Num[2]);
		printf("%c", pInfo.str_Serial_Num[3]);
		printf("%c", pInfo.str_Serial_Num[4]);
		printf("%c", pInfo.str_Serial_Num[5]);
		printf("%c", pInfo.str_Serial_Num[6]);
		printf("%c", pInfo.str_Serial_Num[7]);
		printf("%c", pInfo.str_Serial_Num[8]);
		printf("%c", pInfo.str_Serial_Num[9]);
		printf("%c", pInfo.str_Serial_Num[10]);
		printf("%c", pInfo.str_Serial_Num[11]);
		printf("%c", pInfo.str_Serial_Num[12]);
		printf("%c", pInfo.str_Serial_Num[13]);
		printf("%c", pInfo.str_Serial_Num[14]);
		printf("%c", pInfo.str_Serial_Num[15]);
		printf("%c", pInfo.str_Serial_Num[16]);
		printf("%c", pInfo.str_Serial_Num[17]);
		printf("%c", pInfo.str_Serial_Num[18]);
		printf("%c", pInfo.str_Serial_Num[19]);printf("\n");

		printf(">>hw_Type:%c", pInfo.str_hw_Type[0]);
		printf("%c", pInfo.str_hw_Type[1]);
		printf("%c", pInfo.str_hw_Type[2]);
		printf("%c", pInfo.str_hw_Type[3]);
		printf("%c", pInfo.str_hw_Type[4]);
		printf("%c", pInfo.str_hw_Type[5]);
		printf("%c", pInfo.str_hw_Type[6]);
		printf("%c", pInfo.str_hw_Type[7]);
		printf("%c", pInfo.str_hw_Type[8]);
		printf("%c", pInfo.str_hw_Type[9]);printf("\n");
	
	}else
	{
		printf(">>Get VCI_ReadBoardInfo error!\n");
		exit(1);
	}

	VCI_INIT_CONFIG config;
	config.AccCode=0;
	config.AccMask=0xffffffff;
	config.Filter=1;
	config.Mode=0;

	/*125 Kbps  0x03  0x1C*/	
	/*500 Kbps  0x00  0x1C*/
	config.Timing0=0x00;
	config.Timing1=0x1C;
	
	if(VCI_InitCAN(VCI_USBCAN2,0,0,&config)!=1)
	{
		printf("init CAN error\n");
		VCI_CloseDevice(VCI_USBCAN2,0);
//		goto ext;
	}

	if(VCI_StartCAN(VCI_USBCAN2,0,0)!=1)
	{
		printf("Start CAN error\n");
		VCI_CloseDevice(VCI_USBCAN2,0);
//		goto ext;
	}

// 	if(VCI_InitCAN(VCI_USBCAN2,0,1,&config)!=1)
// 	{
// 		printf("init can 1 error\n");
// 		VCI_CloseDevice(VCI_USBCAN2,0);
// //		goto ext;
// 	}
// 	if(VCI_StartCAN(VCI_USBCAN2,0,1)!=1)
// 	{
// 		printf("start can 1 error\n");
// 		VCI_CloseDevice(VCI_USBCAN2,0);
// //		goto ext;
// 	}

	VCI_CAN_OBJ send[3];
	send[0].ID=0;
	send[0].SendType=1;
	send[0].RemoteFlag=0;
	send[0].ExternFlag=1;
	send[0].DataLen=8;
	send[1]=send[0];
	send[2]=send[0];
	send[1].ID=1;
	send[2].ID=2;
	
	int i=0;
	
	for(i = 0; i < send[0].DataLen; i++)
	{
		send[0].Data[i] = i;
		send[1].Data[i] = i + send[0].Data[i];
		send[2].Data[i] = i + send[1].Data[i];
	}
	canlogcreat();
	
	int m_run0=1;
	int m_run1=0x11;

	pthread_t threadid;
	pthread_t threadid1;
	
	int ret;
	ret=pthread_create(&threadid,NULL,receive_func,&m_run0);
	
	//ret=pthread_create(&threadid1,NULL,receive_func,&m_run1);

#if 1
	while (1)
	{

	}
#else
	int times = 30;
	int sendind = 3;
	time_t tm1, tm2;
	time(&tm1);
	while(times--)
	{
		if(VCI_Transmit(VCI_USBCAN2, 0, 0, send, 3) > 0)
		{
			printf("CAN 1 ID: %08X\r\n",send[0].ID);
			printf("CAN 1 data :\r\n");
			for(i=0;i<send[0].DataLen;i++)
			{
				printf(" %08X",send[0].Data[i]);
			}
			printf("\n");
			send[0].ID=sendind++;
			send[1].ID=sendind++;
			send[2].ID=sendind++;
		}
		else
		{
			break;
		}
		usleep(1);
		if(VCI_Transmit(VCI_USBCAN2, 0, 1, send, 3) > 0)
		{
			printf("CAN 2 ID: %08X\r\n", send[0].ID);
			printf("CAN 2 data :\r\n");			
			for(i = 0; i < send[0].DataLen; i++)
			{
				printf(" %08X", send[0].Data[i]);
			}
			printf("\n");
			send[0].ID = sendind++;
			send[1].ID = sendind++;
			send[2].ID = sendind++;
		}
		else
			break;
		usleep(1);
	}

	time(&tm2);
	
	usleep(100);
	
	printf("minute:%d   second:%d\n",(tm2-tm1)/60,(tm2-tm1)%60);

	m_run0=0;
	m_run1=0x10;
	pthread_join(threadid,NULL);
	pthread_join(threadid1,NULL);
#endif

//ext:	
//	VCI_CloseDevice(VCI_USBCAN2,0);
}
