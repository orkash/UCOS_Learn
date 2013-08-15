/*****************************************************************************
|*
|*  Copyright (c) 2001 TASKING, Inc.
|*
|*  Description:
|*
|*	This module implements the processor specific source code
|*	for uC/OS-II.
|*	To be used for the 8051 family.
|*
 ****************************************************************************/


/*
 * INCLUDE FILES
 */
#include	"includes.h"

#if !defined(_CC51)
#  error This piece of code only works with TASKING CC51 V6.0r1 or higher
#endif


/*
 * EXTERNAL VARIABLES
 */
extern	_idat UBYTE _STKSTART[]; 

#pragma asm
	extrn   xdata ( _OSTCBCur )		; OS_TCB * _xdata OSTCBCur;
	extrn   xdata ( _OSTCBHighRdy )		; OS_TCB * _xdata OSTCBHighRdy;
	extrn	xdata ( _OSRunning )
	extrn	xdata ( _OSPrioCur )		; OS_TCB * _xdata OSPrioCur;
	extrn	xdata ( _OSPrioHighRdy )	; OS_TCB * _xdata OSPrioHighRdy;
	extrn	data(__SP)
#pragma endasm

/*
 * -------------------------------------------------------------------------
 * Both the Task stack and the system stack contain
 * the OS context in this format:
 *
 * high addr:	SP
 *		__SP+1	; TASKING CC51
 *		__SP	; Virtual Stack Pointer
 *		R7
 *		R6
 *		R5
 *		R4
 *		R3
 *		R2
 *		R1
 *		R0
 *		DPH
 *		DPL
 *		B
 *		ACC
 *		PSW
 *		PCH
 *		PCL
 * low addr:
 *
 * In TCB stack, SP contains the number of bytes
 * saved after OS context (i.e. user data in stack)
 */

/*
 * -------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------
 */

_inline void save_all( void )
{
#pragma asm
	; Save current task context
	; PCL, PCH have already been pushed by lcall/interrupt
	push	psw		
	push	acc
	push	b
	push	dpl
	push	dph
	push	ar0
	push	ar1
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	push	__SP		; Save CC51 Virtual Stack Pointer
	push	__SP+1		;
	push	sp
#pragma endasm
}

_inline void rest_all( void )
{
#pragma asm
	; Restore task context
	pop 	acc		; SP: discard it
	pop	__SP+1		; Restore Virtual Stack Pointer
	pop	__SP		;
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	pop	ar1
	pop	ar0
	pop	dph
	pop	dpl
	pop 	b
	pop 	acc
	pop 	psw

	; ret/reti will pop PCH, PCL
#pragma endasm
}

/*****************************************************************************
|*
|*  FUNCTION:           OSStartHighRdy
|*
|*  DESCRIPTION:
|*
|*      Start Multitasking
|*
 */
void OSStartHighRdy(void)
{
#pragma asm
	mov	R6, #1			; R6 = 1 : use ret at end of routine

	; Determine HighRdy Task stack start and size

	lcall	_?OSTaskSwHook		; Call OSTaskSwHook;

	mov	DPTR, #_OSRunning
	mov	A, #1
	movx	@DPTR, A		; OSRunning = 1
	
OSStartHighRdy2:
	mov	DPTR, #_OSTCBHighRdy	; DPTR = pointer to OSTCBHighRdy

	movx	A, @DPTR
	mov	R2, A
	inc	DPTR
	movx	A, @DPTR
	mov	R3, A			; R23 = pointer to structure OS_TCB of OSTCBHighRdy

	mov	DPH, R2
	mov	DPL, R3			; DPTR = pointer to structure OS_TCB of OSTCBHighRdy

	movx	A, @DPTR
	mov	R2, A
	inc	DPTR
	movx	A, @DPTR
	mov	R3, A			; R23 = pointer to OSTCBHighRdy->StkPtr

	mov	DPH, R2
	mov	DPL, R3			; DPTR = pointer to OSTCBHighRdy->StkPtr (= Task-stack)

	movx	A, @DPTR
	inc	A			; A contains SP of Task-stack

	clr	C
	subb	A, #LOW(__STKSTART)
	mov	R4, A			; R4 contains number of bytes to copy

	mov	A, DPL
	clr	C
	subb	A, R4
	mov	DPL, A
	mov	A,DPH
	subb	A, #0
	mov	DPH, A			; DPTR = bottom of Task-stack

	mov	R0, #LOW(__STKSTART)	; R0 = bottom of System-stack

OSStartHighRdyCopy:			; Copy Task-stack to System-stack
	inc	DPTR
	inc	R0
	movx	A, @DPTR
	mov	@R0, A
	djnz	R4, OSStartHighRdyCopy	; Task-stack is loaded in System-stack
					; DPTR contains top of Task-stack
					; R0 contains top of system-stack (= SP)

	mov	SP, R0			; SP points to top of System-stack

	; If R6 = 1 then return with ret, else return with reti
	cjne	R6, #1, OSStartHighRdyRTI

	; Pop all user register from task stack
#pragma endasm
	rest_all();
#pragma asm
	setb	ea			; Enable interrupts again
	ret				; Pop PCH,PCL

OSStartHighRdyRTI:
	; Pop all user register from task stack
#pragma endasm
	rest_all();
#pragma asm
	setb	ea			; Enable interrupts again
	reti				; Pop PCH,PCL and restore the interrupt logic
					; as just before the interrupt occurs
#pragma endasm
}

