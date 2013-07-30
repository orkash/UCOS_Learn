#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "spi.h"
#include "flash.h"
#include "mmc_sd.h"
#include "ff.h"
#include "integer.h"
#include "diskio.h"
#include "text.h"
#include "24cxx.h"
#include "fontupd.h"
#include "stdio.h"
#include "string.h"
#include "touch.h"
#include "includes.h"
#include "remote.h"
//Mini STM32�����巶������ 
//UCOS ʵ��5:��Ϣ���в���
//����ԭ��@ALIENTEK
//������̳:www.openedv.com	
/*
 UCOSIIʵ��5����Ϣ���е�ʹ�ò���

 1. ʵ��Ŀ�ģ���Ϣ���д��������������
 2. ʵ������KEY0�������£���ʾReceived������Ϣ����˳��Ϊ 4,3,2,1��0
           KEY1�������£���ʾReceived������Ϣ����˳��Ϊ 0,1,2,3,4
           ��Ȼ����ֻ�ǲ��Գ�����İ�����Ҫ��������		   
		  
3.�õ���UCOSII����
�������ǲ��ظ�����֮ǰ������ĺ�����
 
��Ϣ���д���������     
OS_EVENT  *OSQCreate (void **start, INT16U size)
LIFO��ʽ������Ϣ������
 INT8U  OSQPostFront (OS_EVENT *pevent, void *msg)//����ȳ�
FIFO��ʽ������Ϣ������
 INT8U  OSQPost (OS_EVENT *pevent, void *msg)//�Ƚ��ȳ�

OSTaskCreate()��������TaskStart����
��TaskStart����4�ε���ucos���񴴽�����OSTaskCreate()����4������TaskLed��TaskLed1��TaskLCD��TaskKey ��ͬʱ������Ϣ����
Str_Q =  OSQCreate(&MsgGrp[0],N_MESSAGES);��
Ȼ�����OSTaskSuspend()������TaskStart���������Ϊ��4�����񴴽���TaskStart��������������Ѿ���ϣ���������Ȼ��4�������ڿ�ʼִ��ִ�У�

TaskLed�� 	LED0ѭ����˸����ת���Ϊ200ms
TaskLed1��	LED1ѭ����˸,��ת���Ϊ200ms
TaskLCD:  	ÿ��50ms������Ϣ���У�����ʾ�õ�����Ϣ
TaskKey�� 	ÿ��20msɨ�谴��ֵ ��

TaskKey���а���ɨ�裬����������TaskLCD��TaskKey֮����һ���������OSTaskSuspend�ͻָ�OSTaskResume���������������Ŀ����Ϊ���ð���ɨ��֮������Ϣ����Ϣ�������֮��TaskLCD����ſ�ʼ������Ϣ���䣬Ҳ������Ϣ��������˲ſ�ʼ������������鿴����˳��

���ΪKEY0���£���ô��LIFO(����ȳ�)��ʽ����Ϣ���з�����Ϣ0-4������֮�󣬿��Կ���Һ����ʾ�յ�����Ϣ˳��Ϊ4,3,2,1,0��Ϊʲô����Ϊ�Ǻ���ȳ����Ȼ����͵�4�����յ���.
���KEY1���£���ô��FIFO(�Ƚ��ȳ�)��ʽ����Ϣ���з�����Ϣ0-4������֮�󣬿��Կ���Һ����ʾ�յ�����Ϣ˳��Ϊ0,1,2,3,4����Ϊ���Ƚ��ȳ����Ȼ���ȷ��͵�0�����յ�.

*/ 
 
//���������ջ��С
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define KEY_STK_SIZE     64
#define LCD_STK_SIZE     64
#define START_STK_SIZE   512

//�����������ȼ�
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      5
#define KEY_TASK_Prio       2
#define LCD_TASK_Prio       4
#define START_TASK_Prio     10

#define N_MESSAGES  12

//�����ջ
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED1_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
 

//��������
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);
void TaskKey(void *pdata);
void TaskLCD(void *pdata);
 

