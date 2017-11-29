#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "controlcan.h"

#include <ctime>

#include <ctime>
#include <cstdlib>
#include "unistd.h"
#include"logrec.h"
#include "canfun.h"

main()
{

	printf(">>this is hello !\r\n");

	int  m_run0;
	int m_run1;
	int ret;
	pthread_t threadid;
	pthread_t threadid1;	
	
	StartCan0();	
	canlogcreat();	
	
	ret=pthread_create(&threadid,NULL,receive_func,&m_run0);
	
	ret=pthread_create(&threadid1,NULL,Send_func,&m_run1);


	while (1)
	{
		sleep(22);// wait 10ms
	}
//ext:	
//	VCI_CloseDevice(VCI_USBCAN2,0);
}
