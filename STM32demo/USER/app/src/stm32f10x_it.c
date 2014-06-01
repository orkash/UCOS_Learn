/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "user_cfg.h"
#include "lcd_tft.h"
#include "includes.h"



 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
#ifndef USER_UCOSII
void DMA1_Channel1_IRQHandler(void)
{
#if (LCD_DMA_CLR_EN > 0) || (LCD_DMA_DAT_EN > 0)
    switch(LcdDmaDat.DMA_Mode){
        case EN_DMA_LCD_CLR :
            LcdDmaDat.DMA_InterruptCnt ++;
            if(LcdDmaDat.DMA_InterruptCnt > LcdDmaDat.DMA_InterruptMax){
                DMA_ChannelCmd(EN_DMA_LCD_CLR,DISABLE);
            }
            break;
        case EN_DMA_LCD_DAT :
            break;
        default : break;
    }
    DMA_ClearITPendingBit(DMA1_IT_TC1);    
#endif    
}
#else
void DMA1_Channel1_IRQHandler(void)
{
    OSIntEnter();
#if (LCD_DMA_CLR_EN > 0) || (LCD_DMA_DAT_EN > 0)
    switch(LcdDmaDat.DMA_Mode){
        case EN_DMA_LCD_CLR :
            LcdDmaDat.DMA_InterruptCnt ++;
            if(LcdDmaDat.DMA_InterruptCnt > LcdDmaDat.DMA_InterruptMax){
                DMA_ChannelCmd(EN_DMA_LCD_CLR,DISABLE);
            }
            break;
        case EN_DMA_LCD_DAT :
            break;
        default : break;
    }
    DMA_ClearITPendingBit(DMA1_IT_TC1);    
#endif    
    OSIntExit();
}
#endif
 
/*
void PendSV_Handler(void)
{
}
*/ 
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
//systick中断服务函数,使用ucos时用到
void SysTick_Handler(void)
{				   
	OSIntEnter();		//进入中断
    OSTimeTick();       //调用ucos的时钟服务程序               
    OSIntExit();        //触发任务切换软中断
}
#endif


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
