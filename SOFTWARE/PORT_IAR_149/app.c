
#include "includes.h"

#define  TASK_STK_SIZE                  64       /* ���������ջ��С     */


OS_STK   TaskStartStk[TASK_STK_SIZE];

void   TaskStart(void *data);                    /* ǰ���������񣨺����� */



void  main (void)
{
    WDTCTL = WDTPW + WDTHOLD;					/* ��ֹ���Ź�              */

    OSInit();                                              /* ��ʼ��uCOS-II */
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 3);
    OSStart();                                             /* ��ʼ�������  */
}



void  TaskStart (void *pdata)
{
    pdata  = pdata;                                        /* ���κ����壬��ֹ���������� */

    WDTCTL = WDT_MDLY_32;                                  /* ����ʱ�ӽ��ļ��Ϊ32ms     */
    IE1   |= 0x01;                                         /* �����Ź���ʱ���ж�         */

    /*************************************
    *       Ӧ�ó����ʼ��
    *************************************/
    
    P6SEL &= BIT7; 											
    P6DIR |= BIT7;
    
    

    while (1) {
        /******************************************************************************************
        *         ���Դ���
        *
        �� �˴����ǽ�������ֲ�������ȷ��ֻ�Ƕ�ʱ��P1.0����ߵ͵�ƽ����ӦMSP-FET449ѧϰ���P1.0 LED��˸��
        *
        *          MSP430F449
        *     -----------------
        *  /|\|              XIN|-  
        *   | |                 |
        *   --|RST          XOUT|-
        *     |                 |
        *     |             P1.0|-->LED  
        * 
        *******************************************************************************************/
        
    	P6OUT ^= BIT7; 	 								    

        OSTimeDly(50);   /* ��ʱ10��ʱ�ӽ��ģ���������ȴ���ʱ���� */
    }
}
