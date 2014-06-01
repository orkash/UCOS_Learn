/*
*************************************************************************************
**文件说明：配置文件
**文件名称： user_cfg.h 
**创建日期：2009.07.21  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#ifndef USER_CFG_H
#define USER_CFG_H

#include "user_type.h"
#include "stm32f10x.h"
#include "drivers.h"
#include "includes.h"
#include "memory.h"


/************************************************
**说明: OS启用
************************************************/
#define USER_UCOSII                         (1)




/************************************************
**说明: 版本号
************************************************/
#define VERSION                             (1.1)

/************************************************
**说明: 调试版本 
0为运行版本 
1为软件调试版本 
2为硬件调试版本
************************************************/
#define DEBUG                                 (0)



/************************************************
**说明: LCD背景色
************************************************/
#define LCD_BACKGROUND_COLOR              (GRAY)





#endif



