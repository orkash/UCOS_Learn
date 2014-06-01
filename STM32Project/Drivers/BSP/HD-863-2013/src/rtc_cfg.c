/*
*************************************************************************************
**文件说明：RTC配置
**文件名称：rtc_cfg.c 
**创建日期：2013.11.07  
**创 建 人：djs
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#include "rtc_cfg.h"







/************************************************************************************
* 名    称：u8 Is_Leap_Year(u8 year)
* 功    能：闰年判断函数
* 入口参数：year  
* 出口参数：是闰年返回1  平年返回0
* 说    明：
************************************************************************************/
u8 Is_Leap_Year(u8 year)
{                     
   u16 year_value;
   year_value = year + 2000;
   if(year_value % 4 == 0) 	    //必须能被4整除
   {
   	  if(year_value % 100 == 0) 
      { 
         if(year_value % 400 == 0)	   //如果以00结尾,还要能被400整除 
		 	return 1;         
         else return 0;   
      }
	  else 
	  	return 1;   
   }
   else 
   	return 0; 
}


/************************************************************************************
* 名    称：void RTC_Configuration(void)
* 功    能：RTC功能配置函数
* 入口参数：无
* 出口参数：无
* 说    明：
************************************************************************************/
void RTC_Config(void)
{	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR ,ENABLE);
    PWR_BackupAccessCmd(ENABLE);				//使能RTC和后备寄存器访问	
	BKP_DeInit();
	RCC_LSEConfig(RCC_LSE_ON);					//开启外部32.768KHz晶振	
//	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待外部低速晶振稳定
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//RTC选择外部低速晶振作为其时钟源
	RCC_RTCCLKCmd(ENABLE);						//使能RTC时钟,这条指令过后LSE时钟接至RTC	
	RTC_WaitForSynchro();						//等待RTC时钟同步	
	RTC_WaitForLastTask();						//等待上一条RTC写指令执行完毕
//	RTC_ITConfig(RTC_IT_SEC,ENABLE);			//使能RTC秒中断
	RTC_WaitForLastTask();	
	RTC_SetPrescaler(32767);					//设置RTC分频系数，32.768KHz / 32768 = 1HZ，实现1S定时
	RTC_WaitForLastTask();
}

/************************************************************************************
* 名    称：void RTC_CurrentTimeInit(void)
* 功    能：RTC初始时间设定
* 入口参数：无
* 出口参数：无
* 说    明：上电检测备份区域密匙  如果没有初始化过时间则调用该函数 2013.04.16 22:30:00
************************************************************************************/
void RTC_CurrentTimeInit(TimeDat * CurrentTime)
{
	CurrentTime->Year = 13;	
	CurrentTime->Month = 7;
	CurrentTime->Day = 16;
	CurrentTime->Hour = 20;
	CurrentTime->Minute = 37;
	CurrentTime->Second = 0;
	RTC_TimeAdjust(CurrentTime);	
}

/************************************************************************************
* 名    称：void RTC_TimeAdjust(void)
* 功    能：RTC时间寄存器初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：默认到2099年
************************************************************************************/
void RTC_TimeAdjust(TimeDat * CurrentTime)
{
	if(CurrentTime->Year <= 99){
		RTC_WaitForLastTask();
		RTC_SetCounter(RTC_TimeRegulate(CurrentTime));
		RTC_WaitForLastTask();
	}
}

/************************************************************************************
* 名    称：u32 RTC_TimeRegulate(void)
* 功    能：时间转换函数
* 入口参数：无
* 出口参数：当前时间对应的秒计数
* 说    明：将当前的年月日时分秒 按照规则转换成对应的秒计数
************************************************************************************/
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
Uint32 RTC_TimeRegulate(TimeDat * CurrentTime)
{
	u8 i;
	u8 Val_year,Val_mon,Val_day,Val_hour,Val_min,Val_sec;
    u32 Sec_count = 0;
	Val_year = CurrentTime->Year;
	Val_mon  = CurrentTime->Month;
	Val_day  = CurrentTime->Day;
	Val_hour = CurrentTime->Hour;
	Val_min  = CurrentTime->Minute;
	Val_sec  = CurrentTime->Second;     
	for(i = 0;i < Val_year;i ++) 		//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(i))
			Sec_count += 31622400;		//闰年的秒钟数
		else 
			Sec_count += 31536000;      //平年的秒钟数
	}
	Val_mon -= 1;
	for(i = 0;i < Val_mon;i ++)         //把前面月份的秒钟数相加
	{
		Sec_count += (u32)mon_table[i] * 86400;//月份秒钟数相加
		if(Is_Leap_Year(Val_year) && i == 1)
			Sec_count += 86400;			//闰年2月份增加一天的秒钟数         
	}
	Sec_count += (u32)(Val_day - 1) * 86400;//把前面日期的秒钟数相加 
	Sec_count += (u32)Val_hour * 3600;//小时秒钟数
	Sec_count += (u32)Val_min * 60;      //分钟秒钟数	
	Sec_count += Val_sec;//最后的秒钟加上去
	return Sec_count;
}


/************************************************************************************
* 名    称：void RTC_Prepare(void)
* 功    能：RTC准备函数
* 入口参数：无  
* 出口参数：无
* 说    明：
************************************************************************************/
void RTC_Prepare(void)
{
    TimeDat  CurrentTime;
	if(BKP_ReadBackupRegister(BKP_DR1) != 0x1234){
		RTC_Config();
		RTC_CurrentTimeInit(&CurrentTime);
		BKP_WriteBackupRegister(BKP_DR1,0x1234);
	}
	else{
		RTC_WaitForSynchro();		  
		//RTC_ITConfig(RTC_IT_SEC,ENABLE);
		RTC_WaitForLastTask();
	}
}

/************************************************************************************
* 名    称：void RTC_GetTime(void)
* 功    能：RTC当前时间返回函数
* 入口参数：无  
* 出口参数：无
* 说    明：将当前时间保存至时间结构体
************************************************************************************/	
void RTC_GetTime(TimeDat * CurrentTime)
{
	u32 timecount = 0; 
	u32 temp = 0;
	u8  temp1,temp2; 	   
	timecount = RTC_GetCounter();
 	temp = timecount / 86400;   	//得到天数
	if(temp != 0)					//超过一天了
	{       
		temp1 = 0;      			//从2000年开始
		while(temp >= 365)
		{                          
			if(Is_Leap_Year(temp1)) //是闰年
			{
				if(temp >= 366)
					temp -= 366;
				else break;  
			}
			else 
				temp -= 365;       //平年 
			temp1 ++;  
		}   
		CurrentTime->Year = temp1;  	  //得到年份
		temp2 = 0;
		while(temp >= 28)         //超过了一个月的最小值
		{
			if(Is_Leap_Year(temp1) && temp2 == 1) //当年是闰年并且是2月份
			{
				if(temp >= 29)	
					temp -= 29;
				else break; 
			}
			else 
			{
				if(temp >= mon_table[temp2])
					temp -= mon_table[temp2];//平年
				else break;
			}
			 temp2 ++;  
		}
		temp2 = temp2 + 1;			 //得到月份
		CurrentTime->Month = temp2;  
		temp = temp + 1;				//得到日期
		CurrentTime->Day = temp;  
	}
	temp = timecount % 86400;     //得到秒钟数      
	temp2 = temp / 3600;     //小时
	CurrentTime->Hour = temp2;  
	temp2 = (temp % 3600) / 60; //分钟      
	CurrentTime->Minute= temp2;  
	temp2 = (temp % 3600) % 60; //秒钟
	CurrentTime->Second = temp2;    
}
