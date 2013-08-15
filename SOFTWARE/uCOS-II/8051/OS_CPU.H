/*****************************************************************************
|*
|*  Copyright (c) 2001 TASKING, Inc.
|*
|*  Description:
|*
|*		This file contains the processor specific defines
|*
|*		To be used for 8051 family.
|*
 ****************************************************************************/



#ifndef _OS_CPU_H_
#define _OS_CPU_H_

/*
************************************************************
*                      CONSTANTS
************************************************************
*/
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


/*
************************************************************
*                        MACROS
************************************************************
*/
#define  OS_ENTER_CRITICAL()    (EA = 0)
#define  OS_EXIT_CRITICAL()     (EA = 1)
#define  OS_TASK_SW()           OSCtxSw()

/*
************************************************************
*                       DATA TYPES
************************************************************
*/
typedef char            BOOLEAN;    /* Cannot use _bit in structures */
typedef unsigned char   UBYTE;
typedef signed   char   BYTE;
typedef unsigned int    UWORD;
typedef signed   int    WORD;
typedef unsigned long   ULONG;
typedef signed   long   LONG;

typedef unsigned char   INT8U;
typedef signed   char   INT8S;
typedef unsigned int    INT16U;
typedef signed   int    INT16S;
typedef unsigned long   INT32U;
typedef signed   long   INT32S;

/*
************************************************************
*                       MISC
************************************************************
*/
#define	OS_FAR					/* x86-only */
#define	OS_NEAR					/* x86-only */
#define	OS_ROMDATA	_rom			/* const data in ROM */
#define	OS_STK		INT8U			/* Stack is byte-organized */
#define	OS_STK_GROWTH	1			/* Virtual Stack grows from HIGH to LOW in Tasking 8051 Compiler  */

#endif  /* _OS_CPU_H_ */

