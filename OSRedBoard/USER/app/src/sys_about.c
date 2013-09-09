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

//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void SysTickConfig(Uint8 SYSCLK)
{
#ifdef OS_CRITICAL_METHOD   //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
  Uint32 reload;
#endif
  SysTick->CTRL &= ~(1 << 2);               //SYSTICK使用外部时钟源        
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了.
  reload = SYSCLK / 8;                      //每秒钟的计数次数 单位为K     
  reload *= 1000000 / OS_TICKS_PER_SEC;     //根据OS_TICKS_PER_SEC设定溢出时间
                                            //reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右  
  SysTick->CTRL |= 1 << 1;                  //开启SYSTICK中断
  SysTick->LOAD = reload;                   //每1/OS_TICKS_PER_SEC秒中断一次  
  SysTick->CTRL |= 1 << 0;                  //开启SYSTICK    
#endif
}