/*****************************************************************************
|*
|*  FUNCTION:           OSCtxSw
|*
|*  DESCRIPTION:
|*
|*      Perform a context switch (from task level)
|*
 */
void OSCtxSw(void)
{
	save_all();			/* push registers on stack */
#pragma asm
	mov	R6, #1			; R6 = 1 : use ret at end of routine

OSCtxSw2:
	mov	DPTR, #_OSTCBCur	; DPTR = &OSTCBCur

	movx	A, @DPTR
	mov	R2, A
	inc	DPTR
	movx	A, @DPTR
	mov	R3, A			; R23 = pointer to structure OS_TCB of OSTCBCur

	mov	DPH, R2
	mov	DPL, R3			; DPTR = pointer to structure OS_TCB of OSTCBCur

	movx	A, @DPTR
	mov	R2, A
	inc	DPTR
	movx	A, @DPTR
	mov	R3, A			; R23 = pointer to OSTCBCur->StkPtr

	mov	DPH, R2
	mov	DPL, R3			; DPTR = pointer to OSTCBCur->StkPtr (= Task-stack as last saved)

	movx	A, @DPTR
	inc	A			; A contains SP as last saved

	clr	C
	subb	A, #LOW(__STKSTART)
	mov	R4, A			; R4 contains number of bytes on Task-stack

	mov	A, DPL
	clr	C
	subb	A, R4
	mov	DPL, A
	mov	A,DPH
	subb	A, #0
	mov	DPH, A			; DPTR = bottom of Task-stack

	mov	R0, #LOW(__STKSTART)	; R0 = bottom of System-stack

	mov	A, SP
	clr	C
	subb	A, R0
	mov	R4, A			; R4 contains number of bytes on System-stack

OSCtxSwCopy:				; Copy System-stack to Task-stack
	inc	DPTR
	inc	R0
	mov	A, @R0
	movx	@DPTR, A
	djnz	R4, OSCtxSwCopy		; System-stack is saved to Task-stack
					; DPTR contains top of Task-stack (= SP)

	mov	R0, DPH
	mov	R1, DPL			; R01 contains top of Task-stack

	mov	DPTR, #_OSTCBCur	; DPTR = pointer to OSTCBCur

	movx	A, @DPTR
	mov	R2, A
	inc	DPTR
	movx	A, @DPTR
	mov	R3, A			; R23 = pointer to OSTCBCur->StkPtr

	mov	DPH, R2
	mov	DPL, R3			; DPTR = pointer to OSTCBCur->StkPtr

	mov	A, R0
	movx	@DPTR, A
	inc	DPTR
	mov	A, R1
	movx	@DPTR, A		; Top of Task-stack saved in OSTCBCur->StkPtr

	mov	DPTR, #_OSTCBHighRdy	; DPTR = pointer to OSTCBHighRdy	
	movx	A, @DPTR
	mov	R0, A
	inc	DPTR
	movx	A, @DPTR
	mov	R1, A			; R01 = OSTCBHighRdy

	mov	DPTR, #_OSTCBCur
	mov	A, R0
	movx	@DPTR, A
	inc	DPTR
	mov	A, R1
	movx	@DPTR, A		; OSTCBCur = OSTCBHighRdy

	lcall	_?OSTaskSwHook		; Call OSTaskSwHook;

	mov	DPTR, #_OSPrioHighRdy	; DPTR = pointer to OSTCBHighRdy	
	movx	A, @DPTR
	mov	R0, A
	inc	DPTR
	movx	A, @DPTR
	mov	R1, A			; R01 = OSPrioHighRdy

	mov	DPTR, #_OSPrioCur
	mov	A, R0
	movx	@DPTR, A
	inc	DPTR
	mov	A, R1
	movx	@DPTR, A		; OSPrioCur = OSPrioHighRdy


	; Load new task: OSTCBCur
	;
	ljmp    OSStartHighRdy2
#pragma endasm
}

/*****************************************************************************
|*
|*  FUNCTION:           OSIntCtxSw
|*
|*  DESCRIPTION:
|*
|*      Perform a context switch (from an ISR)
|*	Registers already saved by the interrupt routine
|*
 */