OS_EVENT *Str_Q;
void * MsgGrp[N_MESSAGES];
OS_Q_DATA q_data[];
u8 err; 

 
 int main(void)
 {
 	 
	SystemInit();
	delay_init(72);	     //��ʱ��ʼ��
	NVIC_Configuration();
	uart_init(9600);
 	LED_Init();
	KEY_Init();
	LCD_Init();
	delay_ms(500);
 	POINT_COLOR=RED;
	OSInit();
	OSTaskCreate( TaskStart,	//task pointer
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
	OSStart();
	return 0;

 }	
  
//��ʼ����
void TaskStart(void * pdata)
{
	pdata = pdata; 

	OS_ENTER_CRITICAL(); 
 	Str_Q = OSQCreate(&MsgGrp[0],N_MESSAGES);	
	OSTaskCreate(TaskLed, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
	OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);
  	OSTaskCreate(TaskKey, (void * )0, (OS_STK *)&TASK_KEY_STK[KEY_STK_SIZE-1], KEY_TASK_Prio);
  	OSTaskCreate(TaskLCD, (void * )0, (OS_STK *)&TASK_LCD_STK[LCD_STK_SIZE-1], LCD_TASK_Prio);
 
 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//����1
//����DS0������.
void TaskLed(void *pdata)
{	 
	while(1)
	{	
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,200);	
	}
}
//����2							
//����DS1������.
void TaskLed1(void *pdata)
{ 
	while(1)
	{ 
	    LED1=!LED1;
		OSTimeDlyHMSM(0,0,0,200);	
	}
}
 void refreshPanel(){
     LCD_Clear(WHITE);
	 POINT_COLOR=RED;
   	 LCD_ShowString(10,10,"UcosII Message Queue Test");
	 LCD_ShowString(10,120,"Send:");
	 LCD_ShowString(110,120,"Received:");
	 POINT_COLOR=BLUE;
	 LCD_ShowString(10,50,"Press KEY0:  LIFO");
	 LCD_ShowString(10,70,"Press KEY1:  FIFO");
	 LCD_ShowNum(20,140,0,3,16);
 	 LCD_ShowNum(20,160,1,3,16);
	 LCD_ShowNum(20,180,2,3,16);
	 LCD_ShowNum(20,200,3,3,16);
	 LCD_ShowNum(20,220,4,3,16);
 }
//����3
//�������  
 void TaskKey(void *pdata)
 {
 	u8  key=0;
   	u8 i0=0, i1=1, i2=2,i3=3,i4=4;
    while(1){
	  key=KEY_Scan();

	  if(key==1||key==2)
    	{
		 refreshPanel(); 
	  
	 	 if(key==1) 
	 	 {	

 	   	 	OSQPostFront(Str_Q,&i0); //��LIFO������ȳ�����ʽ������Ϣ
			OSQPostFront(Str_Q,&i1);
			OSQPostFront(Str_Q,&i2);
			OSQPostFront(Str_Q,&i3);
			OSQPostFront(Str_Q,&i4);
	  	 }
	     else  
	   	 {		
	    	OSQPost(Str_Q,&i0);	 //��FIFO���Ƚ��ȳ�����ʽ������Ϣ
 			OSQPost(Str_Q,&i1);
			OSQPost(Str_Q,&i2);
			OSQPost(Str_Q,&i3);
			OSQPost(Str_Q,&i4);
			
	   	 }	
		 OSTaskResume(LCD_TASK_Prio); //����ָ�
	  }			
	 
	  OSTimeDlyHMSM(0,0,0,20);		 
	}
 }


 void TaskLCD(void *pdata)
 {	 u8 *result,i=0;
	 POINT_COLOR=RED;
   	 LCD_ShowString(10,10,"UcosII Message Queue Test");
	 LCD_ShowString(10,120,"Send:");
	 LCD_ShowString(110,120,"Received:");
	 POINT_COLOR=BLUE;
	 LCD_ShowString(10,50,"Press KEY0:  LIFO");
	 LCD_ShowString(10,70,"Press KEY1:  FIFO");
     while(1)
	 {	
 	 
		 if(i==0)	 
		  {
		   OSTaskSuspend(LCD_TASK_Prio);//������𣬵ȴ���Ϣ������ϻָ�����ʼ������Ϣ	
		  }
 		 result= (u8 *)OSQPend(Str_Q,0,&err) ;//������Ϣ���� 

     	 LCD_ShowNum(120,140+i*20,*result,3,16);
		 i++;if(i==5) i=0; 
  		 OSTimeDlyHMSM(0,0,0,50);	 
	 }
 } 

