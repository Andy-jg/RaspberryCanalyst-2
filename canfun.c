#include"canfun.h"

VCI_BOARD_INFO pInfo;
VCI_INIT_CONFIG config;

VCI_CAN_OBJ  Gsmbuf;
VCI_CAN_OBJ  Tcubuf;
VCI_CAN_OBJ  Calibuf;


void InitSendDate();
void *receive_func(void* param);  //接收线程的处理函数
void *Send_func(void* param);

void ReadBoardInf(void);
void StartCan0(void );
void CaninitDate();
void Systime20msfun(void);
void Systtime10msfun(void);

UCHAR OpenDev();
UCHAR InitCan();
UCHAR StartCan();


char buf[200];
VCI_CAN_OBJ rec[100];

u8 SystimeDelay10ms;
u16 SelflearDelay;
u8  caiSendflag;
u8 SysCase;
VCI_CAN_OBJ send[3];



void setTimer10ms(int seconds, int mseconds)
{
	struct timeval temp;
	temp.tv_sec = seconds;
	temp.tv_usec = mseconds;

	select(0, NULL, NULL, NULL, &temp);
	Systtime10msfun();
}
void Systtime10msfun(void)
{
	SystimeDelay10ms++;
	SelflearDelay++;
	int reclen=0;
	
	if(SelflearDelay>Cidelay)
	{
		SelflearDelay=0;
		caiSendflag=1;
	}
	if(0==SystimeDelay10ms%2)
	{


	}	

	if(SystimeDelay10ms>=Cdelay2000ms)
	{
		SystimeDelay10ms=0;

		switch(SysCase)
		{
		case GEAR_RP:
		{
			Gsmbuf.Data[0]=0x78;
			SysCase++;

		}
		break;
		case GEAR_PR:
		{
			Gsmbuf.Data[0]=0x87;
			SysCase++;
		}
		break;
		case GEAR_RN:
		{
			Gsmbuf.Data[0]=0x96;
			SysCase++;
		}
		break;
		case GEAR_ND:
		{
			Gsmbuf.Data[0]=0xa5;
			SysCase++;
		}
		break;
		case GEAR_DN:
		{
			Gsmbuf.Data[0]=0x96;
			SysCase++;
		}
		break;
		case GEAR_NR:
		{
			Gsmbuf.Data[0]=0x87;
			SysCase=1;
		}
		break;
		default :
		{
			SysCase++;
			if(SysCase>7)
			{
				SysCase=0;
			}
		}
		break;
		}
	}
	send[0]=Gsmbuf;
	send[1]=Tcubuf;
	send[2]=Calibuf;
	if(1==caiSendflag)
	{
		caiSendflag=0;
		if((reclen==VCI_Transmit(VCI_USBCAN2, CandevIndex, Can0, send, 3)) > 0)
		{
//				printf("CAN 1 ID: %08X\r\n",send[0].ID);
//				printf("CAN 1 data :\r\n");
//				for(uint i=0; i<send[0].DataLen; i++)
//				{
//					printf(" %08X",send[0].Data[i]);
//				}
//				printf("\n");

		}
		else if((-1)==reclen)
		{
			pthread_exit(0);

		}
	}
	else
	{
		if((reclen==VCI_Transmit(VCI_USBCAN2, CandevIndex, Can0, send, 2)) > 0)
		{
			//printf("CAN 1 ID: %08X\r\n",send[0].ID);
			//printf("CAN 1 data :\r\n");
//				for(uint i=0; i<send[0].DataLen; i++)
//				{
//					printf(" %08X",send[0].Data[i]);
//				}
//				printf("\n");

		}
		else if((-1)==reclen)
		{
			pthread_exit(0);

		}
	}

}

void setTimer20ms(int seconds, int mseconds)
{
	struct timeval temp;
	temp.tv_sec = seconds;
	temp.tv_usec = mseconds;
	select(0, NULL, NULL, NULL, &temp);

	Systime20msfun();
}
void Systime20msfun(void)
{
	int reclen=0;
	int i;
	char bufindex;
	if((reclen=VCI_Receive(VCI_USBCAN2,CandevIndex,Can0,rec,100,10))>0)
	{
		if(ERROR_RES==reclen)
		{
			VCI_ERR_INFO RedErr;
			VCI_ReadErrInfo(VCI_USBCAN2,CandevIndex,Can0,&RedErr);
			//printf("ErrorCode:%d ArLost: %d Passive_Err%08X", RedErr.ErrCode, RedErr.ArLost_ErrData,RedErr.Passive_ErrData);

			reclen=0;
		}

		for(bufindex=0; bufindex<reclen; bufindex++)
		{
			//printf("IND:%d", rec[bufindex].ID);

			//for(uint i = 0; i < rec[bufindex].DataLen; i++)
			//{
			//	printf(" %02X", rec[bufindex].Data[i]);
			//}
			sprintf(buf,"ID,%04x,rectime,%10d,data,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",rec[bufindex].ID,rec[bufindex].TimeStamp,rec[bufindex].Data[0],
			        rec[bufindex].Data[1],rec[bufindex].Data[2],rec[bufindex].Data[3],rec[bufindex].Data[4],rec[bufindex].Data[5],rec[bufindex].Data[6],rec[bufindex].Data[7]);
			canwrite(buf);
			//printf("\n");
		}
	}
	else if((-1)==reclen)
	{
		canlogclose();
		pthread_exit(0);
	}
}





