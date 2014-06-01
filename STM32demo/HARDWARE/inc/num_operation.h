/*
*************************************************************************************
**文件说明：数字操作
**文件名称： num_operation.h 
**创建日期：2013.11.15  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#ifndef NUM_OPERATION_H
#define NUM_OPERATION_H
#include "user_type.h"
#include "rtc_cfg.h"





/*
*************************************************************************************
* 名    称：void NUM_TimeDatToDisBuf(TimeDat * CurrentTime,NUM_DisBuf * DisDat)
* 功    能：rtc数据转换到BCD到显示缓存
* 入口参数：TimeDat * CurrentTime
            NUM_DisBuf * DisDat
* 出口参数：无
*************************************************************************************
*/
extern void NUM_TimeDatToDisBuf(TimeDat * CurrentTime,NUM_DisBuf * DisDat);

/*
*************************************************************************************
**函数名称：void NUM_CharHexToBCD(Uint8 dat,NUM_DisBuf * DisDat) 
**函数功能：将16进制数据拆分BCD码                
**入口参数：Uint8 dat 待拆分数据
**          Uint8 offset 在缓存中偏移量
            NUM_DisBuf * DisDat 显存指针
**返回参数：无                          
*************************************************************************************
*/
extern void NUM_CharHexToBCD(Uint8 dat,NUM_DisBuf * DisDat);

/*
*************************************************************************************
**函数名称：void NUM_ShortHexToBCD(Uint16 dat,NUM_DisBuf * DisDat) 
**函数功能：将16进制数据拆分BCD码                                   
**入口参数：Uint16 dat 待拆分数据										
**          Uint8 offset 在缓存中偏移量
            NUM_DisBuf * DisDat 显存指针
**返回参数：无                                                          
*************************************************************************************
*/
extern void NUM_ShortHexToBCD(Uint16 dat,NUM_DisBuf * DisDat);

/*
*************************************************************************************
**函数名称：void NUM_LongHexToBCD(Uint16 dat,NUM_DisBuf * DisDat) 
**函数功能：将16进制数据拆分BCD码                                         
**入口参数：Uint32 dat 待拆分数据											
**          Uint8 offset 在缓存中偏移量
            NUM_DisBuf * DisDat 显存指针
**返回参数：无                                                              
*************************************************************************************
*/
extern void NUM_LongHexToBCD(Uint32 dat,NUM_DisBuf * DisDat);

/*
*************************************************************************************
**函数名称：Uint32 NUM_GetBackDis(Uint8 n,NUM_DisBuf * DisDat)
**函数功能：从缓存中回读数值                                               
**入口参数：Uint8 n 回读个数												
**          Uint8 offset 缓存偏移量
            NUM_DisBuf * DisDat 显存指针
**返回参数：无                                                                   
*************************************************************************************
*/
extern Uint32 NUM_GetBackDis(Uint8 n,NUM_DisBuf * DisDat);

#endif


