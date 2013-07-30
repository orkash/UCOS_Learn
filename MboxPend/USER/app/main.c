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
//UCOS ʵ��4	:��Ϣ�������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com	
/*
UCOSIIʵ��4:��Ϣ����ʹ�ò���

 1. ʵ��Ŀ�ģ���Ϣ���䴴�������Ͳ���
 2. ʵ������  KEY0�������£�LED0,LED1û�б仯
            	KEY1�������£�LED1״̬��ת
				KEY2(WK_UP)���£�LED0,LED1״̬��ת
		  
�õ���UCOSII����
��Ϣ���䴴��������  
OS_EVENT  *OSMboxCreate (void *msg)
������Ϣ���亯��:   
void  *OSMboxPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
�����䷢����Ϣ����: 
INT8U  OSMboxPost (OS_EVENT *pevent, void *msg)//��ȴ�������и����ȼ�����������Ϣ
�㲥������Ϣ������  
INT8U  OSMboxPostOpt (OS_EVENT *pevent, void *msg, INT8U opt)//��ȴ��������������������Ϣ

OSTaskCreate()��������TaskStart����
��TaskStart����5�ε���ucos���񴴽�����OSTaskCreate()����5������TaskLed��TaskLed1��TaskLCD��TaskKey��TaskTouch��ͬʱ������Ϣ����Str_Box = OSMboxCreate ((void*)0);��
Ȼ�����OSTaskSuspend()������TaskStart()���������Ϊ��5�����񴴽���TaskStart��������������Ѿ���ϣ���������Ȼ��5�������ڿ�ʼִ��ִ�У�

TaskLed�� 	����յ���Ϣ1��3����ôLED0��ת
TaskLed1��	����յ���Ϣ2��3����ôLED1��ת
TaskLCD:  	LCD�ϰ벿��һ��������ɫѭ������
TaskKey�� 	ÿ��20msɨ�谴��ֵ ��
TaskTouch�� ÿ��2msɨ�败�����°벿�ֵĴ����㣬����ʾ��LCD�ϡ�Ҳ�������ǵĴ����������

����ɨ�������У����KEY0�����£���ô������Ϣ����Str_Box������Ϣ1��
 	   i=1;
	   OSMboxPost(Str_Box,&i); //������Ϣ1
 ���KEY1�����£���ô��������Ϣ2��
 	   i=2;
	   OSMboxPost(Str_Box,&i); //������Ϣ2
 ���KEY2(WK_UP)�����£��������еȴ��������������������Ϣ3��
 	   i=3;
	   OSMboxPostOpt(Str_Box,&i,OS_POST_OPT_BROADCAST); //����������㲥��Ϣ3
	   
���ǿ��Կ�����������ǰ���KEY0,��ΪTaskLed1�����ȼ������TaskLed�����Ե����������ڵȴ���ʱ��ֻ��TaskLed1�����յ���Ϣ1����������LED������ת��
�������KEY1,��ôTaskLed1�յ���Ϣ2��״̬��ת��
�������KEY2����ôTaskLed1��TaskLed�����յ���Ϣ�����ʱ������LED״̬���ᷴת��

OSMboxPostOpt()��OSMboxPost()����������ǰ���ǹ㲥��Ϣ�����еȴ����񶼿����յ�������ֻ������ȼ��������յ���

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

OS_EVENT *Str_Box;

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
	Str_Box = OSMboxCreate ((void*)0);	//������Ϣ����
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
{	u8 result=0;
	while(1)
	{	result= *((u8 *)OSMboxPend(Str_Box,0,&err)); //������Ϣ����
		if(result ==1||result ==3){ 
		  LED0=!LED0;
		}
		OSTimeDlyHMSM(0,0,0,200);	
	}
}
//����2							
//����DS1������.
void TaskLed1(void *pdata)
{	u8 result=0;
	while(1)
	{   result= *((u8 *)OSMboxPend(Str_Box,0,&err));//������Ϣ���� 
	   	if(result ==2||result ==3){ 
		   LED1=!LED1;
		}
		OSTimeDlyHMSM(0,0,0,200);	
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
 	   i=1;
	   OSMboxPost(Str_Box,&i); //������Ϣ1
	  }
	  else if(key==2)
	  {
	   i=2;
	   OSMboxPost(Str_Box,&i); //������Ϣ2
	    	
	  }				
	  else if(key==3)
	  {	
	   i=3;
	   OSMboxPostOpt(Str_Box,&i,OS_POST_OPT_BROADCAST); //����������㲥��Ϣ3
 	  }
	  OSTimeDlyHMSM(0,0,0,20);		 
	}
 }
 //����4
 //Һ����ʾ
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

