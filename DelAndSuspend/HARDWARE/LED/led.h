#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
//Mini STM32������
//LED��������			 
//����ԭ��@ALIENTEK
//2012/2/27

//LED�˿ڶ���
#define LED0 PDout(2)// PD2
#define LED1 PDout(3)// PD3
#define LED2 PDout(4)// PD4	
#define LED3 PDout(7)// PD7	

void LED_Init(void);//��ʼ��

		 				    
#endif
