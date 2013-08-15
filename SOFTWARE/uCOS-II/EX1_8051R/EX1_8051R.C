/*****************************************************************************
|*
|*  Copyright (c) 2001 TASKING, Inc.
|*
|*  Description:
|*
|*	This module shows an example how to use uC/OS.
|*	It implements 5 tasks:
|*	Stat() calculates some statistics about the processor usage
|*	DispStr() is used to display string created by other tasks
|*	Task() sets/resets an I/O pin and creates a string to display by DispStat()
|*	Task() is created 3 times, each with other startup data.
|*	Stat() is created in main(). The other tasks are created by Task();
|*
 ****************************************************************************/

#include "includes.h"

#define	VIRT_STACK_1	0x400
OS_STK		Stk3[VIRT_STACK_1 + OS_SYS_STACK_SIZE];

#define	VIRT_STACK_2	0x400
OS_STK		Stk2[VIRT_STACK_2 + OS_SYS_STACK_SIZE];

#define	VIRT_STACK_3	0x400
OS_STK		Stk1[VIRT_STACK_3 + OS_SYS_STACK_SIZE];

#define	VIRT_STACK_STAT	0x400
OS_STK		Stk_stat[VIRT_STACK_STAT + OS_SYS_STACK_SIZE];

#define	VIRT_STACK_DISP	0x400
OS_STK		Stk_disp[VIRT_STACK_DISP + OS_SYS_STACK_SIZE];

#define printfQ_SIZE	10


char		TaskMsg[3][40];			/* Messages from task 1..3 */
char		StatMsg[100];			/* Message from Stat() */


OS_EVENT	* printfSem;			/* semaphore for printf */
OS_EVENT	* printfQPtr;			/* pointer to queue for printf messages */
void		* printfQ[printfQ_SIZE];	/* printf queue */


void Task(void *pdata);
void Stat(void *data);
void DispStr(void *data);
void StartTimer0(void);
int main( void );

/*
 * Data[] is organized as "taskno,bitno,delay"
 */

int	Data1[] = {1,7,20};
int	Data2[] = {2,6,40};
int	Data3[] = {3,5,80};



/*****************************************************************************
|*
|*  FUNCTION:           Task
|*
|*  DESCRIPTION:
|*
|*      Set/reset I/O pin (P1x) and post a message to display a string
|*
 */
void Task(void *pdata)
{
	unsigned char	err;

	unsigned char	output;				/* used to display if pin is set or reset */
	unsigned char	port;				/* used to displat which pin is set */
	unsigned char	task;				/* used to display which task set the pin */

	unsigned char	mask;				/* holds bitmask for the pin */
	unsigned int	dlay;				/* timeticks between toggling the pin */


	task = ((int *)pdata)[0] & 0xFF;
	port = ((int *)pdata)[1] & 0xFF;
	mask = 1 << port;
	dlay = ((int *)pdata)[2];

	/* avoid compiler warning */
	pdata = pdata;

	while (1)
	{
		/* Toggle P1.<bitno> */
		P1 ^= mask;
		output = (P1 & mask)? 1 : 0;

		/* wait for semaphore to use the printf routine (not reentrant) */
		OSSemPend(printfSem, 0, &err);
		sprintf(TaskMsg[task],"Task %d set output P1.%d to %d \n", (int) task, (int) port, (int) output);
		OSSemPost(printfSem);

		/* Post message to display the string */
		OSQPost(printfQPtr, &TaskMsg[task][0]);

		/* wait for a certain time */
		OSTimeDly(dlay);
	}
}


/*****************************************************************************
|*
|*  FUNCTION:           Stat
|*
|*  DESCRIPTION:
|*
|*      Starts the other tasks and compute CPU usage
|*
 */
