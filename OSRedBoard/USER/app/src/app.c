#include "user_cfg.h"



//���������ջ��С
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define LED2_STK_SIZE    64
#define LCD_STK_SIZE     64
#define KEY_STK_SIZE     64
#define TOUCH_STK_SIZE   64
#define START_STK_SIZE   512

//�����������ȼ�
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      5
#define LED2_TASK_Prio      4
#define LED3_TASK_Prio      3






#define LCD_TASK_Prio       4
#define KEY_TASK_Prio       2
#define TOUCH_TASK_Prio     7

#define START_TASK_Prio     10

//�����ջ
OS_STK  TASK_LED_STK[LED_STK_SIZE];
OS_STK  TASK_LED1_STK[LED1_STK_SIZE];
OS_STK  TASK_LED2_STK[LED_STK_SIZE];
OS_STK  TASK_LED3_STK[LED_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK  TASK_TOUCH_STK[TOUCH_STK_SIZE];

//��������
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);
void TaskLed2(void *pdata);
void TaskLed3(void *pdata);

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //ʹ��PA�˿�ʱ��

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_Init(GPIOA, &GPIO_InitStructure);	
 GPIO_SetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2); 
}

void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}

//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK)
{
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	u32 reload;
#endif
 	SysTick->CTRL&=~(1<<2);	//SYSTICKʹ���ⲿʱ��Դ	 
//	fac_us=SYSCLK/8;		//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��
	    
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	reload=SYSCLK/8;		//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;//����OS_TICKS_PER_SEC�趨���ʱ��
							//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
//	fac_ms=1000/OS_TICKS_PER_SEC;//����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=1<<1;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=1<<0;   	//����SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}




 int main(void)
 {
 	 
	SystemInit(); //ϵͳ��ʼ��72Mʱ��
    delay_init(72);	     //��ʱ��ʼ��

	NVIC_Configuration();

 	LED_Init();	     //LED�˿ڳ�ʼ��

	OSInit();	   //UCOSII��ʼ��
	OSTaskCreate( TaskStart,	//������ʼ����
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
	OSStart();	//UCOSIIϵͳ����
	return 0;

 }	
  
//��ʼ����
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
//����1
//����DS0������.
void TaskLed(void *pdata)
{
	while(1)
	{  	
//        if(OSTaskDelReq(OS_PRIO_SELF)==OS_TASK_DEL_REQ) //�ж��Ƿ���ɾ������
//		 OSTaskDel(OS_PRIO_SELF);						   //ɾ��������TaskLed
//		LED0=!LED0;
        GPIO_SetBits(GPIOA,GPIO_Pin_0);
		OSTimeDlyHMSM(0,0,0,500);	
	}
}
//����2							
//����DS1������.
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
//����3							
//����DS2������.
void TaskLed2(void *pdata)
{
	while(1)
	{    
//		LED2=!LED2;
        GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		OSTimeDlyHMSM(0,0,0,300);	
	}
}
//����4							
//����DS3������.
void TaskLed3(void *pdata)
{
	while(1)
	{    
//		LED3=!LED3;
		OSTimeDlyHMSM(0,0,0,200);	
	}
}


