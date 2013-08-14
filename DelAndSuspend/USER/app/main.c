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
//UCOS ʵ��2	:����Ĺ��𣬻ָ���ɾ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
/*
UCOSIIʵ��2��UCOSII����Ĺ��𣬻ָ���ɾ������

1. ʵ��Ŀ�ģ�����UCOSII������Ĺ��𣬻ָ���ɾ��
2. ʵ�����󣺿���֮��LED0,LED1��˸��LCD�ϰ벿�ֲ�������ѭ����ɫ���������°벿�־��д������幦�ܡ�
          ����KEY0������LED0ֹͣ��˸��LCDֹͣѭ����ɫ
		  ����KEY1������LED0�ָ���˸��LCD�ָ�ѭ����ɫ
		  ����KEY2(WK_UP)������LED0ֹͣ��˸��LCDֹͣѭ����ɫ�����ʱ���ٰ���KEY1�������޷��ָ�����Ϊ�����Ѿ�����˯��״̬���������ȡ�
3. �õ���UCOSII����������
�������֮ǰʵ�齲����ĺ������ǲ��ظ������ˣ�ֻ�������õ��ĺ�����

�����������	INT8U  OSTaskSuspend (INT8U prio)�������ȼ���Ϊprio��������𣬹����������ֹͣ��������У�������һ�ε��ȡ�
����ָ�������		INT8U  OSTaskResume (INT8U prio)�������ȼ�Ϊprio������ָ���
					�ָ���������ù��������������״̬��������һ�ε��ȡ�
��������ɾ��������  INT8U  OSTaskDelReq (INT8U prio)������ɾ�����ȼ���prio������
����ɾ��������      INT8U  OSTaskDel (INT8U prio)��ɾ�����ȼ�Ϊprio������
 					ɾ������֮��������ݵ����ˣ�û���������ˡ�

�������ǿ��Կ������������������ĺ�������ڲ�����ֻ��һ�����ȼ�,��Ϊ���ȼ���UCOSII�������ÿ��������Ψһ�ģ����Կ�������ʶ������

4. ʵ������
OSTaskCreate()��������TaskStart����
��TaskStart����5�ε���ucos���񴴽�����OSTaskCreate��������5������TaskLed��TaskLed1��TaskLCD��TaskKey��TaskTouch��Ȼ�����OSTaskSuspend()������
TaskStart()���������Ϊ��5�����񴴽���TaskStart��������������Ѿ���ϣ���������Ȼ��5��������ѭ��ִ�У�

TaskLed�� 	LED0ÿ��500ms״̬��ת
TaskLed1��	LED1ÿ��200ms״̬��ת
TaskLCD:  	LCD�ϰ벿��һ��������ɫѭ������
TaskKey�� 	ÿ��20msɨ�谴��ֵ
TaskTouch�� ÿ��2msɨ�败�����°벿�ֵĴ����㣬����ʾ��LCD�ϡ�Ҳ�������ǵĴ����������

������TaskKey�У�����ѭ��ɨ���ֵ�����KEY0���£���ô����
		OSTaskSuspend(LED_TASK_Prio);
		OSTaskSuspend(LCD_TASK_Prio);
������TaskLed������TaskLCD�������ʱ�����񽫲���ִ�У�LED0ֹͣ��˸��LCDֹͣѭ����ʾ�������ڵȴ�״ֱ̬�������������е��ý�Һ���OSTaskResume()�������ҡ�

���KEY1�����������ú�����
	    OSTaskResume(LED_TASK_Prio);
		OSTaskResume(LCD_TASK_Prio);
������TaskLed������TaskLCD�ָ������ʱ���������¿�ʼ�������״̬��������һ��������ȡ����Ǳ���Կ���LED0�ָ���˸��LCD�ָ�ѭ����ʾ��

���KEY2�������£���ô�����ú�����
        OSTaskDelReq(LED_TASK_Prio);
		OSTaskDelReq(LCD_TASK_Prio);
��������TaskLed��TaskLCDɾ������ס������ֻ�����������ɾ������ô������TaskLed��TaskLCDִ�е�ʱ�򣬽�ͬʱ������������ж��Ƿ�������ɾ�������������ô��ִ��ɾ��������
	    if(OSTaskDelReq(OS_PRIO_SELF)==OS_TASK_DEL_REQ) 
		 OSTaskDel(OS_PRIO_SELF);
		 
Ҳ����˵��ɾ�������Ƿ�������ִ�У���һ��Ϊ����ɾ�����񣬵ڶ�������ɾ�������������ĺô�����ϵͳ��Ƶ�ʱ�����ֱ��ɾ������������Щ��Դû���ͷŶ�����ϵͳ���в�������

ɾ������֮������TaskLed��TaskLCD������˯��״̬�������ᱻϵͳ���ȡ����ʱ����Կ���LED0������˸��LCDҲ����ѭ����ʾ��

*/	
 
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
void TaskLCD(void *pdata);
void TaskKey(void *pdata);
void TaskTouch(void *pdata);

