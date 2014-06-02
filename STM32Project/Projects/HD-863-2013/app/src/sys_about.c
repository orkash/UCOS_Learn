/*
*************************************************************************************
**文件说明：系统配置文件
**文件名称： sys_about.c
**创建日期：2013.09.08
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#include "user_cfg.h"
#include "drivers.h"




/*
*************************************************************************************
* 名    称：void SysTickConfig(void)
* 功    能：ucosii系统时钟节拍配置
* 入口参数：无
* 出口参数：无
*************************************************************************************
*/
void SysTickConfig(void)
{
#ifdef OS_CRITICAL_METHOD  //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    Uint32 reload;
#endif
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //SYSTICK时钟源
#ifdef OS_TICKS_PER_SEC                                   //如果时钟节拍数定义了,说明要使用ucosII了.
    reload = SystemCoreClock  / OS_TICKS_PER_SEC;         //根据OS_TICKS_PER_SEC设定溢出时间
    SysTick_Config(reload);                               //reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右
#endif
}


/*
*************************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：NVIC中断设置
* 入口参数：无
* 出口参数：无
*************************************************************************************
*/
void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}



/*
*************************************************************************************
* 名    称：void SysInit(void)
* 功    能：系统初始化
* 入口参数：无
* 出口参数：无
*************************************************************************************
*/
void SysInit(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

    NVIC_Configuration();                     //中断配置

    SystemInit();                             //系统时钟配置
//    SysTickConfig();                          //ucos系统时钟配置
    GPIO_Config(EN_GPIO_LED,0);               //
    //LCDx_Init();
    RTC_Prepare();
    SD_Init();
    FSMC_Config(EN_FSMC_PSRAM,ENABLE);       //开启PSRAM FSMC接口


}

