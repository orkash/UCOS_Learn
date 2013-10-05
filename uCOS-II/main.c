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

//�ڴ������ر���
OS_MEM * CommTxBuffer1;
INT8U CommTxPart1[8][128];

//��Ϣ������ر���
OS_EVENT * Str_box;

//�ź�����ر���
OS_EVENT * Str_semp;

//�������ź���
OS_EVENT * Str_mutex;

//��ʱ����ر���
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
								&err);                      //������ʱ��
	CommTxBuffer1 = OSMemCreate(CommTxPart1,8,128,&err);	//�����ڴ����
	Str_box       = OSMboxCreate((void *)0);                //������Ϣ����
	Str_semp      = OSSemCreate(0);                         //�����ź���
	Str_mutex     = OSMutexCreate(2,&err);                  //�����������ź���
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
	
	OSTmrStart(Str_Tmr,&err);                    //������ʱ��

	while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{		
		OSTimeDlyHMSM(0,0,0,100);
//		OSTaskSuspend(APP_TASK_START_PRIO);
//		OSTimeDlyHMSM(0,0,3,0);
	}
}

void  TmrCallBack(OS_TMR *ptmr, void *p_arg)
{
	OS_Printf("��ʱ����\n");  //��ʾ��Ϣ
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
			OS_Printf("����1--�����ڴ�ɹ��������룺");  //��ʾ��Ϣ
		}
		else {
			OS_Printf("����1--�����ڴ���һ������");
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
		OS_Printf("����1--������Ϣ���䣬���ȴ�������ɡ�\n");  //��ʾ��Ϣ
		OSMboxPost(Str_box,BlkPtr);                            //������Ϣ���������ǽ��յ��ļ�������
		OSSemPend(Str_semp,0,&err);                            //�ȴ���ӡ����ź�
		OS_Printf("����1--���յ���������źţ��ͷ��ڴ�\n\n");  //��ʾ��Ϣ
		err = OSMemPut(CommTxBuffer1,BlkPtr);                  //�ͷ��ڴ�
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
		OS_Printf("����2--���յ�����Ϣ�ǣ�%s\n",BlkPtr);
		OS_Printf("����2--���ʹ�ӡ����źţ�\n");
		OSSemPost(Str_semp);
//		OSTaskDel(OS_PRIO_SELF);
    }
}