void InitSendDate()
{
	Gsmbuf.ID=GSM_ID;
	Gsmbuf.SendType=1;
	Gsmbuf.RemoteFlag=0;
	Gsmbuf.ExternFlag=0;
	Gsmbuf.DataLen=8;

	Tcubuf.ID=TCU_ID;
	Tcubuf.SendType=1;
	Tcubuf.RemoteFlag=0;
	Tcubuf.ExternFlag=0;
	Tcubuf.DataLen=8;

	Calibuf.ID=Cali_ID;
	Calibuf.SendType=1;
	Calibuf.RemoteFlag=0;
	Calibuf.ExternFlag=0;
	Calibuf.DataLen=8;
	Calibuf.Data[0]=14;
	Calibuf.Data[1]=0x55;


}

void *receive_func(void* param)  //接收线程的处理函数
{
	while(TRUE)
	{
		setTimer20ms(0, 37000);
	}
}



void *Send_func(void* param)
{
	while(TRUE)
	{
		setTimer10ms(0,10000);
	}
}


void ReadBoardInf(void)
{
	if(VCI_ReadBoardInfo(VCI_USBCAN2,CandevIndex,&pInfo)==1)
	{
		char buf[200];
		printf(">>Get VCI_ReadBoardInfo success!\n");

		sprintf(buf,"SN%20x",pInfo.str_Serial_Num);
		printf(">>Serial_Num:%c", buf);

		printf("\n");

		printf(">>hw_Type:%c", pInfo.str_hw_Type[0]);
		printf("%c", pInfo.str_hw_Type[1]);
		printf("%c", pInfo.str_hw_Type[2]);
		printf("%c", pInfo.str_hw_Type[3]);
		printf("%c", pInfo.str_hw_Type[4]);
		printf("%c", pInfo.str_hw_Type[5]);
		printf("%c", pInfo.str_hw_Type[6]);
		printf("%c", pInfo.str_hw_Type[7]);
		printf("%c", pInfo.str_hw_Type[8]);
		printf("%c", pInfo.str_hw_Type[9]);
		printf("\n");

	}
	else
	{
		printf(">>Get VCI_ReadBoardInfo error!\n");
		exit(1);
	}


}



void StartCan0(void )
{
	CaninitDate();
	OpenDev();

	InitCan();
	StartCan();
}



void CaninitDate()
{
	config.AccCode=0;
	config.AccMask=0xffffffff;
	config.Filter=1;
	config.Mode=0;
	config.Timing0=0x00;
	config.Timing1=0x1C;
	/*125 Kbps  0x03  0x1C*/
	/*500 Kbps  0x00  0x1C*/
	InitSendDate();
}



UCHAR OpenDev()
{
	UCHAR Retvar;
	Retvar=FALSE;
	if(VCI_OpenDevice(VCI_USBCAN2,CandevIndex,Can0)==1)
	{
		printf(">>open deivce success!\n");
	}
	else
	{
		printf(">>open deivce error!\n");
		Retvar=TRUE;
		exit(1);
	}
	return Retvar;
}
UCHAR InitCan()
{
	UCHAR Retvar;
	Retvar=FALSE;
	if(VCI_InitCAN(VCI_USBCAN2,CandevIndex,Can0,&config)!=1)
	{
		printf("init CAN error\n");
		VCI_CloseDevice(VCI_USBCAN2,CandevIndex);
		Retvar=TRUE;

	}
	return Retvar;
}
UCHAR StartCan()
{
	UCHAR Retvar;
	Retvar=FALSE;
	if(VCI_StartCAN(VCI_USBCAN2,CandevIndex,Can0)!=1)
	{
		printf("Start CAN error\n");
		VCI_CloseDevice(VCI_USBCAN2,CandevIndex);
		Retvar=TRUE;
//		goto ext;
	}
	return Retvar;

}



