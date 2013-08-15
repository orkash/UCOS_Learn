;
;    Startup code automatically generated and updated by:
;    TASKING Tools for 8051 v6.0 r1 (build version: 1.86)
;    Do not edit unless switching off the automatic generation
;    checkbox in EDE | Processor Options | Startup
;
;    Copyright (c) 2001 TASKING, Inc.
;
$CASE
$DEBUG
        NAME     _CSTART

        EXTRN    CODE(_?main)
        EXTRN    CODE(__LK_B_INITIALIZE)
        EXTRN    CODE(__ENDINIT)
        EXTRN    CODE(REENTRANT)

        PUBLIC   __START
        PUBLIC   __STKSTART
        PUBLIC   __EXIT

?STACK  SEGMENT IDATA
        RSEG ?STACK
__STKSTLAB:
        DS      40
__STKSTART      IDATA __STKSTLAB - 1

        PUBLIC __TOP_OF_VIRT_STACK
?VIRT_STACK    SEGMENT XDATA
        RSEG    ?VIRT_STACK
        DS      400
__TOP_OF_VIRT_STACK:

CSEG AT 0x0000

        LCALL __START

STARTUP SEGMENT CODE
        RSEG STARTUP
__START:
        CLR     A
        MOV     PSW, A

        MOV     SP, #LOW(__STKSTART)

        EXTRN   DATA(__SP)
        PUSH    0A8H
        CLR     0AFH
        MOV     __SP, #HIGH(__TOP_OF_VIRT_STACK)
        MOV     __SP+1, #LOW(__TOP_OF_VIRT_STACK)
        POP     0A8H 

        LCALL   __LK_B_INITIALIZE

        MOV     PSW, #(0*8)

        USING   0
        LCALL   _?main

__EXIT:
__STOP:
        SJMP    __STOP

        END
