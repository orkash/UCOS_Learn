/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                              (c) Copyright 2002, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                               TI MSP430 
*
*
* File         : OS_CPU.H
* By           : Alley Zhou(zzy@lierda.com)
*                Jean J. Labrosse
*********************************************************************************************************
*/

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              数据类型
*                                         
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* 无符号  8 bit                    */
typedef signed   char  INT8S;                    /* 有符号  8 bit                    */
typedef unsigned int   INT16U;                   /* 无符号  16 bit                   */
typedef signed   int   INT16S;                   /* 有符号  16 bit                   */
typedef unsigned long  INT32U;                   /* 无符号  32 bit                   */
typedef signed   long  INT32S;                   /* 有符号  32 bit                   */
typedef float          FP32;                     /* 单精度浮点数                     */
typedef double         FP64;                     /* 双精度浮点数                     */

typedef unsigned int   OS_STK;                   /* 堆栈宽度16位                     */
typedef unsigned int   OS_CPU_SR;                /* 状态寄存器宽度16位               */

/* 
*********************************************************************************************************
*                                             TI MSP430
*
* 方法 #1:   用简单指令开关中断。
*            注意，用方法1关闭中断，从调用函数返回后中断会重新打开！
*
*
* 方法 #2:   中断的势能与否与先前的中断状态有关，比如，在进入临界段前中断势能关闭的话，
*            退出临界段时中断势能仍然关闭。
*
* 方法 #3    中断势能与否与先前的中断状态有关。先将状态寄存器SR存储到局部变量CPU_SR中，
*            然后关闭中断，UCOS－II在需要关闭中断的地方都分配一个局部变量CPU_SR，
*            最后通过将CPU_SR的值复制到状态寄存器来恢复中断势能状态。
*********************************************************************************************************
*/
#define  OS_CRITICAL_METHOD    3


#if      OS_CRITICAL_METHOD == 1
#define  OS_ENTER_CRITICAL()		_DINT()				  /* 关中断                        */
#define  OS_EXIT_CRITICAL()			_EINT()				  /* 开中断                        */
#endif

#if      OS_CRITICAL_METHOD == 2
#define  OS_ENTER_CRITICAL()                              /* 关中断                        */
#define  OS_EXIT_CRITICAL()                               /* 开中断                        */
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  (cpu_sr = OSCPUSaveSR())    /* 关中断                        */
#define  OS_EXIT_CRITICAL()   (OSCPURestoreSR(cpu_sr))    /* 开中断                        */
#endif

/*
*********************************************************************************************************
*                                             MSP430
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1                       /* MSP430 的堆栈从高到底             */

#define  OS_TASK_SW()         OSCtxSw()				  /* 任务级任务切换函数                */

/*
*********************************************************************************************************
*                                           全局变量
*********************************************************************************************************
*/

OS_CPU_EXT  OS_STK  *OSISRStkPtr;                    /* 中断服务程序堆栈指针               */

/*
*********************************************************************************************************
*                                            函数声明
*********************************************************************************************************
*/

OS_CPU_SR  OSCPUSaveSR(void);
void       OSCPURestoreSR(OS_CPU_SR cpu_sr);
