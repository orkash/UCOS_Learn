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
//UCOS 实验2	:任务的挂起，恢复和删除
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
/*
UCOSII实验2：UCOSII任务的挂起，恢复和删除测试

1. 实验目的：测试UCOSII的任务的挂起，恢复和删除
2. 实现现象：开机之后，LED0,LED1闪烁，LCD上半部分部分区域循环变色，触摸屏下半部分具有触摸画板功能。
          按下KEY0按键，LED0停止闪烁，LCD停止循环变色
		  按下KEY1按键，LED0恢复闪烁，LCD恢复循环变色
		  按下KEY2(WK_UP)按键，LED0停止闪烁，LCD停止循环变色。这个时候再按下KEY1按键将无法恢复，因为任务已经进入睡眠状态，不被调度。
3. 用到的UCOSII函数简析：
这里对于之前实验讲解过的函数我们不重复讲解了，只讲解新用到的函数。

任务挂起函数：	INT8U  OSTaskSuspend (INT8U prio)；将优先级别为prio的任务挂起，挂起任务就是停止任务的运行，并触发一次调度。
任务恢复函数：		INT8U  OSTaskResume (INT8U prio)；将优先级为prio的任务恢复，
					恢复任务就是让挂起的任务进入就绪状态，并触发一次调度。
任务请求删除函数：  INT8U  OSTaskDelReq (INT8U prio)；请求删除优先级别prio的任务。
任务删除函数：      INT8U  OSTaskDel (INT8U prio)；删除优先级为prio的任务，
 					删除任务之后，任务身份吊销了，没法再运行了。

这里我们可以看到这里关于任务操作的函数的入口参数都只有一个优先级,因为优先级在UCOSII里面对于每个任务是唯一的，所以可以用来识别任务。

4. 实验描述
OSTaskCreate()函数创建TaskStart任务，
在TaskStart任务5次调用ucos任务创建函数OSTaskCreate（）创建5个任务：TaskLed，TaskLed1，TaskLCD，TaskKey，TaskTouch。然后调用OSTaskSuspend()函数将
TaskStart()任务挂起，因为在5个任务创建后，TaskStart任务该做的事情已经完毕，挂起任务。然后5个任务在循环执行；

TaskLed： 	LED0每隔500ms状态反转
TaskLed1：	LED1每隔200ms状态反转
TaskLCD:  	LCD上半部分一定区域颜色循环更换
TaskKey： 	每隔20ms扫描按键值
TaskTouch： 每隔2ms扫描触摸屏下半部分的触摸点，并显示在LCD上。也就是我们的触摸画板程序。

在任务TaskKey中，我们循环扫描键值，如果KEY0按下，那么调用
		OSTaskSuspend(LED_TASK_Prio);
		OSTaskSuspend(LCD_TASK_Prio);
将任务TaskLed和任务TaskLCD挂起，这个时候，任务将不在执行（LED0停止闪烁，LCD停止循环显示），处于等待状态直到在其他任务中调用解挂函数OSTaskResume()将任务解挂。

如果KEY1按键，将调用函数：
	    OSTaskResume(LED_TASK_Prio);
		OSTaskResume(LCD_TASK_Prio);
将任务TaskLed和任务TaskLCD恢复，这个时候，任务将重新开始进入就绪状态，并引发一次任务调度。我们便可以看到LED0恢复闪烁，LCD恢复循环显示。

如果KEY2按键按下，那么将调用函数：
        OSTaskDelReq(LED_TASK_Prio);
		OSTaskDelReq(LCD_TASK_Prio);
请求将任务TaskLed和TaskLCD删除，记住，这里只是请求而不是删除。那么在任务TaskLed和TaskLCD执行的时候，将同时调用这个方法判断是否有任务删除请求，如果有那么将执行删除操作：
	    if(OSTaskDelReq(OS_PRIO_SELF)==OS_TASK_DEL_REQ) 
		 OSTaskDel(OS_PRIO_SELF);
		 
也就是说，删除任务是分两步来执行，第一步为请求删除任务，第二步才是删除任务。这样做的好处是在系统设计的时候避免直接删除导致任务有些资源没有释放而导致系统运行不正常。

删除任务之后，任务TaskLed和TaskLCD将处于睡眠状态，将不会被系统调度。这个时候可以看到LED0不再闪烁，LCD也不会循环显示。

*/	
 
//设置任务堆栈大小
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define LED2_STK_SIZE    64
#define LCD_STK_SIZE     64
#define KEY_STK_SIZE     64
#define TOUCH_STK_SIZE   64
#define START_STK_SIZE   512

//设置任务优先级
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      5
#define LED2_TASK_Prio      4
#define LED3_TASK_Prio      3






#define LCD_TASK_Prio       4
#define KEY_TASK_Prio       2
#define TOUCH_TASK_Prio     7

#define START_TASK_Prio     10

