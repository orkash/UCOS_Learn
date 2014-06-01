/*
*************************************************************************************
**文件说明：dma配置
**文件名称： dma_cfg.c 
**创建日期：2013.11.06  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#include "dma_cfg.h"

/*
*************************************************************************************
* 名    称：void DMA_Config(DMA_ControlDat * ControlDat,Uint16 DMAMask)
* 功    能：DMA配置
* 入口参数：DMA_ControlDat * ControlDat                
            Uint16 DMAMask
* 出口参数：无
*************************************************************************************
*/
void DMA_Config(DMA_ControlDat * ControlDat,Uint16 DMAMask )
{
    DMA_InitTypeDef DMA_InitStructure;
    
    if(ReadMask(DMAMask,EN_DMA_LCD_CLR) == EN_DMA_LCD_CLR) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        DMA_DeInit(DMA1_Channel1);
        DMA_InitStructure.DMA_PeripheralBaseAddr = ControlDat->DMA_PeripheralBaseAddr;           //外设地址
        DMA_InitStructure.DMA_MemoryBaseAddr     = ControlDat->DMA_MemoryBaseAddr;               //存储器地址               
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;         //外设地址增量模式
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;            //存储器地址增量模式
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //外设数据宽度
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;      //存储器数据宽度
        DMA_InitStructure.DMA_BufferSize         = ControlDat->DMA_BufferSize;                   //数据传输数量
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;            //数据传输方向 0：从外设读 1：从存储器读
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;                  //循环模式
        DMA_InitStructure.DMA_Priority           = DMA_Priority_High;                //通道优先级
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Enable;                   //存储器到存储器模式
        DMA_Init(DMA1_Channel1,&DMA_InitStructure);
    }
    if(ReadMask(DMAMask,EN_DMA_SPI1) == EN_DMA_SPI1){
        DMA_DeInit(DMA1_Channel2);
        DMA_InitStructure.DMA_PeripheralBaseAddr = ControlDat->DMA_PeripheralBaseAddr;
        DMA_InitStructure.DMA_MemoryBaseAddr     = ControlDat->DMA_MemoryBaseAddr;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_BufferSize         = ControlDat->DMA_BufferSize;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_High; 
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel2,&DMA_InitStructure);

        DMA_DeInit(DMA1_Channel3);
        DMA_InitStructure.DMA_PeripheralBaseAddr = ControlDat->DMA_PeripheralBaseAddr;
        DMA_InitStructure.DMA_MemoryBaseAddr     = ControlDat->DMA_MemoryBaseAddr;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_BufferSize         = ControlDat->DMA_BufferSize;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium; 
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel3,&DMA_InitStructure);
/*
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
*/
    }
}

/*
*************************************************************************************
* 名    称：void DMA_ChannelCmd(Uint16 DMAMask ,FunctionalState NewState)
* 功    能：DMA通道使能配置
* 入口参数：Uint16 DMAMask ,FunctionalState NewState
* 出口参数：无
*************************************************************************************
*/
void DMA_ChannelCmd(Uint16 DMAMask ,FunctionalState NewState)
{
    if(ReadMask(DMAMask,EN_DMA_LCD_CLR) == EN_DMA_LCD_CLR) {
        DMA_ClearFlag(DMA1_FLAG_GL1|DMA1_FLAG_TC1|DMA1_FLAG_HT1|DMA1_FLAG_TE1);
        DMA_Cmd(DMA1_Channel1,NewState);
        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,NewState);
    }
    if(ReadMask(DMAMask,EN_DMA_SPI1) == EN_DMA_SPI1){
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, NewState);
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, NewState);
    }
}




































