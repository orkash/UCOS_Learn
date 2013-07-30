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
//UCOS ʵ��3	:�ź���ʹ�ò���
//����ԭ��@ALIENTEK
//������̳:www.openedv.com	
/*
UCOSIIʵ��3��UCOSII�ź�����ʹ�ò���

1. ʵ��Ŀ�ģ��ź�������������ʹ�ò���
2. ʵ�����󣺰���KEY0����LED0����˸5�Σ�10�η�ת��
          ����KEY1����LED0����תһ��
		  ����KEY2����LED0��ѭ����˸��ͬʱ�ٰ���KEY0,KEY1֮��LED0״̬����ı�
		  
3. �õ���UCOSII����
�������ǲ����ظ�����֮ǰʵ�齲����ĺ���
�ź����������� 		
OS_EVENT  *OSSemCreate (INT16U cnt)//������ʼֵΪcnt���ź���
�ź������ͺ�����    
INT8U  OSSemPost (OS_EVENT *pevent)��//����һ�Σ��ź�����������1
�ź�����������	
void  OSSemPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)//����һ�Σ��ź�����������1
�ź���ɾ��������   
 OS_EVENT  *OSSemDel (OS_EVENT *pevent, INT8U opt, INT8U *err)//ɾ���ź������ź�����غ�������Ч

4. ʵ������
�����ź����Ļ�������֮������ǲ���ϸ���⣬���ǹ����ṩ��UCOSII��ص���������ϸ������Щ���ͬʱ���Ƽ������ܵ��鼮��Ƕ��ʽʵʱ����ϵͳUCOSIIԭ����Ӧ�á���
OSTaskCreate()��������TaskStart����
��TaskStart����5�ε���ucos���񴴽�����OSTaskCreate��������5������TaskLed��TaskLed1��TaskLCD��TaskKey��TaskTouch��ͬʱ
�����ź�����������Sem_Event=OSSemCreate(4)�����ź���Sem_Event��Ȼ�����OSTaskSuspend()������
TaskStart()���������Ϊ��5�����񴴽���TaskStart��������������Ѿ���ϣ���������Ȼ��5��������ѭ��ִ�У�

TaskLed�� 	������õ��ź�����Ч��LED0ÿ��600ms״̬��ת
TaskLed1��	LED1ÿ��600ms״̬��ת
TaskLCD:  	LCD�ϰ벿��һ��������ɫѭ������
TaskKey�� 	ÿ��20msɨ�谴��ֵ ��
TaskTouch�� ÿ��2msɨ�败�����°벿�ֵĴ����㣬����ʾ��LCD�ϡ�Ҳ�������ǵĴ����������

������TaskKey�У�����ѭ��ɨ���ֵ�����KEY0���£���ô�������ź������ͺ�������10���ź������ź�������������10
 	   for(i=0;i<10;i++)
	   OSSemPost(Sem_Event); //���������ź���10��
���ʱ������TaskLed���������ź����ȴ�״̬����ʱ���񽫽������״̬�� �����Կ���LED0��˸5�Ρ�
�������KEY1����ô�������ź������ͺ�������1���ź������ź���������������1�����ʱ����Կ���LED0״̬��ת
      OSSemPost(Sem_Event); //����һ���ź��� 
���KEY2(WK_UP)�������£���ô�������ź���ɾ������ɾ���ź������ź�������Ч�����ʱ��LED0�ָ���˸��
 OSSemDel(Sem_Event,OS_DEL_ALWAYS,&err);
 ���в���OS_DEL_ALWAYS��������ɾ���ź�����

*/
 
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
void TaskLCD(void *pdata);
void TaskKey(void *pdata);
void TaskTouch(void *pdata);

