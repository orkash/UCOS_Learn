/*
*************************************************************************************
**文件说明：数字操作
**文件名称： num_operation.c 
**创建日期：2013.11.15  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#include "num_operation.h"
#include "word.h"


void NUM_BCDToDisBuf(NUM_DisBuf * DisDat)
{
	for(DisDat->Index = 0;DisDat->Index < DisDat->DisN;DisDat->Index ++){
		DisDat->DisBuf[DisDat->Index] = Ascii[DisDat->NumSize][DisDat->BcdNum[DisDat->Index]];
	}
    for(DisDat->ZeroN = 0;DisDat->ZeroN < DisDat->DisN;DisDat->ZeroN ++){
        if(0x30 != DisDat->BcdNum[DisDat->ZeroN]){            //计算首位0个数
            break;
        }
    }       
}

/*
*************************************************************************************
* 名    称：void NUM_TimeDatToDisBuf(TimeDat * CurrentTime,NUM_DisBuf * DisDat)
* 功    能：rtc数据转换到BCD到显示缓存
* 入口参数：TimeDat * CurrentTime
            NUM_DisBuf * DisDat
* 出口参数：无
*************************************************************************************
*/
void NUM_TimeDatToDisBuf(TimeDat * CurrentTime,NUM_DisBuf * DisDat)
{
	DisDat->BcdNum[0]  = 2 | 0x30;
	DisDat->BcdNum[1]  = 0 | 0x30;
	DisDat->BcdNum[2]  = (CurrentTime->Year / 10)    | 0x30;
	DisDat->BcdNum[3]  = (CurrentTime->Year % 10)    | 0x30;
	DisDat->BcdNum[4]  = 0x2d;		//显示日期分隔符 -
	DisDat->BcdNum[5]  = (CurrentTime->Month  / 10)  | 0x30; 
	DisDat->BcdNum[6]  = (CurrentTime->Month  % 10)  | 0x30;
	DisDat->BcdNum[7]  = 0x2d;
	DisDat->BcdNum[8]  = (CurrentTime->Day / 10)    | 0x30;
	DisDat->BcdNum[9]  = (CurrentTime->Day % 10)    | 0x30;
	
    DisDat->BcdNum[10] = (CurrentTime->Hour / 10)   | 0x30;
	DisDat->BcdNum[11] = (CurrentTime->Hour % 10)   | 0x30;
	DisDat->BcdNum[12] = 0x3a;	    //显示时间分隔符 :
	DisDat->BcdNum[13] = (CurrentTime->Minute / 10) | 0x30; 
	DisDat->BcdNum[14] = (CurrentTime->Minute % 10) | 0x30;
	DisDat->BcdNum[15] = 0x3a;
	DisDat->BcdNum[16] = (CurrentTime->Second / 10) | 0x30;
	DisDat->BcdNum[17] = (CurrentTime->Second % 10) | 0x30;
    
    DisDat->NumSize = ASCII_8_16;
    DisDat->DisN    = 18;
    NUM_BCDToDisBuf(DisDat);
}

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
void NUM_CharHexToBCD(Uint8 dat,NUM_DisBuf * DisDat)
{
	DisDat->BcdNum[0 + DisDat->Offset] = (dat / 10) | 0x30;
	dat = dat % 10;
	DisDat->BcdNum[1 + DisDat->Offset] = dat | 0x30;
    
    DisDat->DisN = DisDat->Offset + 2;
    NUM_BCDToDisBuf(DisDat);
}

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
void NUM_ShortHexToBCD(Uint16 dat,NUM_DisBuf * DisDat)
{
	DisDat ->BcdNum[0 + DisDat->Offset] = (dat / 1000 ) | 0x30;
	dat = dat % 1000;
	DisDat ->BcdNum[1 + DisDat->Offset] = (dat / 100 ) | 0x30;
	dat = dat % 100;
	DisDat ->BcdNum[2 + DisDat->Offset] = (dat / 10 ) | 0x30;
	dat = dat % 10;
	DisDat ->BcdNum[3 + DisDat->Offset] = dat | 0x30;
    
    DisDat->DisN = DisDat->Offset + 4;
    NUM_BCDToDisBuf(DisDat);
}

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
void NUM_LongHexToBCD(Uint32 dat,NUM_DisBuf * DisDat)
{
	DisDat ->BcdNum[0 + DisDat->Offset] = (dat / 100000) | 0x30;
	dat = dat % 100000;
	DisDat ->BcdNum[1 + DisDat->Offset] = (dat / 10000) | 0x30;
	dat = dat % 10000;
	DisDat ->BcdNum[2 + DisDat->Offset] = (dat / 1000) | 0x30;
	dat = dat % 1000;
	DisDat ->BcdNum[3 + DisDat->Offset] = (dat / 100) | 0x30;
	dat = dat % 100;
	DisDat ->BcdNum[4 + DisDat->Offset] = (dat / 10) | 0x30;
	dat = dat % 10;
	DisDat ->BcdNum[5 + DisDat->Offset] = dat | 0x30;
    
    DisDat->DisN = DisDat->Offset + 6;
    NUM_BCDToDisBuf(DisDat);
}

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
Uint32 NUM_GetBackDis(Uint8 n,NUM_DisBuf * DisDat)
{
	Uint32 num;
	Uint8 i;
	for(i = 0,num = 0;i < n;i ++){
		num = num * 10 + (DisDat ->BcdNum[i + DisDat->Offset] & 0x0f);
	}
	return num;
}


















