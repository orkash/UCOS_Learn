#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
//Mini STM32开发板
//LED驱动代码			 
//正点原子@ALIENTEK
//2012/2/27

//LED端口定义
#define LED0 PDout(2)// PD2
#define LED1 PDout(3)// PD3
#define LED2 PDout(4)// PD4	
#define LED3 PDout(7)// PD7	

void LED_Init(void);//初始化

		 				    
#endif