OS_EVENT * Sem_Event;
OS_SEM_DATA result[];
u8 err; 

 void Load_Drow_Dialog(void)
{   
	LCD_Fill(0, 180,240,320,WHITE); 
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_ShowString(216,304,"RST");//��ʾ��������
	LCD_DrawLine(0, 158, 240, 158);
  	POINT_COLOR=RED;//���û�����ɫ 
}
 
 void Load_Adjust_Panel()
 {
  	LCD_Clear(WHITE);//����
	Touch_Adjust();  //��ĻУ׼ 
	Save_Adjdata();	 
	Load_Drow_Dialog();
 }
 int main(void)
 {
 	 
	SystemInit();
	delay_init(72);	     //��ʱ��ʼ��
	NVIC_Configuration();
	uart_init(9600);
 	LED_Init();
	KEY_Init();
	LCD_Init();
	SPI_Flash_Init();
	AT24CXX_Init();		//IIC��ʼ�� 
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
  
//��ʼ����
void TaskStart(void * pdata)
{
	pdata = pdata; 
	OS_ENTER_CRITICAL(); 
	Sem_Event=OSSemCreate(4);  //�����ź���
	OSTaskCreate(TaskLed, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
	OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);
 	OSTaskCreate(TaskLCD, (void * )0, (OS_STK *)&TASK_LCD_STK[LCD_STK_SIZE-1], LCD_TASK_Prio);
  	OSTaskCreate(TaskKey, (void * )0, (OS_STK *)&TASK_KEY_STK[KEY_STK_SIZE-1], KEY_TASK_Prio);
 	OSTaskCreate(TaskTouch, (void * )0, (OS_STK *)&TASK_TOUCH_STK[TOUCH_STK_SIZE-1], TOUCH_TASK_Prio);
 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//����1
//����DS0������.
void TaskLed(void *pdata)
{
	while(1)
	{	OSSemPend(Sem_Event,0,&err); //�����ź��������ź�����Ч��cnt>0),��ô����������У��������ȴ�״̬
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,600);	
	}
}
//����2							
//����DS1������.
void TaskLed1(void *pdata)
{
	while(1)
	{    
		LED1=!LED1;
		OSTimeDlyHMSM(0,0,0,600);	
	}
}

//����3
//�������  
 void TaskKey(void *pdata)
 {
 	u8  key=0,i=0;
   
    while(1){
	  key=KEY_Scan();
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
 //����4
 //Һ����ʾ
 void TaskLCD(void *pdata){
 	u8  colorIndex=0,count=0;
	u16 colorTable[]={BLACK,YELLOW,RED,GREEN};
    LCD_ShowString(10,4,"LCD Display Panel");
	POINT_COLOR=RED;
	LCD_ShowString(10,20,"Sem_Event count: ");	
	 
    while(1)		   							   
	 {
	  POINT_COLOR=BLUE;
	  OSSemQuery(Sem_Event,result);//��ѯ�ź���Sem_Event��״̬
	  count=result->OSCnt;		   //��ȡ�ź���Sem_Event�ļ���ֵ
	  LCD_ShowNum(150,20,count,4,16);//��ʾ�ź���Sem_Event�ļ���ֵ
	  if(colorIndex==4) 
         colorIndex=0; 
	     LCD_Fill(80,40,160,120,colorTable[colorIndex]);
	     colorIndex++;
	     OSTimeDlyHMSM(0,0,0,200);
	  } 		   
 }	

//����5
//��������
 void TaskTouch(void *pdata)
 {  LCD_ShowString(10,160,"Touch Panel");
 	while(1)
	{   POINT_COLOR=RED;	
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1); //�����ж���ӳ��	  
		if(Pen_Point.Key_Sta==Key_Down)//������������
		{
			Pen_Int_Set(0);//�ر��ж�
			Convert_Pos();
			Pen_Point.Key_Sta=Key_Up;
			if(Pen_Point.X0>216&&Pen_Point.Y0>304)Load_Drow_Dialog();//�������
			else if(Pen_Point.Y0>180)
			{
				Draw_Big_Point(Pen_Point.X0,Pen_Point.Y0);//��ͼ	    
				GPIO_SetBits(GPIOC,GPIO_Pin_1);   
			}		 
			Pen_Int_Set(1);//�����ж�	
		}
		 OSTimeDlyHMSM(0,0,0,2);
	};		
 }

