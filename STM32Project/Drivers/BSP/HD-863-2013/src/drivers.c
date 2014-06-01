/*
*************************************************************************************
**文件说明：驱动文件
**文件名称：sys_about.c
**创建日期：2013.11.06
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/


#define  DRIVERS_ENABLE

#include "user_cfg.h"
#include "fsmc_cfg.c"
#include "gpio_cfg.c"
#include "dma_cfg.c"
#include "rtc_cfg.c"
#include "lcd_tft.c"


#include "bsp_sdio_sdcard.c"

#include "num_operation.c"

#ifdef  USER_UCOSII
#include "os_lcd_tft.c"

#endif