//任务堆栈
OS_STK  TASK_LED_STK[LED_STK_SIZE];
OS_STK  TASK_LED1_STK[LED1_STK_SIZE];
OS_STK  TASK_LED2_STK[LED_STK_SIZE];
OS_STK  TASK_LED3_STK[LED_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK  TASK_TOUCH_STK[TOUCH_STK_SIZE];

//任务申明
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);
void TaskLed2(void *pdata);
void TaskLed3(void *pdata);
void TaskLCD(void *pdata);
void TaskKey(void *pdata);
void TaskTouch(void *pdata);

//加载触摸画板函数
 void Load_Drow_Dialog(void)
{   
	LCD_Fill(0, 180,240,320,WHITE); 
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(216,304,"RST");//显示清屏区域
	LCD_DrawLine(0, 158, 240, 158);
  	POINT_COLOR=RED;//设置画笔蓝色 
}
//进入触摸屏校准函数 
 void Load_Adjust_Panel()
 {
  	LCD_Clear(WHITE);//清屏
	Touch_Adjust();  //屏幕校准 
	Save_Adjdata();	 
	Load_Drow_Dialog();
 }

 int main(void)
 {
 	 
	SystemInit(); //系统初始化72M时钟
	delay_init(72);	     //延时初始化
	NVIC_Configuration();
//	uart_init(9600); //串口初始化
 	LED_Init();	     //LED端口初始化
//	KEY_Init();		  //KEY按键端口初始化
//	LCD_Init();		 //LCD端口初始化
//	SPI_Flash_Init();
//	AT24CXX_Init();		//IIC初始化 
//	Touch_Init();
	delay_ms(500);
//	Load_Drow_Dialog();
// 	POINT_COLOR=RED;
	OSInit();	   //UCOSII初始化
	OSTaskCreate( TaskStart,	//创建开始任务
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
	OSStart();	//UCOSII系统启动
	return 0;

 }	
  
//开始任务
void TaskStart(void * pdata)
{
	pdata = pdata; 
	OS_ENTER_CRITICAL();   
	OSTaskCreate(TaskLed, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
	OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);
    OSTaskCreate(TaskLed2, (void * )0, (OS_STK *)&TASK_LED2_STK[LED2_STK_SIZE-1], LED2_TASK_Prio);
    OSTaskCreate(TaskLed3, (void * )0, (OS_STK *)&TASK_LED3_STK[LED2_STK_SIZE-1], LED3_TASK_Prio);
// 	OSTaskCreate(TaskLCD, (void * )0, (OS_STK *)&TASK_LCD_STK[LCD_STK_SIZE-1], LCD_TASK_Prio);
//  	OSTaskCreate(TaskKey, (void * )0, (OS_STK *)&TASK_KEY_STK[KEY_STK_SIZE-1], KEY_TASK_Prio);
// 	OSTaskCreate(TaskTouch, (void * )0, (OS_STK *)&TASK_TOUCH_STK[TOUCH_STK_SIZE-1], TOUCH_TASK_Prio);
 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//任务1
//控制DS0的亮灭.
void TaskLed(void *pdata)
{
	while(1)
	{  	
//        if(OSTaskDelReq(OS_PRIO_SELF)==OS_TASK_DEL_REQ) //判断是否有删除请求
//		 OSTaskDel(OS_PRIO_SELF);						   //删除任务本身TaskLed
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,500);	
	}
}
//任务2							
//控制DS1的亮灭.
void TaskLed1(void *pdata)
{
	while(1)
	{    
		LED1=!LED1;
		OSTimeDlyHMSM(0,0,0,400);	
	}
}
//任务3							
//控制DS2的亮灭.
void TaskLed2(void *pdata)
{
	while(1)
	{    
		LED2=!LED2;
		OSTimeDlyHMSM(0,0,0,300);	
	}
}
//任务4							
//控制DS3的亮灭.
void TaskLed3(void *pdata)
{
	while(1)
	{    
		LED3=!LED3;
		OSTimeDlyHMSM(0,0,0,200);	
	}
}














//任务3
//按键检测  
 void TaskKey(void *pdata)
 {	u8  key=0;
    while(1){
	  key=KEY_Scan();
	  if(key==1) 
	  {
		OSTaskSuspend(LED_TASK_Prio);
		OSTaskSuspend(LCD_TASK_Prio);
	  }
	  else if(key==2)
	  {
	    OSTaskResume(LED_TASK_Prio);
		OSTaskResume(LCD_TASK_Prio);	
	  }				
	  else if(key==3)
	  {
	    OSTaskDelReq(LED_TASK_Prio);
		OSTaskDelReq(LCD_TASK_Prio);
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
	  if(OSTaskDelReq(OS_PRIO_SELF)==OS_TASK_DEL_REQ) 
	     OSTaskDel(OS_PRIO_SELF);  
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
	{  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1); //触摸中断线映射	  
		if(Pen_Point.Key_Sta==Key_Down)//触摸屏被按下
		{
			Pen_Int_Set(0);//关闭中断
			Convert_Pos();
			Pen_Point.Key_Sta=Key_Up;
			if(Pen_Point.X0>216&&Pen_Point.Y0>304)Load_Drow_Dialog();//清除
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

