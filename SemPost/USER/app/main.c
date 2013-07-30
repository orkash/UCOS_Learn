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
//UCOS 实验3	:信号量使用测试
//正点原子@ALIENTEK
//技术论坛:www.openedv.com	
/*
UCOSII实验3：UCOSII信号量的使用测试

1. 实验目的：信号量创建请求发送使用测试
2. 实验现象：按下KEY0按键LED0将闪烁5次（10次反转）
          按下KEY1按键LED0将反转一次
		  按下KEY2按键LED0将循环闪烁，同时再按下KEY0,KEY1之后LED0状态不会改变
		  
3. 用到的UCOSII函数
这里我们不再重复讲解之前实验讲解过的函数
信号量创建函数 		
OS_EVENT  *OSSemCreate (INT16U cnt)//创建初始值为cnt的信号量
信号量发送函数：    
INT8U  OSSemPost (OS_EVENT *pevent)；//调用一次，信号量计数器加1
信号量请求函数：	
void  OSSemPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)//请求一次，信号量计数器减1
信号量删除函数：   
 OS_EVENT  *OSSemDel (OS_EVENT *pevent, INT8U opt, INT8U *err)//删除信号量，信号量相关函数将无效

4. 实验描述
关于信号量的基本概念之类的我们不详细讲解，我们光盘提供的UCOSII相关的资料有详细讲解这些概念，同时，推荐看任哲的书籍《嵌入式实时操作系统UCOSII原理与应用》。
OSTaskCreate()函数创建TaskStart任务，
在TaskStart任务5次调用ucos任务创建函数OSTaskCreate（）创建5个任务：TaskLed，TaskLed1，TaskLCD，TaskKey，TaskTouch，同时
调用信号量创建函数Sem_Event=OSSemCreate(4)创建信号量Sem_Event。然后调用OSTaskSuspend()函数将
TaskStart()任务挂起，因为在5个任务创建后，TaskStart任务该做的事情已经完毕，挂起任务。然后5个任务在循环执行；

TaskLed： 	若请求得到信号量有效，LED0每隔600ms状态反转
TaskLed1：	LED1每隔600ms状态反转
TaskLCD:  	LCD上半部分一定区域颜色循环更换
TaskKey： 	每隔20ms扫描按键值 。
TaskTouch： 每隔2ms扫描触摸屏下半部分的触摸点，并显示在LCD上。也就是我们的触摸画板程序。

在任务TaskKey中，我们循环扫描键值，如果KEY0按下，那么将调用信号量发送函数发送10次信号量，信号量计数器将增10
 	   for(i=0;i<10;i++)
	   OSSemPost(Sem_Event); //连续发送信号量10次
这个时候任务TaskLed正在请求信号量等待状态，此时任务将进入就绪状态， 将可以看到LED0闪烁5次。
如果按下KEY1，那么将调用信号量发送函数发送1次信号量，信号量计数器将增加1，这个时候可以看到LED0状态反转
      OSSemPost(Sem_Event); //发送一次信号量 
如果KEY2(WK_UP)按键按下，那么将调用信号量删除函数删除信号量，信号量将无效。这个时候LED0恢复闪烁。
 OSSemDel(Sem_Event,OS_DEL_ALWAYS,&err);
 其中参数OS_DEL_ALWAYS表明立即删除信号量。

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

OS_EVENT * Sem_Event;
OS_SEM_DATA result[];
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
	Sem_Event=OSSemCreate(4);  //创建信号量
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
{
	while(1)
	{	OSSemPend(Sem_Event,0,&err); //请求信号量，若信号量有效（cnt>0),那么任务继续运行，否则进入等待状态
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,600);	
	}
}
//任务2							
//控制DS1的亮灭.
void TaskLed1(void *pdata)
{
	while(1)
	{    
		LED1=!LED1;
		OSTimeDlyHMSM(0,0,0,600);	
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
 	   for(i=0;i<10;i++)
	   OSSemPost(Sem_Event); //连续发送信号量10次
	  }
	  else if(key==2)
	  {
	   OSSemPost(Sem_Event); //发送一次信号量 	
	  }				
	  else if(key==3)
	  {
 	   OSSemDel(Sem_Event,OS_DEL_ALWAYS,&err);//删除信号量Sem_Event之后，信号量讲无效，所有任务将不受限信号量
	  }
	  OSTimeDlyHMSM(0,0,0,20);		 
	}
 }
 //任务4
 //液晶显示
 void TaskLCD(void *pdata){
 	u8  colorIndex=0,count=0;
	u16 colorTable[]={BLACK,YELLOW,RED,GREEN};
    LCD_ShowString(10,4,"LCD Display Panel");
	POINT_COLOR=RED;
	LCD_ShowString(10,20,"Sem_Event count: ");	
	 
    while(1)		   							   
	 {
	  POINT_COLOR=BLUE;
	  OSSemQuery(Sem_Event,result);//查询信号量Sem_Event的状态
	  count=result->OSCnt;		   //获取信号量Sem_Event的计数值
	  LCD_ShowNum(150,20,count,4,16);//显示信号量Sem_Event的计数值
	  if(colorIndex==4) 
         colorIndex=0; 
	     LCD_Fill(80,40,160,120,colorTable[colorIndex]);
	     colorIndex++;
	     OSTimeDlyHMSM(0,0,0,200);
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

