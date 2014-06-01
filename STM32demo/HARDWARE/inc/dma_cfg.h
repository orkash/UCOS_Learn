/*
*************************************************************************************
**文件说明：dma配置
**文件名称： dma_cfg.h 
**创建日期：2013.11.06  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#ifndef DMA_CFG_H
#define DMA_CFG_H
#include "user_type.h"
#include "stm32f10x.h"

/**************************************************
说明：DMA 开启掩码
**************************************************/
#define EN_DMA_LCD_CLR            ((Uint16)0x0001)
#define EN_DMA_LCD_DAT            ((Uint16)0x0002)
#define EN_DMA_SPI1               ((Uint16)0x1000)






/*
*************************************************************************************
* 名    称：void DMA_Config(DMA_ControlDat * ControlDat,Uint16 DMAMask)
* 功    能：DMA配置
* 入口参数：DMA_ControlDat * ControlDat                
            Uint16 DMAMask
* 出口参数：无
*************************************************************************************
*/
extern void DMA_Config(DMA_ControlDat * ControlDat,Uint16 DMAMask );

/*
*************************************************************************************
* 名    称：void DMA_ChannelCmd(Uint16 DMAMask ,FunctionalState NewState)
* 功    能：DMA通道使能配置
* 入口参数：Uint16 DMAMask ,FunctionalState NewState
* 出口参数：无
*************************************************************************************
*/
extern void DMA_ChannelCmd(Uint16 DMAMask ,FunctionalState NewState);





#endif
