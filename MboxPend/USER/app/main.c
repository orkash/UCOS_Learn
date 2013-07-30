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
//UCOS 实验4	:消息邮箱测试
//正点原子@ALIENTEK
//技术论坛:www.openedv.com	
/*
UCOSII实验4:消息邮箱使用测试

 1. 实验目的：消息邮箱创建请求发送测试
 2. 实验现象：  KEY0按键按下，LED0,LED1没有变化
            	KEY1按键按下，LED1状态反转
				KEY2(WK_UP)按下，LED0,LED1状态反转
		  
用到的UCOSII函数
消息邮箱创建函数：  
OS_EVENT  *OSMboxCreate (void *msg)
请求消息邮箱函数:   
void  *OSMboxPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
向邮箱发送消息函数: 
INT8U  OSMboxPost (OS_EVENT *pevent, void *msg)//向等待任务表中高优先级的任务发送消息
广播邮箱消息函数：  
INT8U  OSMboxPostOpt (OS_EVENT *pevent, void *msg, INT8U opt)//向等待任务表中所有任务发送消息

OSTaskCreate()函数创建TaskStart任务，
在TaskStart任务5次调用ucos任务创建函数OSTaskCreate()创建5个任务：TaskLed，TaskLed1，TaskLCD，TaskKey，TaskTouch。同时创建消息邮箱Str_Box = OSMboxCreate ((void*)0);。
然后调用OSTaskSuspend()函数将TaskStart()任务挂起，因为在5个任务创建后，TaskStart任务该做的事情已经完毕，挂起任务。然后5个任务在开始执行执行；

TaskLed： 	如果收到消息1或3，那么LED0反转
TaskLed1：	如果收到消息2或3，那么LED1反转
TaskLCD:  	LCD上半部分一定区域颜色循环更换
TaskKey： 	每隔20ms扫描按键值 。
TaskTouch： 每隔2ms扫描触摸屏下半部分的触摸点，并显示在LCD上。也就是我们的触摸画板程序。

按键扫描任务中，如果KEY0被按下，那么将向消息邮箱Str_Box发送消息1，
 	   i=1;
	   OSMboxPost(Str_Box,&i); //发送消息1
 如果KEY1被按下，那么将发送消息2，
 	   i=2;
	   OSMboxPost(Str_Box,&i); //发送消息2
 如果KEY2(WK_UP)被按下，将向所有等待任务表中所有任务发送消息3，
 	   i=3;
	   OSMboxPostOpt(Str_Box,&i,OS_POST_OPT_BROADCAST); //向所有任务广播消息3
	   
我们可以看到，如果我们按下KEY0,因为TaskLed1的优先级别高于TaskLed，所以当两个任务都在等待的时候，只有TaskLed1可以收到消息1，所以两个LED都不反转。
如果按下KEY1,那么TaskLed1收到消息2，状态反转。
如果按下KEY2，那么TaskLed1和TaskLed都会收到消息，这个时候两个LED状态都会反转。

OSMboxPostOpt()和OSMboxPost()的区别在于前者是广播消息，所有等待任务都可以收到，后者只会高优先级的任务收到。

*/
 
//设置任务堆栈大小
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define LCD_STK_SIZE     64
#define KEY_STK_SIZE     64
#define TOUCH_STK_SIZE   64
#define START_STK_SIZE   512

//设置任务优先级
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      5
#define LCD_TASK_Prio       4
#define KEY_TASK_Prio       2
#define TOUCH_TASK_Prio     7

#define START_TASK_Prio     10

//任务堆栈
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED1_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK  TASK_TOUCH_STK[TOUCH_STK_SIZE];

//任务申明
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);
void TaskLCD(void *pdata);
void TaskKey(void *pdata);
void TaskTouch(void *pdata);

OS_EVENT *Str_Box;