void OSIntCtxSw(void)
{
#pragma asm
	mov	R6, #0			; R6 = 0 : use reti at end of routine

	; Correct stack poiner for:
	;   - call to OSIntExit  (2 bytes)
	;   - call to OSIntCtxSw (2 bytes)
	;
	mov	a, sp
	clr	c
	subb	a, #4
	mov	sp, a

	; From now on it is the same as OSCtxSw:

	ljmp	OSCtxSw2
#pragma endasm
}

/*****************************************************************************
|*
|*  FUNCTION:           OSTaskCreate
|*
|*  DESCRIPTION:
|*
|*      Create a new task
|*
 */
void *OSTaskStkInit( void (*task)(void *pd), void *pdata, void *ptos, INT16U opt)
{
	OS_STK	* stk;
	OS_STK	* vstack_ptr;


	opt = opt;

	stk = (OS_STK *) ptos - 1 - OS_SYS_STACK_SIZE;		/* SP increments before push */
	vstack_ptr = (OS_STK *) ptos - OS_SYS_STACK_SIZE;	/* __SP decrements before push */

	/* WARNING: Next code must match with ucos51a.asm */

	*stk++	= ((UWORD) task & 0xFF);            	/* PCL    */
	*stk++  = ((UWORD) task >> 8);              	/* PCH    */
	*stk++  = 0x00;					/* PSW    */
	*stk++  = 0x11;					/* ACC    */
	*stk++  = 0x22;					/* B      */
	*stk++  = 0x33;					/* DPL    */
	*stk++  = 0x44;					/* DPH    */
	*stk++  = 0x55;					/* R0     */
	*stk++  = 0x66;					/* R1     */
	*stk++  = 0x77;					/* R2     */
	*stk++  = 0x88;					/* R3     */
	*stk++  = 0x99;					/* R4     */
	*stk++  = 0x00;					/* R5     */
	*stk++  = ((UWORD) pdata >> 8);			/* R6     */
	*stk++  = ((UWORD) pdata & 0xFF);		/* R7     */
	*stk++	= ((UWORD) vstack_ptr >> 8);		/* __SP   */
	*stk++	= ((UWORD) vstack_ptr & 0xFF);		/* __SP+1 */
	*stk	= 0x11 + (UBYTE) _STKSTART;		/* SP 0x11 : pushing the SP saves the value */
    							/* 	      of the SP before the push */

	return ((void *) stk);
}



/*****************************************************************************
|*
|*  Function:           Timer0IntHandler
|*
|*  Description:
|*
|*      This interrupt function will be called every time Timer 0
|*	overflows.
|*
 */
_interrupt(1) _frame(PSW, A, B, DPL, DPH, R0, R1, R2, R3, R4, R5, R6, R7) void Timer0IntHandler( void )
{

	/* save additional data:
	 * virtual stackpointer __SP
	 * stackpointer SP
	 */

#pragma asm

	push	__SP
	push	__SP + 1

	push	SP

#pragma endasm

	OSIntEnter();

	/* the timer 0 overflow flag is automatically reset by hardware */

	/* no auto-reload in mode 1 */
	TL0 = 0x00;
	TH0 = 0x00;

	OSTimeTick();

	OSIntExit();

#pragma asm

	pop	ACC	

	pop	__SP + 1
	pop	__SP

#pragma endasm

}



#if OS_CPU_HOOKS_EN
/*****************************************************************************
|*
|*  FUNCTION:           OSTaskCreateHook
|*
|*  DESCRIPTION:
|*
|*      empty
|*
 */
void OSTaskCreateHook(OS_TCB *ptcb)
{
	ptcb = ptcb;
}
#endif /* OS_CPU_HOOKS_EN */



#if OS_CPU_HOOKS_EN
/*****************************************************************************
|*
|*  FUNCTION:           OSTaskDelHook
|*
|*  DESCRIPTION:
|*
|*      Empty
|*
 */
void OSTaskDelHook(OS_TCB *ptcb)
{
	ptcb = ptcb;
}
#endif /* OS_CPU_HOOKS_EN */



#if OS_CPU_HOOKS_EN
/*****************************************************************************
|*
|*  FUNCTION:           OSTaskSwHook
|*
|*  DESCRIPTION:
|*
|*      Empty
|*
 */
void OSTaskSwHook(void)
{
}
#endif /* OS_CPU_HOOKS_EN */



#if OS_CPU_HOOKS_EN
/*****************************************************************************
|*
|*  FUNCTION:           OSTaskStatHook
|*
|*  DESCRIPTION:
|*
|*      Empty
|*
 */
void OSTaskStatHook(void)
{
}
#endif /* OS_CPU_HOOKS_EN */



#if OS_CPU_HOOKS_EN
/*****************************************************************************
|*
|*  FUNCTION:           OSTimeTickHook
|*
|*  DESCRIPTION:
|*
|*      Empty
|*
 */
void OSTimeTickHook(void)
{
}
#endif /* OS_CPU_HOOKS_EN */


