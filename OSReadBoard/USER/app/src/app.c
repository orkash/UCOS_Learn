#include "user_cfg.h"
#include "includes.h"


//���������ջ��С
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define LCD_STK_SIZE     64
#define KEY_STK_SIZE     64
#define TOUCH_STK_SIZE   64
#define START_STK_SIZE   512

//�����������ȼ�
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      5
#define LCD_TASK_Prio       4
#define KEY_TASK_Prio       2
#define TOUCH_TASK_Prio     7

#define START_TASK_Prio     10

//�����ջ
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED1_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK  TASK_TOUCH_STK[TOUCH_STK_SIZE];

//��������
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
  
//��ʼ����
void TaskStart(void * pdata)
{
	pdata = pdata; 
	OS_ENTER_CRITICAL(); 
	Sem_Event=OSSemCreate(4);  //�����ź���
	OSTaskCreate(TaskLed, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
	OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);

 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//����1
//����DS0������.
void TaskLed(void *pdata)
{
	while(1)
	{	OSSemPend(Sem_Event,0,&err); //�����ź��������ź�����Ч��cnt>0),��ô����������У��������ȴ�״̬
		//LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,600);	
	}
}
//����2							
//����DS1������.
void TaskLed1(void *pdata)
{
	while(1)
	{    
		//LED1=!LED1;
		OSTimeDlyHMSM(0,0,0,600);	
	}
}

//����3
//�������  
 void TaskKey(void *pdata)
 {
 	u8  key=0,i=0;
   
    while(1){
	  //key=KEY_Scan();
	  if(key==1) 
	  {
 	   for(i=0;i<10;i++)
	   OSSemPost(Sem_Event); //���������ź���10��
	  }
	  else if(key==2)
	  {
	   OSSemPost(Sem_Event); //����һ���ź��� 	
	  }				
	  else if(key==3)
	  {
 	   OSSemDel(Sem_Event,OS_DEL_ALWAYS,&err);//ɾ���ź���Sem_Event֮���ź�������Ч���������񽫲������ź���
	  }
	  OSTimeDlyHMSM(0,0,0,20);		 
	}
 }

 