void Stat(void *data)
{
	unsigned long	max;					/* Maximal idle count if no other processes run */
	int		usage;					/* Percent of used CPU time */
	unsigned char	err;					/* Contains err if OSSemPend() fails */


	data = data;					/* Prevent compiler warning */

	OSSemPend(printfSem, 0, &err);
	printf("\nDetermining  CPU's capacity ...\n");	/* Determine maximum count for OSIdleCtr */
	OSSemPost(printfSem);

	OSTimeDly(1);					/* Synchronize to clock tick */
	OSIdleCtr = 0L;					/* Determine max. idle counter value ... */
	OSTimeDly(15);					/* ... for 15 clock ticks (about 1 sec at 12 MHz) */
	max = OSIdleCtr;

	OSTaskCreate(Task, Data3, &Stk3[VIRT_STACK_3 + OS_SYS_STACK_SIZE], 20);
	OSTaskCreate(Task, Data2, &Stk2[VIRT_STACK_2 + OS_SYS_STACK_SIZE], 30);
	OSTaskCreate(Task, Data1, &Stk1[VIRT_STACK_1 + OS_SYS_STACK_SIZE], 40);
	OSTaskCreate(DispStr, (void *)0, &Stk_disp[VIRT_STACK_DISP + OS_SYS_STACK_SIZE], 60);

	OSIdleCtr = 0L;

	while (1)
	{
		OS_ENTER_CRITICAL();

		/* Reset statistics counters */
		OSCtxSwCtr = 0L;
		OSIdleCtr  = 0L;
		OS_EXIT_CRITICAL();

		/* Wait about one second */
		OSTimeDly(15);

		/* Compute and display statistics */
		usage = 100 - ((100 * OSIdleCtr) / max);
		OSSemPend(printfSem, 0, &err);
		sprintf(StatMsg, "***** Task Switches/sec: %ld   CPU Usage: %d percent   Idle Ctr: %ld / %ld \n",
                	OSCtxSwCtr, usage, OSIdleCtr, max);
		OSQPost(printfQPtr, &StatMsg[0]);
		OSSemPost(printfSem);
	}
}


/*****************************************************************************
|*
|*  FUNCTION:           DispStr
|*
|*  DESCRIPTION:
|*
|*      After receiving a message, display the corresponding string
|*
 */
void DispStr(void *data)
{
	unsigned char	err;					/* Contains err if OSQPend() fails */	
	char		* s;					/* pointer to string to display */

	data = data;					/* avoid compiler warnings */

	while (1)
	{
		/* Wait forever for message */
		s = OSQPend(printfQPtr, 0, &err);

		/* Wait for semaphore and print string */
		OSSemPend(printfSem, 0, &err);
		printf(s);
		OSSemPost(printfSem);
	}
}

/*****************************************************************************
|*
|*  FUNCTION:           StartTimer0
|*
|*  DESCRIPTION:
|*
|*      Initialize Timer 0
|*
 */
void StartTimer0(void)
{
	unsigned char Temp;

	/* Program Timer 0:
	 *     Gate = 0 => timer enabled on TR0
	 *     C/nT = 0 => source = 1/12 fOSC
	 *     mode = 1 => 16-Bit counter/timer
	 */

	Temp = TMOD;
	Temp &= 0xF0;
	Temp |= 0x01;
	TMOD = Temp;

	TL0 = 0;
	TH0 = 0;

	ET0 = 1;		/* Enable Interrupt on Timer 0 Overflow */

	TR0 = 1;		/* Start Timer 0 */
}


/*****************************************************************************
|*
|*  FUNCTION:           main
|*
|*  DESCRIPTION:
|*
|*      Create first task (Stat()) and start OS
|*
 */
int main( void )
{
	OSInit();

	printfSem = OSSemCreate(1);
	printfQPtr = OSQCreate(&printfQ[0], printfQ_SIZE);

	OSTaskCreate(Stat, (void *)0, &Stk_stat[VIRT_STACK_STAT + OS_SYS_STACK_SIZE], 50);

	OS_ENTER_CRITICAL();

	StartTimer0();

	/* Start multitasking - will do OS_EXIT_CRITICAL() */
	OSStart();

	return 0;	/* will never happen... */
}



