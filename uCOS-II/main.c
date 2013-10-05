/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            WIN32 Sample Code
*
* File : APP.C
* By   : Eric Shufro
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE       128

#define  ATASK_PRIO          9
#define  BTASK_PRIO          10
#define  CTASK_PRIO          11

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

OS_STK        AppStartTaskStk[APP_TASK_START_STK_SIZE];
OS_STK        ATaskStk[TASK_STK_SIZE];
OS_STK        BTaskStk[TASK_STK_SIZE];
OS_STK        CTaskStk[TASK_STK_SIZE];

//内存管理相关变量
OS_MEM * CommTxBuffer1;
INT8U CommTxPart1[8][128];

//消息邮箱相关变量
OS_EVENT * Str_box;

//信号量相关变量
OS_EVENT * Str_semp;

//互斥型信号量
OS_EVENT * Str_mutex;

//定时器相关变量
OS_TMR * Str_Tmr;

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);
static  void  ATask(void *p_arg);
static  void  BTask(void *p_arg);
static  void  CTask(void *p_arg);
#if OS_VIEW_MODULE > 0
static  void  AppTerminalRx(INT8U rx_data);
static  void  BppTerminalRx(INT8U rx_data);
#endif

static  void  TmrCallBack(OS_TMR *ptmr, void *p_arg);

/*
*********************************************************************************************************
*                                                _tmain()
*
* Description : This is the standard entry point for C++ WIN32 code.  
* Arguments   : none
*********************************************************************************************************
*/

void main(int argc, char *argv[])
{
	INT8U  err;

#if 0
    BSP_IntDisAll();                       /* For an embedded target, disable all interrupts until we are ready to accept them */
#endif

    OSInit();                              /* Initialize "uC/OS-II, The Real-Time Kernel"                                      */
    
	err = OSTaskCreateExt(AppStartTask,
                    (void *)0,
                    (OS_STK *)&AppStartTaskStk[TASK_STK_SIZE-1],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppStartTaskStk[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)(void *)"Start Task"   , &err);
    OSTaskNameSet(OS_TASK_IDLE_PRIO  , (INT8U *)(void *)"uC/OS-II Idle", &err);

    OSStart();                             /* Start multitasking (i.e. give control to uC/OS-II)                               */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/
void  AppStartTask (void *p_arg)
{
	INT8U  err;
	p_arg = p_arg;

#if 0
	BSP_Init();                                  /* For embedded targets, initialize BSP functions                             */
#endif

#if OS_TASK_STAT_EN > 0
	OSStatInit();                                /* Determine CPU capacity                                                     */
#endif

	Str_Tmr       = OSTmrCreate(0,
		                        10,
								OS_TMR_OPT_PERIODIC,
								TmrCallBack,
								(void *)0,
								(INT8U *)(void *)"Tmr test",
								&err);                      //创建软定时器
	CommTxBuffer1 = OSMemCreate(CommTxPart1,8,128,&err);	//创建内存分区
	Str_box       = OSMboxCreate((void *)0);                //创建消息邮箱
	Str_semp      = OSSemCreate(0);                         //创建信号量
	Str_mutex     = OSMutexCreate(2,&err);                  //创建互斥型信号量
	err = OSTaskCreateExt(ATask,
		(void *)0,
		(OS_STK *)&ATaskStk[TASK_STK_SIZE-1],
		ATASK_PRIO,
		ATASK_PRIO,
		(OS_STK *)&ATaskStk[0],
		TASK_STK_SIZE,
		(void *)0,
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	err = OSTaskCreateExt(BTask,
		(void *)0,
		(OS_STK *)&BTaskStk[TASK_STK_SIZE-1],
		BTASK_PRIO,
		BTASK_PRIO,
		(OS_STK *)&BTaskStk[0],
		TASK_STK_SIZE,
		(void *)0,
		OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(ATASK_PRIO, (INT8U *)(void *)"A Task", &err);
	OSTaskNameSet(BTASK_PRIO, (INT8U *)(void *)"B Task", &err);
	
	OSTmrStart(Str_Tmr,&err);                    //启动软定时器

	while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{		
		OSTimeDlyHMSM(0,0,0,100);
//		OSTaskSuspend(APP_TASK_START_PRIO);
//		OSTimeDlyHMSM(0,0,3,0);
	}
}

void  TmrCallBack(OS_TMR *ptmr, void *p_arg)
{
	OS_Printf("定时任务\n");  //提示信息
}



void  ATask (void *p_arg)
{
	INT8U  err;
	INT8U i;
	INT8U * BlkPtr;;
	p_arg = p_arg;

	OSTaskDel(APP_TASK_START_PRIO);
	
	while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{
		BlkPtr = OSMemGet(CommTxBuffer1,&err);
		if(err  == OS_ERR_NONE){
			OS_MemClr(BlkPtr,20);
			OS_Printf("任务1--申请内存成功，请输入：");  //提示信息
		}
		else {
			OS_Printf("任务1--申请内存有一个错误");
			OS_STOP();
		}
		for(i = 0 ;i < 128; i++){
			BlkPtr[i] = getchar();
			if(BlkPtr[i] == 0x0a){
				BlkPtr[i] = 0x00;
				break;
			}
			else if(BlkPtr[i] == 'Q'){
				OS_STOP();
			}
		}
		OS_Printf("任务1--发送消息邮箱，并等待传输完成。\n");  //提示信息
		OSMboxPost(Str_box,BlkPtr);                            //发送消息邮箱内容是接收到的键盘输入
		OSSemPend(Str_semp,0,&err);                            //等待打印完成信号
		OS_Printf("任务1--接收到传输完成信号，释放内存\n\n");  //提示信息
		err = OSMemPut(CommTxBuffer1,BlkPtr);                  //释放内存
//        OSTimeDlyHMSM(0, 0, 0, 200);       
    }
}
void  BTask (void *p_arg)
{
	INT8U * BlkPtr;
	INT8U err;
    p_arg = p_arg;

	while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{       		
//		OS_Printf("2-");  /* your code here. Create more tasks, etc.                                    */
//        OSTimeDlyHMSM(0, 0, 0,500);
		BlkPtr = OSMboxPend(Str_box,0,&err);
		OS_Printf("任务2--接收到的消息是：%s\n",BlkPtr);
		OS_Printf("任务2--发送打印完成信号！\n");
		OSSemPost(Str_semp);
//		OSTaskDel(OS_PRIO_SELF);
    }
}
