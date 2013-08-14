#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
//Mini STM32开发板
//LED驱动代码			 
//正点原子@ALIENTEK
//2012/2/27

//LED端口定义
#define LED0 PAout(0)// PD2
#define LED1 PAout(1)// PD3
#define LED2 PAout(2)// PD4	
#define LED3 PDout(7)// PD7	

void LED_Init(void);//初始化

		 				    
#endif