u8 err; 

 void Load_Drow_Dialog(void)
{   
	LCD_Fill(0, 180,240,320,WHITE); 
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(216,304,"RST");//显示清屏区域
	LCD_DrawLine(0, 158, 240, 158);
  	POINT_COLOR=RED;//设置画笔蓝色 
}
 
 void Load_Adjust_Panel()
 {
  	LCD_Clear(WHITE);//清屏
	Touch_Adjust();  //屏幕校准 
	Save_Adjdata();	 
	Load_Drow_Dialog();
 }
 int main(void)
 {
 	 
	SystemInit();
	delay_init(72);	     //延时初始化
	NVIC_Configuration();
	uart_init(9600);
 	LED_Init();
	KEY_Init();
	LCD_Init();
	SPI_Flash_Init();
	AT24CXX_Init();		//IIC初始化 
	Remote_Init();
	Touch_Init();
	delay_ms(500);
	Load_Drow_Dialog();
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
	Str_Box = OSMboxCreate ((void*)0);	//创建消息邮箱
	OSTaskCreate(TaskLed, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
	OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);
 	OSTaskCreate(TaskLCD, (void * )0, (OS_STK *)&TASK_LCD_STK[LCD_STK_SIZE-1], LCD_TASK_Prio);
  	OSTaskCreate(TaskKey, (void * )0, (OS_STK *)&TASK_KEY_STK[KEY_STK_SIZE-1], KEY_TASK_Prio);
 	OSTaskCreate(TaskTouch, (void * )0, (OS_STK *)&TASK_TOUCH_STK[TOUCH_STK_SIZE-1], TOUCH_TASK_Prio);
 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//任务1
//控制DS0的亮灭.
void TaskLed(void *pdata)
{	u8 result=0;
	while(1)
	{	result= *((u8 *)OSMboxPend(Str_Box,0,&err)); //请求消息邮箱
		if(result ==1||result ==3){ 
		  LED0=!LED0;
		}
		OSTimeDlyHMSM(0,0,0,200);	
	}
}
//任务2							
//控制DS1的亮灭.
void TaskLed1(void *pdata)
{	u8 result=0;
	while(1)
	{   result= *((u8 *)OSMboxPend(Str_Box,0,&err));//请求消息邮箱 
	   	if(result ==2||result ==3){ 
		   LED1=!LED1;
		}
		OSTimeDlyHMSM(0,0,0,200);	
	}
}

//任务3
//按键检测  
 void TaskKey(void *pdata)
 {
 	u8  key=0,i=0;
   
    while(1){
	  key=KEY_Scan();
	  if(key==1) 
	  {
 	   i=1;
	   OSMboxPost(Str_Box,&i); //发送消息1
	  }
	  else if(key==2)
	  {
	   i=2;
	   OSMboxPost(Str_Box,&i); //发送消息2
	    	
	  }				
	  else if(key==3)
	  {	
	   i=3;
	   OSMboxPostOpt(Str_Box,&i,OS_POST_OPT_BROADCAST); //向所有任务广播消息3
 	  }
	  OSTimeDlyHMSM(0,0,0,20);		 
	}
 }
 //任务4
 //液晶显示
 void TaskLCD(void *pdata){
 	u8  colorIndex=0;
	u16 colorTable[]={BLACK,YELLOW,RED,GREEN};
    LCD_ShowString(10,4,"LCD Display Panel");

    while(1)		   							   
	 {
 
	  if(colorIndex==4) 
         colorIndex=0; 
	     LCD_Fill(80,40,160,120,colorTable[colorIndex]);
	     colorIndex++;
	     OSTimeDlyHMSM(0,0,0,400);
	  } 		   
 }	

//任务5
//触摸画笔
 void TaskTouch(void *pdata)
 {  LCD_ShowString(10,160,"Touch Panel");
 	while(1)
	{   POINT_COLOR=RED;	
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1); //触摸中断线映射	  
		if(Pen_Point.Key_Sta==Key_Down)//触摸屏被按下
		{
			Pen_Int_Set(0);//关闭中断
			Convert_Pos();
			Pen_Point.Key_Sta=Key_Up;
			if(Pen_Point.X0>216&&Pen_Point.Y0>304)Load_Drow_Dialog();//清除画面
			else if(Pen_Point.Y0>180)
			{
				Draw_Big_Point(Pen_Point.X0,Pen_Point.Y0);//画图	    
				GPIO_SetBits(GPIOC,GPIO_Pin_1);   
			}		 
			Pen_Int_Set(1);//开启中断	
		}
		 OSTimeDlyHMSM(0,0,0,2);
	};		
 }

