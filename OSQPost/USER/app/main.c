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
//Mini STM32开发板范例代码 
//UCOS 实验5:消息队列测试
//正点原子@ALIENTEK
//技术论坛:www.openedv.com	
/*
 UCOSII实验5：消息队列的使用测试

 1. 实验目的：消息队列创建发送请求测试
 2. 实验现象：KEY0按键按下，显示Received到的消息数据顺序为 4,3,2,1，0
           KEY1按键按下，显示Received到的消息数据顺序为 0,1,2,3,4
           当然，这只是测试程序，你的按键不要连续按。		   
		  
3.用到的UCOSII函数
这里我们不重复讲解之前讲解过的函数。
 
消息队列创建函数：     
OS_EVENT  *OSQCreate (void **start, INT16U size)
LIFO方式发送消息函数：
 INT8U  OSQPostFront (OS_EVENT *pevent, void *msg)//后进先出
FIFO方式发送消息函数：
 INT8U  OSQPost (OS_EVENT *pevent, void *msg)//先进先出

OSTaskCreate()函数创建TaskStart任务，
在TaskStart任务4次调用ucos任务创建函数OSTaskCreate()创建4个任务：TaskLed，TaskLed1，TaskLCD，TaskKey 。同时创建消息队列
Str_Q =  OSQCreate(&MsgGrp[0],N_MESSAGES);。
然后调用OSTaskSuspend()函数将TaskStart任务挂起，因为在4个任务创建后，TaskStart任务该做的事情已经完毕，挂起任务。然后4个任务在开始执行执行；

TaskLed： 	LED0循环闪烁，反转间隔为200ms
TaskLed1：	LED1循环闪烁,反转间隔为200ms
TaskLCD:  	每隔50ms请求消息队列，并显示得到的消息
TaskKey： 	每隔20ms扫描按键值 。

TaskKey进行按键扫描，这里在任务TaskLCD和TaskKey之间有一个任务挂起OSTaskSuspend和恢复OSTaskResume操作，这个操作的目的是为了让按键扫描之后发送消息到消息队列完成之后，TaskLCD任务才开始请求消息邮箱，也就是消息发送完成了才开始请求，这样方便查看队列顺序。

如果为KEY0按下，那么以LIFO(后进先出)方式向消息队列发送消息0-4，发送之后，可以看到液晶显示收到的消息顺序为4,3,2,1,0，为什么？因为是后进先出嘛，自然最后发送的4最先收到了.
如果KEY1按下，那么以FIFO(先进先出)方式向消息队列发送消息0-4，发送之后，可以看到液晶显示收到的消息顺序为0,1,2,3,4，因为是先进先出嘛，自然最先发送的0最先收到.

*/ 
 
//设置任务堆栈大小
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define KEY_STK_SIZE     64
#define LCD_STK_SIZE     64
#define START_STK_SIZE   512

//设置任务优先级
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      5
#define KEY_TASK_Prio       2
#define LCD_TASK_Prio       4
#define START_TASK_Prio     10

#define N_MESSAGES  12

//任务堆栈
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED1_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
 

//任务申明
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
	delay_init(72);	     //延时初始化
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
  
//开始任务
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
//任务1
//控制DS0的亮灭.
void TaskLed(void *pdata)
{	 
	while(1)
	{	
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,200);	
	}
}
//任务2							
//控制DS1的亮灭.
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
//任务3
//按键检测  
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

 	   	 	OSQPostFront(Str_Q,&i0); //以LIFO（后进先出）方式发送消息
			OSQPostFront(Str_Q,&i1);
			OSQPostFront(Str_Q,&i2);
			OSQPostFront(Str_Q,&i3);
			OSQPostFront(Str_Q,&i4);
	  	 }
	     else  
	   	 {		
	    	OSQPost(Str_Q,&i0);	 //以FIFO（先进先出）方式发送消息
 			OSQPost(Str_Q,&i1);
			OSQPost(Str_Q,&i2);
			OSQPost(Str_Q,&i3);
			OSQPost(Str_Q,&i4);
			
	   	 }	
		 OSTaskResume(LCD_TASK_Prio); //任务恢复
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
		   OSTaskSuspend(LCD_TASK_Prio);//任务挂起，等待消息发送完毕恢复任务开始接受消息	
		  }
 		 result= (u8 *)OSQPend(Str_Q,0,&err) ;//请求消息队列 

     	 LCD_ShowNum(120,140+i*20,*result,3,16);
		 i++;if(i==5) i=0; 
  		 OSTimeDlyHMSM(0,0,0,50);	 
	 }
 } 

