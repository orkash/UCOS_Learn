/* 本例程uCOS-II 版本为2.51,仅创建一个LED闪烁任务于 P1.0 上,每隔 0.5 秒闪动一次,任务而此例程运行于内 RAM > 300 Byte 的51内核单片机即可。*/  

#include "includes.h"


sbit LED0=P2^0;
sbit LED1=P2^1;
//sbit LED2=P2^2;

OS_STK TaskStartStk1[32];
OS_STK TaskStartStk2[32];
OS_STK TaskStartStk3[32];

void Task1(void *ppdata)
{

	ppdata=ppdata;
	for(;;)
    	{
        TI=0;
		SBUF=0xaa;
		while(TI==0);
		   TI=0;
//		printf("How are you?");
		LED0=0; LED1=1;//LED2=1;
        OSTimeDly(OS_TICKS_PER_SEC);
    	}    
}

void Task2(void *ppdata)
{

	ppdata=ppdata;
	for(;;)
    	{
        TI=0;
		SBUF=0xbb;
		while(TI==0);
		   TI=0;
 //       printf("I am WangZhen");
		LED0=1; LED1=0;//LED2=1;
        OSTimeDly(OS_TICKS_PER_SEC);
    	}    
}
 /*
void Task3(void *ppdata)
{

	ppdata=ppdata;
	for(;;)
    	{
        TI=0;
		SBUF=0xCC;
		while(TI==0);
		   TI=0;
		LED0=1; LED1=1;//LED2=0;
        OSTimeDly(OS_TICKS_PER_SEC*3/2);
    	}    
}  
 */
void seri_init(void) //reentrant
{
    TMOD=TMOD|0x20;
	SCON=0x50;
    TH1=0xFD;
    TL1=0xFD;
	PCON|=0x00;
    TR1=1;
}

void main(void)
{
    seri_init();
	OSInit();
	InitHardware();
	OSTaskCreate(Task1,(void*)0,&TaskStartStk1[0],3);
    OSTaskCreate(Task2,(void*)0,&TaskStartStk2[0],2);
//	OSTaskCreate(Task3,(void*)0,&TaskStartStk3[0],1);
//	LED1=1;
	OSStart();

}
