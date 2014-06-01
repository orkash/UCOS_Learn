/*
*************************************************************************************
**文件说明：gpio配置
**文件名称： gpio_cfg.h 
**创建日期：2013.11.06  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#ifndef GPIO_CFG_H
#define GPIO_CFG_H
#include "user_type.h"
#include "stm32f10x.h"


/**************************************************
说明：gpio端口使能掩码    
可根据工程需要继续添加，并在代码文件中添加相应处理。
**************************************************/
#define  EN_GPIO_KEY        ((Uint16)0x0001)
#define  EN_GPIO_SPI        ((Uint16)0x0002)
#define  EN_GPIO_USART1     ((Uint16)0x0010)
#define  EN_GPIO_USB        ((Uint16)0x0020)
#define  EN_GPIO_I2C2       ((Uint16)0x0100)
#define  EN_GPIO_FSMC       ((Uint16)0x0200)
#define  EN_GPIO_LED        ((Uint16)0x0400)
#define  EN_GPIO_LCD        ((Uint16)0x0800)
#define  EN_GPIO_OTHER_IN   ((Uint16)0x1000)
#define  EN_GPIO_OTHER_OUT  ((Uint16)0x2000)



/**************************************************
说明：FSMC端口相关
**************************************************/
#define RCC_FSMC_PORT                                \
        RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE  \
      | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG 

#define GPIOD_FSMC_PIN                                          \
        GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_3  | GPIO_Pin_4    \
      | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7  | GPIO_Pin_8    \
      | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12   \
      | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15

#define GPIOE_FSMC_PIN                                          \
        GPIO_Pin_3  | GPIO_Pin_4                                \
      | GPIO_Pin_5  | GPIO_Pin_7  | GPIO_Pin_8                  \
      | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12   \
      | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
#define GPIOF_FSMC_PIN                                          \
        GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  |GPIO_Pin_3     \
      | GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_12                 \
      | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
 
#define GPIOG_FSMC_PIN                                          \
        GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3    \
      | GPIO_Pin_4  | GPIO_Pin_5


           

/**************************************************
说明：KEY端口相关
**************************************************/
#define RCC_KEY_PORT   RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
#define KEYA_PORT       GPIOA
#define KEYB_PORT       GPIOC
#define GPIOA_KEY_PIN   GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3
#define GPIOC_KEY_PIN   GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3

/**************************************************
说明：LED端口相关
**************************************************/
#define RCC_LED_PORT   RCC_APB2Periph_GPIOG
#define LED_PORT       GPIOG
#define LED_RED        GPIO_Pin_15
#define LED_BLUE       GPIO_Pin_13
#define LED_GREEN      GPIO_Pin_14
#define GPIOG_LED_PIN  LED_RED | LED_BLUE | LED_GREEN
#define LedRedOn()     GPIO_ResetBits(LED_PORT,LED_RED)
#define LedRedOff()    GPIO_SetBits(  LED_PORT,LED_RED) 
#define LedBlueOn()    GPIO_ResetBits(LED_PORT,LED_BLUE)
#define LedBlueOff()   GPIO_SetBits(  LED_PORT,LED_BLUE) 
#define LedGreenOn()   GPIO_ResetBits(LED_PORT,LED_GREEN)
#define LedGreenOff()  GPIO_SetBits(  LED_PORT,LED_GREEN) 
#define LedAllOn()     GPIO_ResetBits(LED_PORT,LED_RED | LED_BLUE | LED_GREEN)
#define LedAllOff()    GPIO_SetBits(  LED_PORT,LED_RED | LED_BLUE | LED_GREEN)

/**************************************************
说明：LCD端口相关
**************************************************/
#define RCC_LCD_PORT   RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF
#define LCD_RST_PIN    GPIO_Pin_5
#define LCD_PWM_PIN    GPIO_Pin_11
#define GPIOC_LCD_PIN  LCD_RST_PIN
#define GPIOF_LCD_PIN  LCD_PWM_PIN
#define LCD_RST_PORT   GPIOC
#define LCD_PWM_PORT   GPIOF
#define LcdRst()                                  \
do{                                               \
    GPIO_ResetBits(LCD_RST_PORT,LCD_RST_PIN);     \
    __nop();                                      \
    __nop();                                      \
    __nop();                                      \
    __nop();                                      \
    __nop();                                      \
    GPIO_SetBits(LCD_RST_PORT,LCD_RST_PIN);       \
}while(0)
    
    





/*
*************************************************************************************
* 名    称：void GPIO_Config(Uint16 GPIOMask , Uint16 FMask)
* 功    能：GPIO配置
* 入口参数：Uint16 GPIOMask  端口掩码
            Uint16 FMask     附加码  根据实际操作需求添加功能
* 出口参数：无
*************************************************************************************
*/
extern void GPIO_Config(Uint16 GPIOMask , Uint16 FMask);

#endif