//���ش������庯��
 void Load_Drow_Dialog(void)
{   
	LCD_Fill(0, 180,240,320,WHITE); 
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_ShowString(216,304,"RST");//��ʾ��������
	LCD_DrawLine(0, 158, 240, 158);
  	POINT_COLOR=RED;//���û�����ɫ 
}
//���봥����У׼���� 
 void Load_Adjust_Panel()
 {
  	LCD_Clear(WHITE);//����
	Touch_Adjust();  //��ĻУ׼ 
	Save_Adjdata();	 
	Load_Drow_Dialog();
 }

 int main(void)
 {
 	 
	SystemInit(); //ϵͳ��ʼ��72Mʱ��
	delay_init(72);	     //��ʱ��ʼ��
	NVIC_Configuration();
//	uart_init(9600); //���ڳ�ʼ��
 	LED_Init();	     //LED�˿ڳ�ʼ��
//	KEY_Init();		  //KEY�����˿ڳ�ʼ��
//	LCD_Init();		 //LCD�˿ڳ�ʼ��
//	SPI_Flash_Init();
//	AT24CXX_Init();		//IIC��ʼ�� 
//	Touch_Init();
	delay_ms(500);
//	Load_Drow_Dialog();
// 	POINT_COLOR=RED;
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
 //   OSTaskCreate(TaskLed3, (void * )0, (OS_STK *)&TASK_LED3_STK[LED2_STK_SIZE-1], LED3_TASK_Prio);
// 	OSTaskCreate(TaskLCD, (void * )0, (OS_STK *)&TASK_LCD_STK[LCD_STK_SIZE-1], LCD_TASK_Prio);
//  	OSTaskCreate(TaskKey, (void * )0, (OS_STK *)&TASK_KEY_STK[KEY_STK_SIZE-1], KEY_TASK_Prio);
// 	OSTaskCreate(TaskTouch, (void * )0, (OS_STK *)&TASK_TOUCH_STK[TOUCH_STK_SIZE-1], TOUCH_TASK_Prio);
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
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,500);	
	}
}
//����2							
//����DS1������.
void TaskLed1(void *pdata)
{
	while(1)
	{    
		LED1=!LED1;
		OSTimeDlyHMSM(0,0,0,400);	
	}
}
//����3							
//����DS2������.
void TaskLed2(void *pdata)
{
	while(1)
	{    
		LED2=!LED2;
		OSTimeDlyHMSM(0,0,0,300);	
	}
}
//����4							
//����DS3������.
void TaskLed3(void *pdata)
{
	while(1)
	{    
		LED3=!LED3;
		OSTimeDlyHMSM(0,0,0,200);	
	}
}














//����3
//�������  
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
 //����4
 //Һ����ʾ
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

//����5
//��������
 void TaskTouch(void *pdata)
 {  LCD_ShowString(10,160,"Touch Panel");
 	while(1)
	{  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1); //�����ж���ӳ��	  
		if(Pen_Point.Key_Sta==Key_Down)//������������
		{
			Pen_Int_Set(0);//�ر��ж�
			Convert_Pos();
			Pen_Point.Key_Sta=Key_Up;
			if(Pen_Point.X0>216&&Pen_Point.Y0>304)Load_Drow_Dialog();//���
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

