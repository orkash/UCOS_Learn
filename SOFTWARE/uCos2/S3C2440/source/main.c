
// *************************************************
// ����ѧARM9��ADS1.2��һ���ܺõ����̣�������̼��׶���
// ������̿����ÿ������ǵ�LED�ƺͷ�����������Ӳ���ĺû���
// ������CACHE�Գ��������ٶȵ�Ӱ�죬��������FCLK��Ƶ�ʡ�
// LED test
//2004.10.25
//***************************************************

#include "config.h"


//=======================================
#include "app_cfg.h"
#include "Printf.h"
//=========================================
OS_STK  MainTaskStk[MainTaskStkLengh];
OS_STK	Task0Stk [Task0StkLengh];       // Define the Task0 stack 
OS_STK	Task1Stk [Task1StkLengh];       // Define the Task1 stack 
OS_STK	Task2Stk [Task2StkLengh];       // Define the Task1 stack 

int rYear, rMonth,rDay,rDayOfWeek,rHour,rMinute,rSecond;


void Rtc_Init(void);

void Task_LCD(void *p);

OS_EVENT *Semp;                         //Creat semp
U8 err;

int Main(int argc, char **argv)
{
	
	//��ʼ��Ŀ���
	TargetInit(); 
	
	//��ʼ��uC/OS   
   	OSInit ();	 
   	
   	//��ʼ��ϵͳʱ��
   	OSTimeSet(0);
   	
   	//����ϵͳ��ʼ����
   	OSTaskCreate (MainTask,(void *)0, &MainTaskStk[MainTaskStkLengh - 1], MainTaskPrio);																										
	
	OSStart ();
	
	return 0;
}

void MainTask(void *pdata) //Main Task create taks0 and task1
{
   
   #if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
   OS_CPU_SR  cpu_sr;
   #endif
   OS_ENTER_CRITICAL();
  	
	Timer0Init();//initial timer0 for ucos time tick
	ISRInit();   //initial interrupt prio or enable or disable
	//GUI_Init();
	OS_EXIT_CRITICAL();
	
	OSPrintfInit();//use task to print massage to Uart 
	
	OSStatInit();
	OSTaskCreate (Task0,(void *)0, &Task0Stk[Task0StkLengh - 1], Task0Prio);	
	OSTaskCreate (Task1,(void *)0, &Task1Stk[Task1StkLengh - 1], Task1Prio);	
	OSTaskCreate (Task2,(void *)0, &Task2Stk[Task2StkLengh - 1], Task2Prio);	 
	 while(1)
	 {
	 
	   //  GUI_DispString("hello word");
	  OSPrintf("\nEnter Main Task\n");
	  OSTimeDly(OS_TICKS_PER_SEC);
	 	 
	 }

}

void Task0	(void *pdata)//Init lwip and create task2
{
 
	char *s="msg";
	char i;
	
	while (1)
	{
    	
    OSPrintf("Enter Task0\n");	
    OSPrintf("CPU Usage: %d%%\n",OSCPUUsage); //cpu usage XX%

     OSTimeDly(OS_TICKS_PER_SEC);

	}
}

void Task1	(void *pdata) //task for test
{
	
	
	U16 TestCnt=0;
	U16 Version;


	Version=OSVersion();	
	
			
	while (1)
	{
	
	 TestCnt++;
     OSPrintf("********************************\n");
     OSPrintf("Enter Task1 Cnt=%d\n",TestCnt);	
     OSPrintf("Enter Task1\n");
     OSPrintf("uC/OS Version:V%4.2f\n",(float)Version*0.01);//ucos version 
     OSPrintf("********************************\n");
     
     //led 
     if(TestCnt%2)
     	rGPBDAT = 0x0000;
     else
    	rGPBDAT = 0x07fe;
    	
     OSTimeDly(OS_TICKS_PER_SEC);

	}
}



#define RGB(r,g,b)   (unsigned int)( (r << 16) + (g << 8) + b )
#define FROM_BCD(n)		((((n) >> 4) * 10) + ((n) & 0xf))
#define TO_BCD(n)		((((n) / 10) << 4) | ((n) % 10))

