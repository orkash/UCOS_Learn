/*
*************************************************************************************
**文件说明：RTC配置
**文件名称：rtc_cfg.h 
**创建日期：2013.11.07  
**创 建 人：djs
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#ifndef __RCT_H__
#define __RCT_H__
#include "stm32f10x.h"
#include "user_type.h"







void   RTC_Config(void);
void   RTC_CurrentTimeInit(TimeDat * CurrentTime);
void   RTC_TimeAdjust(TimeDat * CurrentTime);

extern Uint32 RTC_TimeRegulate(TimeDat * CurrentTime);

extern void   RTC_Prepare(void);
extern void   RTC_GetTime(TimeDat * CurrentTime);



#endif

