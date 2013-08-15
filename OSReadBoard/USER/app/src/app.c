#include "user_cfg.h"
#include "includes.h"


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


OS_EVENT * Sem_Event;
OS_SEM_DATA result[];
u8 err; 

 int main(void)
 {
 	 
	SystemInit();

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

 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//任务1
//控制DS0的亮灭.
void TaskLed(void *pdata)
{
	while(1)
	{	OSSemPend(Sem_Event,0,&err); //请求信号量，若信号量有效（cnt>0),那么任务继续运行，否则进入等待状态
		//LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,600);	
	}
}
//任务2							
//控制DS1的亮灭.
void TaskLed1(void *pdata)
{
	while(1)
	{    
		//LED1=!LED1;
		OSTimeDlyHMSM(0,0,0,600);	
	}
}

//任务3
//按键检测  
 void TaskKey(void *pdata)
 {
 	u8  key=0,i=0;
   
    while(1){
	  //key=KEY_Scan();
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

 