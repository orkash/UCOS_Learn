/*
*************************************************************************************
**文件说明：gpio配置
**文件名称： gpio_cfg.c 
**创建日期：2013.11.06  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#include "gpio_cfg.h"

/*
*************************************************************************************
* 名    称：void GPIO_Config(Uint16 GPIOMask , Uint16 FMask)
* 功    能：GPIO配置
* 入口参数：Uint16 GPIOMask  端口掩码
            Uint16 FMask     附加码 根据实际操作需求添加功能
* 出口参数：无
*************************************************************************************
*/
void GPIO_Config(Uint16 GPIOMask , Uint16 FMask)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    if(ReadMask(GPIOMask,EN_GPIO_FSMC) == EN_GPIO_FSMC){    //FMSC引脚配置
        
        RCC_APB2PeriphClockCmd(RCC_FSMC_PORT , ENABLE);      //开启FSMC端口时钟
        
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;     //FSMC为复用管脚     
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_InitStructure.GPIO_Pin   = GPIOD_FSMC_PIN;
        GPIO_Init(GPIOD,&GPIO_InitStructure);
     
        GPIO_InitStructure.GPIO_Pin   = GPIOE_FSMC_PIN;
        GPIO_Init(GPIOE,&GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIOF_FSMC_PIN;
        GPIO_Init(GPIOF,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin   = GPIOG_FSMC_PIN;
        GPIO_Init(GPIOG,&GPIO_InitStructure);
    }
    if(ReadMask(GPIOMask,EN_GPIO_KEY) == EN_GPIO_KEY){
        RCC_APB2PeriphClockCmd(RCC_KEY_PORT , ENABLE);      //开启KEY端口时钟
        
        if(ReadMask(FMask, EN_GPIO_KEY) == EN_GPIO_KEY){    
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;   //keyA为推挽输出口
            GPIO_InitStructure.GPIO_Pin   = GPIOA_KEY_PIN;
            GPIO_Init(KEYA_PORT,&GPIO_InitStructure);
            
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;      //keyB为下拉输入口
            GPIO_InitStructure.GPIO_Pin   = GPIOC_KEY_PIN;
            GPIO_Init(KEYB_PORT,&GPIO_InitStructure);
        }
        else{
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;      //keyA为下拉输入口
            GPIO_InitStructure.GPIO_Pin   = GPIOA_KEY_PIN;
            GPIO_Init(KEYA_PORT,&GPIO_InitStructure);
            
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;   //keyB为推挽输出口
            GPIO_InitStructure.GPIO_Pin   = GPIOC_KEY_PIN;
            GPIO_Init(KEYB_PORT,&GPIO_InitStructure);
        }
    }
    if(ReadMask(GPIOMask,EN_GPIO_LED) == EN_GPIO_LED){
        RCC_APB2PeriphClockCmd(RCC_LED_PORT , ENABLE);      //开启LED端口时钟
        
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;   //LED为输出口
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_InitStructure.GPIO_Pin   = GPIOG_LED_PIN;
        GPIO_Init(LED_PORT,&GPIO_InitStructure);
        if(ReadMask(FMask,EN_GPIO_LED) == EN_GPIO_LED){
            LedAllOn();
        }
        else {
            LedAllOff();
        }
    }
    if(ReadMask(GPIOMask,EN_GPIO_LCD) == EN_GPIO_LCD){
        RCC_APB2PeriphClockCmd(RCC_LCD_PORT , ENABLE);      //开启LCD端口时钟
        
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;   //LCD为输出口
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_InitStructure.GPIO_Pin   = GPIOC_LCD_PIN;      //只配置RST管脚
        GPIO_Init(LCD_RST_PORT,&GPIO_InitStructure);        
    }    
}
/*************************************************************************************
* 说明：file end
**************************************************************************************/
