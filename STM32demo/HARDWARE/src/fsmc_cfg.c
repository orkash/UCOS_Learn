/*
*************************************************************************************
**文件说明：fsmc配置
**文件名称： fsmc_cfg.c 
**创建日期：2013.11.06  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#include "fsmc_cfg.h"

/*
*************************************************************************************
* 名    称：void FSMC_Config(Uint16 FSMCMask,FunctionalState NewState)
* 功    能：FSMC配置
* 入口参数：Uint16 FSMCMask 功能掩码
            FunctionalState NewState
            ENABLE    开启
            DISABLE   关闭
* 出口参数：无
*************************************************************************************
*/
void FSMC_Config(Uint16 FSMCMask,FunctionalState NewState)
{
    FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTimingInitStructure;
    if (ReadMask(FSMCMask,EN_FSMC_PSRAM) == EN_FSMC_PSRAM){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,NewState);                                 //开启FSMC模块时钟
        
        FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime      = 1;
        FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime       = 1;
        FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime         = 16;
        FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 1;
        FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision           = 16;
        FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency           = 1;
        FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode            = FSMC_AccessMode_B;
        
        FSMC_NORSRAMInitStructure.FSMC_Bank                        = FSMC_Bank1_NORSRAM1;
        FSMC_NORSRAMInitStructure.FSMC_DataAddressMux              = FSMC_DataAddressMux_Disable;
        FSMC_NORSRAMInitStructure.FSMC_MemoryType                  = FSMC_MemoryType_NOR;
        FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth             = FSMC_MemoryDataWidth_16b;
        FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode             = FSMC_BurstAccessMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity          = FSMC_WaitSignalPolarity_Low;
        FSMC_NORSRAMInitStructure.FSMC_WrapMode                    = FSMC_WrapMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive            = FSMC_WaitSignalActive_BeforeWaitState;
        FSMC_NORSRAMInitStructure.FSMC_WriteOperation              = FSMC_WriteOperation_Enable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignal                  = FSMC_WaitSignal_Disable;
        FSMC_NORSRAMInitStructure.FSMC_ExtendedMode                = FSMC_ExtendedMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WriteBurst                  = FSMC_WriteBurst_Disable;
        FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct       = &FSMC_NORSRAMTimingInitStructure;
        FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct           = &FSMC_NORSRAMTimingInitStructure;
        
        FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  
        FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, NewState); 
    }

}
/*************************************************************************************
* 说明：file end
**************************************************************************************/
