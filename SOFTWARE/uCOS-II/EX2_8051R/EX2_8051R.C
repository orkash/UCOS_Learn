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
#include "lcd.h"

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

#define printfQ_SIZE	5


unsigned char		msg[4][2];			/* messages to send to DispStr() */

OS_EVENT	* printfSem;				/* semaphore for printf */
OS_EVENT	* printfQPtr;				/* pointer to queue for printf messages */
void		* printfQ[printfQ_SIZE];		/* printf queue */

void Task(void *pdata);
void Stat(void *data);
void DispStr(void *data);
void StartTimer0(void);
int main( void );

/*
 * Data[] is organized as "taskno,bitno,delay"
 */

int	Data1[] = {1,7,10};
int	Data2[] = {2,6,20};
int	Data3[] = {3,5,40};



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
	unsigned char	output;				/* used to display if pin is set or reset */
	unsigned char	portnr;				/* used to displat which pin is set */
	unsigned char	tasknr;				/* used to display which task set the pin */

	unsigned char	mask;				/* holds bitmask for the pin */
	unsigned int	dlay;				/* timeticks between toggling the pin */


	tasknr = ((int *)pdata)[0] & 0xFF;		/* determine task number... */
	portnr = ((int *)pdata)[1] & 0xFF;		/* ...port number... */
	mask = 1 << portnr;
	dlay = ((int *)pdata)[2];			/* ...and delay */

	/* avoid compiler warning */
	pdata = pdata;

	while (1)
	{
		/* Toggle P1.<bitno> */
		P1 ^= mask;
		output = (P1 & mask)? 1 : 0;

		/* write output to global */
		msg[tasknr][0] = tasknr;
		msg[tasknr][1] = output;

		/* Post message to display the string */
		OSQPost(printfQPtr, &msg[tasknr]);

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
	unsigned long	max;				/* Maximal idle count if no other processes run */
	int		usage;				/* Percent of used CPU time */
	unsigned char	err;				/* Contains err if OSSemPend() fails */
	char		str[12];


	data = data;					/* Prevent compiler warning */

	InitLcd();					/* initialize the LCD */

	strcpy( str, "CALC. REF. ");
	OSSemPend(printfSem, 0, &err);
	WriteLcdStr( str );
	OSSemPost(printfSem);

	OSTimeDly(1);					/* Synchronize to clock tick */
	OSIdleCtr = 0L;					/* Determine max. idle counter value ... */
	OSTimeDly(30);					/* ... for 30 clock ticks */
	max = OSIdleCtr;

	P1 = 0xE0;

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
		OSTimeDly(30);

		/* Compute and post statistics */
		usage = 100 - ((100 * OSIdleCtr) / max);
		msg[0][0] = 0;
		msg[0][1] = usage;
		OSQPost(printfQPtr, &msg[0]);
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
	unsigned char	err;				/* Contains err if OSQPend() fails */
	static char	str[12];	
	char		* s;				/* pointer to message char */

	data = data;					/* avoid compiler warnings */

	s = str;					/* point to str */
	strcpy(s, "00  P.0.0.0.");			/* init string */ 

	while (1)
	{
		/* Wait forever for message */
		s = OSQPend(printfQPtr, 0, &err);

		switch (s[0])
		{
		case 0:
			/* update CPU usage */
			str[0] = (s[1] / 10) + '0';
			str[1] = (s[1] % 10) + '0';
			break;
		case 1:
			/* update port 1 */
			str[10] = s[1] + '0'; 
			break;
		case 2:
			/* update port 2 */
			str[8] = s[1] + '0'; 
			break;
		case 3:
			/* update port 3 */
			str[6] = s[1] + '0'; 
			break;
		default:
			/* do nothing */
			break;
		}

		/* wait for semaphore before use of the LCD routine */
		OSSemPend(printfSem, 0, &err);
		WriteLcdStr( str );
		OSSemPost(printfSem);
	}
}

/*****************************************************************************
|*
|*  FUNCTION:           StartTimer0
|*
|*  DESCRIPTION:
|*
|*      Initialize Timer 0 (used as timer for OSTimeTick, see OS_CPU_C.C).
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