void Task2(void *pdata)
{
    unsigned int i, x, m, n, k, y;
    int tmp,key;         

    int width = 10;
    int height = 100;
        
    OSPrintf("Task LCD Running...\r\n");


    //===========================
    // RTC��ʼ��
    //===========================
    Rtc_Init();
	
	//LCD ��ʼ��
	Lcd_N35_Init();

    while(1)
    {
    
    	i++;
    	if(i>99)i=0;

		if(rBCDYEAR==0x99)
			rYear = 1999;
		else
			rYear    = (2000 + rBCDYEAR);
			rMonth   = FROM_BCD(rBCDMON & 0x1f);
			rDay		= FROM_BCD(rBCDDAY & 0x03f);
			rDayOfWeek = rBCDDATE-1;
			rHour    = FROM_BCD(rBCDHOUR & 0x3f);
			rMinute     = FROM_BCD(rBCDMIN & 0x7f);
			rSecond     = FROM_BCD(rBCDSEC & 0x7f);


#if defined(LCD_N35) || defined(LCD_T35) || defined(LCD_X35)        
       OSTimeDly( 5 );
       OSPrintf("Task LCD.\n");
       #if 0
       //��LCD�ϴ�ӡʱ��
       Lcd_printf(100,0,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"%02d:%02d:%02d\n", rHour, rMinute, rSecond);
       //��LCD�ϴ�ӡ���ڣ�����
       Lcd_printf(0,15,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"%4d-%02d-%02d ����%d      �й��ƶ�\n",
        	      rYear, rMonth, rDay,rDayOfWeek);
       
       Lcd_printf(0,230,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0," ����֮��uC/OS2������ʾ��:%02d" , i);
       #endif
 	   OSTimeDly(OS_TICKS_PER_SEC/5);
	 //  mydelay(100);
#elif defined(LCD_A70)
       OSTimeDly( 5 );
       OSPrintf("Task LCD.\n");
       //��LCD�ϴ�ӡʱ��
       Lcd_printf(700,0,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"%02d:%02d:%02d\n", rHour, rMinute, rSecond);
       //��LCD�ϴ�ӡ���ڣ�����
       Lcd_printf(500,440,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"%4d-%02d-%02d ����%d      �й��ƶ�\n",
        	      rYear, rMonth, rDay,rDayOfWeek);
       
       Lcd_printf(500,420,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0," ����֮��uC/OS2������ʾ��:%02d" , i);
       
 	   OSTimeDly(OS_TICKS_PER_SEC/5);
	 //  mydelay(100);
#elif defined(LCD_L80)
       OSTimeDly( 5 );
       OSPrintf("Task LCD.\n");
       //��LCD�ϴ�ӡʱ��
       Lcd_printf(700 - 160,20,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"%02d:%02d:%02d\n", rHour, rMinute, rSecond);
       //��LCD�ϴ�ӡ���ڣ�����
       Lcd_printf(500 -160,440,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"%4d-%02d-%02d ����%d      �й��ƶ�\n",
        	      rYear, rMonth, rDay,rDayOfWeek);
       
       Lcd_printf(500 - 160,420,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0," ����֮��uC/OS2������ʾ��:%02d" , i);
       
 	   OSTimeDly(OS_TICKS_PER_SEC/5);
#elif defined(LCD_VGA1024768)
       OSTimeDly( 5 );
       OSPrintf("Task LCD.\n");
       //��LCD�ϴ�ӡʱ��
       Lcd_printf(700,540,RGB( 0x00,0x00,0x00),RGB( 0xFF,0xFF,0xFF),0,"%02d:%02d:%02d\n", rHour, rMinute, rSecond);
       //��LCD�ϴ�ӡ���ڣ�����
       Lcd_printf(700,580,RGB( 0x00,0x00,0x00),RGB( 0xFF,0xFF,0xFF),0,"%4d-%02d-%02d ����%d\n",
        	      rYear, rMonth, rDay,rDayOfWeek);
       
       Lcd_printf(690,620,RGB( 0x00,0x00,0x00),RGB( 0xFF,0xFF,0xFF),0," ����֮��uC/OS2������ʾ��:%02d" , i);
       
 	   OSTimeDly(OS_TICKS_PER_SEC/5);
	 //  mydelay(100);
#endif
	
    }
}

//************************[ Rtc_Init ]*********************************
void Rtc_Init(void)
{
	int wYear, wMonth,wDay,wDayOfWeek,wHour,wMinute,wSecond;

    wYear = 2008;
    wMonth = 9;
    wDay = 5;
    wDayOfWeek = 5;
    wHour= 9;
    wMinute = 41;
    wSecond = 30;
	
	rRTCCON = 1 ;		//RTC read and write enable

	rBCDYEAR = (unsigned char)TO_BCD(wYear%100);	//��
    rBCDMON  = (unsigned char)TO_BCD(wMonth);		//��
    rBCDDAY	 = (unsigned char)TO_BCD(wDay);			//��	
	rBCDDATE = wDayOfWeek+1;						//����
	rBCDHOUR = (unsigned char)TO_BCD(wHour);		//Сʱ
	rBCDMIN  = (unsigned char)TO_BCD(wMinute);		//��
	rBCDSEC  = (unsigned char)TO_BCD(wSecond);		//��
	
	rRTCCON &= ~1 ;		//RTC read and write disable
}

