#include "user_cfg.h"



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

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能PA端口时钟

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_Init(GPIOA, &GPIO_InitStructure);	
 GPIO_SetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2); 
}

void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}

//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(u8 SYSCLK)
{
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	u32 reload;
#endif
 	SysTick->CTRL&=~(1<<2);	//SYSTICK使用外部时钟源	 
//	fac_us=SYSCLK/8;		//不论是否使用ucos,fac_us都需要使用
	    
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	reload=SYSCLK/8;		//每秒钟的计数次数 单位为K	   
	reload*=1000000/OS_TICKS_PER_SEC;//根据OS_TICKS_PER_SEC设定溢出时间
							//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
//	fac_ms=1000/OS_TICKS_PER_SEC;//代表ucos可以延时的最少单位	   
	SysTick->CTRL|=1<<1;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=1<<0;   	//开启SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数   
#endif
}




 int main(void)
 {
 	 
	SystemInit(); //系统初始化72M时钟
    delay_init(72);	     //延时初始化

	NVIC_Configuration();

 	LED_Init();	     //LED端口初始化

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
//		LED0=!LED0;
        GPIO_SetBits(GPIOA,GPIO_Pin_0);
		OSTimeDlyHMSM(0,0,0,500);	
	}
}
//任务2							
//控制DS1的亮灭.
void TaskLed1(void *pdata)
{
	while(1)
	{    
//		LED1=!LED1;
        GPIO_ResetBits(GPIOA,GPIO_Pin_0);
        GPIO_SetBits(GPIOA,GPIO_Pin_1);
		OSTimeDlyHMSM(0,0,0,400);	
	}
}
//任务3							
//控制DS2的亮灭.
void TaskLed2(void *pdata)
{
	while(1)
	{    
//		LED2=!LED2;
        GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		OSTimeDlyHMSM(0,0,0,300);	
	}
}
//任务4							
//控制DS3的亮灭.
void TaskLed3(void *pdata)
{
	while(1)
	{    
//		LED3=!LED3;
		OSTimeDlyHMSM(0,0,0,200);	
	}
}


