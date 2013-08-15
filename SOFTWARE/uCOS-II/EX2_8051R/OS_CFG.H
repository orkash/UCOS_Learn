/*****************************************************************************
|*
|*  Copyright (c) 2001 TASKING, Inc.
|*
|*  Description:
|*
|*		This file contains the application specific defines
|*
|*		To be used for 8051 family.
|*
 ****************************************************************************/


#ifndef _OS_CFG_H_
#define _OS_CFG_H_

/*
*********************************************************************************************************
*                                          uC/OS CONFIGURATION
*********************************************************************************************************
*/

#define OS_MAX_EVENTS	5				/* Max. number of event control blocks in your application ...  */
										/* ... MUST be >= 2                                             */
#define OS_MAX_MEM_PART	2				/* Max. number of memory partitions ...                         */
										/* ... MUST be >= 2                                             */
#define OS_MAX_QS		5				/* Max. number of queue control blocks in your application ...  */
										/* ... MUST be >= 2                                             */
#define OS_MAX_TASKS	10				/* Max. number of tasks in your application ...                 */
										/* ... MUST be >= 2                                             */
#define OS_LOWEST_PRIO	63				/* Defines the lowest priority that can be assigned ...         */
										/* ... MUST NEVER be higher than 63!                            */

#define OS_TASK_IDLE_STK_SIZE	0x100	/* Idle task stack size (OS_STK_TYPEs)                          */
#define OS_SYS_STACK_SIZE		0x20	/* system stack size											*/

#define OS_CPU_HOOKS_EN			1		/* uC/OS-II hooks are found in the processor port files         */
#define OS_MBOX_EN				0		/* Include code for MAILBOXES                                   */
#define OS_MEM_EN				0		/* Include code for MEMORY MANAGER (fixed sized memory blocks)  */
#define OS_Q_EN					1		/* Include code for QUEUES                                      */
#define OS_SEM_EN				1		/* Include code for SEMAPHORES                                  */
#define OS_TASK_CHANGE_PRIO_EN	0		/* Include code for OSTaskChangePrio()                          */
#define OS_TASK_CREATE_EN		1		/* Include code for OSTaskCreate()                              */
#define OS_TASK_CREATE_EXT_EN	0		/* Include code for OSTaskCreateExt()                           */
#define OS_TASK_DEL_EN			1		/* Include code for OSTaskDel()                                 */
#define OS_TASK_SUSPEND_EN		1		/* Include code for OSTaskSuspend() and OSTaskResume()          */

#define OS_TICKS_PER_SEC		30		/* Set the number of ticks in one second                        */

#endif  /* ndef _OS_CFG_H_ */

