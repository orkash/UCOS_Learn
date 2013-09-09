/*
*************************************************************************************
**文件说明：TFT液晶屏 使用FSMC与DMA通信传输控制函数
**文件名称：FSMC_TFT.C
**创建日期：2013.6.26
**创 建 人：段金松
**----------------------------------------------------------------------------------
**修改日期：2013.
**修改说明：
*************************************************************************************
*/


#include "fsmc_tft.h"
#include "drivers.h"

u16 DestTop_Color_Buffer[14] = {WHITE,BLACK,BLUE,BRED,GRED,GBLUE,RED,MAGENTA,GREEN,CYAN,YELLOW,BROWN,BRRED,GRAY};
/********************************************************************************************
*  名    称：void TFT_FSMC_GPIO_Configuration(void)
*  功    能：TFT屏幕IO口配置函数
*  入口参数：无
*  出口参数：无
*  说    明：
********************************************************************************************/
void TFT_FSMC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}
/********************************************************************************************
*  名    称：void TFT_FSMC_Configuration(void)
*  功    能：TFT屏幕 FSMC功能配置函数
*  入口参数：无
*  出口参数：无
*  说    明：
********************************************************************************************/
void TFT_FSMC_Configuration(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTimingInitStructure;
	
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_B;
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
/************************************************************************************
* 名    称：TFT_WriteRAM_Prepare(void)
* 功    能：写RAM命令
* 入口参数：无
* 出口参数：无
* 说    明：对CGRAM操作之前要先写该命令
************************************************************************************/
void TFT_WriteRAM_Prepare(void)
{
	TFT_Write_Cmd(0X2C);
}
/************************************************************************************
* 名    称：TFT_SetCursor(u16 Xpos,u16 Ypos)
* 功    能：TFT屏幕显示起始光标位置设定
* 入口参数：Xpos   X轴坐标位置
*         	Ypos   Y轴坐标位置
* 出口参数：无
* 说    明：
************************************************************************************/
void TFT_SetCursor(u16 Xpos, u16 Ypos)
{
	TFT_Write_Cmd(0X002A);	
	TFT_Write_Date(Xpos >> 8);	    		  //横向起始位置	 
	TFT_Write_Date(Xpos & 0X00FF);	    			  
	TFT_Write_Date(TFT_7inch_XSize >> 8);	      //横向终止位置
	TFT_Write_Date(TFT_7inch_XSize & 0X00FF);

    TFT_Write_Cmd(0X002B);	
	TFT_Write_Date(Ypos >> 8);	    			  //纵向起始位置
	TFT_Write_Date(Ypos & 0X00FF);
	TFT_Write_Date(TFT_7inch_YSize >> 8);	      //纵向终止位置
	TFT_Write_Date(TFT_7inch_YSize & 0X00FF);
}
/************************************************************************************
* 名    称：TFT_Clear(u16 Color)
* 功    能：TFT  清屏函数
* 入口参数：Color  清屏颜色
* 出口参数：无
* 说    明：将TFT屏幕刷新成特定颜色
************************************************************************************/
void TFT_Clear(u16 Color)
{ 	
	u32 index;
    
	TFT_SetCursor(0,0);
	TFT_WriteRAM_Prepare();	
	for(index = 0;index < 384000;index ++)
		TFT_Write_Date(Color);			
}
/************************************************************************************
* 名    称：TFT_Init(void)
* 功    能：TFT  屏初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：上电后对复位引脚的操作很关键   至少要有一个负脉冲
************************************************************************************/
void TFT_Init(void)
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	TFT_Write_Cmd(0x00E2);		
	TFT_Write_Date(0x0012);		//SSD1963外部晶振频率为10MHz,内核要求大于250MHz,因此36倍频最后得360MHz		   
	TFT_Write_Date(0x0000);		//设置PLL频率V = 2, PLL = 360 / (V + 1) = 120MHz								 
	TFT_Write_Date(0x0004);

	TFT_Write_Cmd(0x00E0);  	//PLL开始指令
	TFT_Write_Date(0x0001);  	//使能PLL
	delay_ms(1);
	TFT_Write_Cmd(0x00E0);	  	//PLL再次开始
	TFT_Write_Date(0x0003);		//该指令过后PLL输出作为系统时钟
	delay_ms(5);

	TFT_Write_Cmd(0x0001);   	//软件复位命令,没有参数
	delay_ms(5);
	TFT_Write_Cmd(0x00E6);	  	//设置数据移位速度
	TFT_Write_Date(0x0004);
	TFT_Write_Date(0x0093);
	TFT_Write_Date(0x00e0);

	TFT_Write_Cmd(0x00B0);	    //液晶屏工作模式设定
//================下边这指令，如果是7寸屏 用0X0000 5寸用0X0020
	TFT_Write_Date(0x0020);
	TFT_Write_Date(0x0000);		//TTL  mode
	TFT_Write_Date((TFT_7inch_XSize >> 8) & 0x00ff);  //设置横向宽度  
	TFT_Write_Date(TFT_7inch_XSize & 0X00FF);
    TFT_Write_Date((TFT_7inch_YSize >> 8) & 0x00ff);  //设置纵向宽度 
	TFT_Write_Date(TFT_7inch_YSize & 0X00FF); 
    TFT_Write_Date(0x0000);		//RGB序列

	TFT_Write_Cmd(0x00B4);	           
	TFT_Write_Date((1000 >> 8) & 0X00FF);  
	TFT_Write_Date(1000 & 0X00FF);
	TFT_Write_Date((51 >> 8) & 0X00FF);  
	TFT_Write_Date(51 & 0X00FF);
	TFT_Write_Date(8);			   
	TFT_Write_Date((3 >> 8) & 0X00FF);  
	TFT_Write_Date(3 & 0X00FF);  
	TFT_Write_Date(0x0000);

	TFT_Write_Cmd(0x00B6);	           
	TFT_Write_Date((530 >> 8) & 0X00FF);   
	TFT_Write_Date(530 & 0X00FF);
	TFT_Write_Date((24 >> 8) & 0X00FF);  
	TFT_Write_Date(24 & 0X00FF);
	TFT_Write_Date(3);			   
	TFT_Write_Date((23 >> 8) & 0X00FF);  
	TFT_Write_Date(23 & 0X00FF);  

	TFT_Write_Cmd(0x00BA);
	TFT_Write_Date(0x0005);           

	TFT_Write_Cmd(0x00B8);
	TFT_Write_Date(0x0007);    
	TFT_Write_Date(0x0001);    

	TFT_Write_Cmd(0x0036);       
	TFT_Write_Date(0x0000);

	TFT_Write_Cmd(0x00F0);    
	TFT_Write_Date(0x0003);
	delay_ms(5);
	TFT_Write_Cmd(0x0029);        

	TFT_Write_Cmd(0x00BE);        
	TFT_Write_Date(0x0006);
	TFT_Write_Date(0x0080);
	
	TFT_Write_Date(0x0001);
	TFT_Write_Date(0x00f0);
	TFT_Write_Date(0x0000);
	TFT_Write_Date(0x0000);

	TFT_Write_Cmd(0x00d0);//设置动态背光控制配置 
	TFT_Write_Date(0x000d);
}
void TFT_Prepare(void)
{
	TFT_FSMC_GPIO_Configuration();
	TFT_FSMC_Configuration();
	TFT_Init();
}
/************************************************************************************
* 名    称：TFT_FILL_DestTop(u8 Fill_Block)
* 功    能：TFT屏幕分块填充颜色
* 入口参数：Fill_Block  分块数
* 出口参数：无
* 说    明：根据Fill_Block的大小 将整个屏幕分成若干块 分别填充不同的颜色
************************************************************************************/
void TFT_FILL_DestTop(u8 Fill_Block)
{
	u8 value = 0;
	u16 i;
	u32	Fill_index = 0, j;
	value = 480 / Fill_Block;
	Fill_index = 800 * value;
	TFT_SetCursor(0,0);
	TFT_WriteRAM_Prepare();
	for(i = 0;i < Fill_Block;i ++)					
		for(j = 0;j < Fill_index;j ++)
			TFT_Write_Date(DestTop_Color_Buffer[i + 3]);
}
/************************************************************************************
* 名    称：TFT_SetPoint(u16 Point_x,u16 Point_y,u16 Point_Color)
* 功    能：TFT屏幕指定位置上显示一个特定颜色的点
* 入口参数：Point_x  		点X轴坐标
*           Point_y			点Y轴坐标
*			Point_Color     点颜色
* 出口参数：无
* 说    明：
************************************************************************************/
void TFT_SetPoint(u16 Point_x,u16 Point_y,u16 Point_Color)
{
  if ((Point_x >= 800) || (Point_y >= 480)) 
  	return;
  TFT_SetCursor(Point_x,Point_y);
  TFT_WriteRAM_Prepare();
  TFT_Write_Date(Point_Color);
}
/****************************************************************************************************
*  名    称：void TFT_DrawBigPoint(u16 Point_x,u16 Point_y,u16 Point_Color)
*  功    能：在屏幕上指定位置画一个图形点
*  入口参数：Point_x    		横轴坐标
*			 			 Point_y    		纵轴坐标
*			 			 Point_Color  		图形颜色
*  出口参数：无
*  说    明：
****************************************************************************************************/
void TFT_DrawBigPoint(u16 Point_x,u16 Point_y,u16 Point_Color)
{
	TFT_DrawLine(Point_x - 10,Point_y, Point_x + 10,Point_y, Point_Color);
	TFT_DrawLine(Point_x, Point_y - 10, Point_x, Point_y + 10, Point_Color);
	TFT_SetPoint(Point_x + 1, Point_y + 1, Point_Color);
	TFT_SetPoint(Point_x - 1, Point_y + 1, Point_Color);
	TFT_SetPoint(Point_x + 1, Point_y - 1, Point_Color);
	TFT_SetPoint(Point_x - 1, Point_y - 1, Point_Color);
	TFT_DrawCircle(Point_x, Point_y, 6, Point_Color);
}	
/************************************************************************************
* 名    称：TFT_PutCharacter(u16 Char_PX,u16 Char_PY,u8 Put_Char,u8 Char_Size,u16 CharColor,u16 BkColor,u8 Cover)
* 功    能：TFT屏幕上指定位置开始 显示任意大小格式的字符
* 入口参数：Char_PX  		X轴起始坐标
*           Char_PY			Y轴起始坐标
*						Put_Char        需要显示的字符
* 					Char_Size		字符大小   Character_16_24     Character_16_32
*						CharColor       字符颜色
*						BkColor			背景颜色
*						Cover           背景色是否显示  1：覆盖原背景色  0：不覆盖原背景色
* 出口参数：无
* 说    明：
************************************************************************************/
void TFT_PutCharacter(u16 Char_PX,u16 Char_PY,u8 Put_Char,u8 Char_Size,u16 CharColor,u16 BkColor,u8 Cover)
{
	u8 i,j,k;
  	u8 hor_size,ver_size;									//hor_size：横向宽度  ver_size：纵向宽度
	u8 *Character_addr;
												//先将显示数据保存至数组内，之后调用填充图片函数(保证刷新速度)
	Character_addr = (u8 *)ASCII[Char_Size][Put_Char];		//保存字符ASCII数据起始地址
  	switch(Char_Size)
	{
		case 0:hor_size = 16;ver_size = 24;break;			//16 * 24
		case 1:hor_size = 16;ver_size = 32;break;			//16 * 32
		case 2:hor_size = 32;ver_size = 48;break;			//32 * 48
		case 3:hor_size = 8;ver_size = 16;break;
		default:break;
	}
  	k = hor_size >> 3;										//计算字长
	for(i = 0;i < ver_size;i ++)
	{
		for(j = 0;j < hor_size;j ++)
		{
			if((Character_addr[i * k + j / 8] << (j % 8)) & 0x80)  //非零表示显示字符颜色
//				TFT_SetPoint(Char_PX + j,Char_PY + i,CharColor);
				temp[i * hor_size + j] = CharColor;
				
			else if(Cover)										   //零显示背景颜色  
//				TFT_SetPoint(Char_PX + j,Char_PY + i,BkColor);
				temp[i * hor_size + j] = BkColor;
		}
	}
	TFT_Fill_BMP_16(Char_PX,Char_PY,hor_size,ver_size,temp);
}
/************************************************************************************
* 名    称：TFT_DisplayStringLine(u16 Str_PX,u16 Str_PY,u8 *Str_PTR,u8 Char_Size,u16 CharColor,u16 BkColor)
* 功    能：TFT屏幕上指定位置开始 显示任意大小格式的一串字符
* 入口参数：Str_PX  		X轴起始坐标
*           Str_PY			Y轴起始坐标
*						*Str_PTR        字符数组
* 					Char_Size		字符大小	 Character_16_24     Character_16_32  Character_32_48
*						CharColor       字符颜色
*						BkColor			背景颜色
*						Cover           背景色是否显示  1：覆盖原背景色  0：不覆盖原背景色
* 出口参数：无
* 说    明：
************************************************************************************/
void TFT_DisplayStringLine(u16 Str_PX,u16 Str_PY,u8 *Str_PTR,u8 Char_Size,u16 CharColor,u16 BkColor,u8 Cover)
{
  u8 i = 0;
  u16 Column_value;
  u8 Character_offset;
  Column_value = Str_PX;
  switch(Char_Size)
  {
  	case 0:Character_offset = 16;break;
	case 1:Character_offset = 16;break;
	case 2:Character_offset = 32;break;
	case 3:Character_offset = 8;break;
	default:break;
  }
  while((*Str_PTR != '\0'))	 
  {
	TFT_PutCharacter(Column_value, Str_PY,*Str_PTR,Char_Size,CharColor,BkColor,Cover);
    Column_value += Character_offset;
    Str_PTR ++;
    i ++;
  }
}
/************************************************************************************
* 名    称：TFT_PutChineseLine(u16 Chinese_X, u16 Chinese_Y,u8 **ChineseLine_PTR,u8 Chinese_num,u8 Chinese_Size,u16 ChineseColor,u16 BkColor,u8 Cover)
* 功    能：TFT屏幕上指定位置开始 显示任意大小格式的一串汉字
* 入口参数：Chinese_X  				X轴起始坐标
*           Chinese_Y				Y轴起始坐标
*						**ChineseLine_PTR       汉字数组
*						Chinese_num				显示汉字个数
* 					Chinese_Size			汉字大小	 Chinese_32_32     Chinese_48_48
*						ChineseColor       		汉字颜色
*						BkColor					背景颜色
*						Cover           		背景色是否显示  1：覆盖原背景色  0：不覆盖原背景色
* 出口参数：无
* 说    明：
************************************************************************************/
void TFT_PutChineseLine(u16 Chinese_X, u16 Chinese_Y,u8 **ChineseLine_PTR,u8 Chinese_num,u8 Chinese_Size,u16 ChineseColor,u16 BkColor,u8 Cover)
{
    u8 i,j,k,l;
  	u8 hor_size,ver_size;									//hor_size：横向宽度  ver_size：纵向宽度
  	switch(Chinese_Size)
	{
		case 0:hor_size = 24;ver_size = 24;break;			//32 * 32
		case 1:hor_size = 48;ver_size = 48;break;			//48 * 48
		default:break;
	}
  	k = hor_size >> 3;										//计算字长
	for(l = 0;l < Chinese_num;l ++)
	{
		for(i = 0;i < ver_size;i ++)
		{
			for(j = 0;j < hor_size;j ++)
			{
				if((ChineseLine_PTR[l][i * k + j / 8] << (j % 8)) & 0x80)  //非零表示显示字符颜色
					TFT_SetPoint(Chinese_X + j + l * hor_size,Chinese_Y + i,ChineseColor);
//					temp[i * hor_size + j] = ChineseColor;
				else if(Cover)										       //零显示背景颜色  
					TFT_SetPoint(Chinese_X + j + l * hor_size,Chinese_Y + i,BkColor);
//					temp[i * hor_size + j] = BkColor;
			}
		}
//		TFT_Fill_BMP_16(Chinese_X + l * hor_size,Chinese_Y,hor_size,ver_size,temp);
	}
}
/************************************************************************************
* 名    称：void TFT_WriteBMP(u8 Xpos, u16 Ypos, u8 Height, u16 Width, u8 *bitmap)
* 功    能：TFT屏幕上显示图片
* 入口参数：Xpos        X轴起始位置
*						Ypos		Y轴起始位置
*           Height		X轴增量
*						Width		Y轴增量
*						*bitmap		图片数组起始位置
* 出口参数：无
* 说    明：此函数中将指针bitmap强制转换成16进制，因此每次取*bitmap_ptr时实际
*			取出的是一个16位数据,符合RGB标准
************************************************************************************/
void TFT_WriteBMP(u16 Xpos, u16 Ypos, u16 Width, u16 Height,u8 *bitmap)
{
	u32 index;
	u32 size = Height * Width;
	u16 *bitmap_ptr = (u16 *)bitmap;
	
	TFT_SetDisplayWindow(Xpos,Ypos,Xpos + Width,Ypos + Height);
	TFT_WriteRAM_Prepare(); 
	for(index = 0; index < size; index ++)		 
		TFT_Write_Date(*bitmap_ptr ++);
	TFT_SetDisplayWindow(0,0,799,479);	  
}
/************************************************************************************
* 名    称：TFT_ShowBMP_Formflash(u16 size,u8 *bitmap)
* 功    能：TFT屏幕上显示图片  数据从片外Flash(W25Q16)中读取
* 入口参数：size  		一次显示数据个数
*						*bitmap  	图片数组地址
* 出口参数：无
* 说    明：调用此函数之前 要设置光标位置TFT_SetCursor  写GRAM使能TFT_WriteRAM_Prepare
************************************************************************************/
void TFT_ShowBMP_Formflash(u32 size,u8 *bitmap)
{
//	u32 index;
	u16 *bitmap_ptr = (u16 *)bitmap;
	 
//	for(index = 0; index < size; index ++)		 
//		TFT_Write_Date(*bitmap_ptr ++);	
	DMA_FSMC_Configuration(bitmap_ptr,size);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == 0);
	DMA_Cmd(DMA1_Channel1,DISABLE);  
}
/************************************************************************************
* 名    称：TFT_SetDisplayWindow(u8 Xpos, u16 Ypos, u8 Height, u16 Width)
* 功    能：设定TFT屏幕上填充颜色的区域范围
* 入口参数：XSta        横向起始位置
*						YSta		纵向起始位置
*           XEnd		横向结束位置
*						YEnd		纵向结束位置	
* 出口参数：无
* 说    明：
************************************************************************************/
void TFT_SetDisplayWindow(u16 XSta,u16 YSta,u16 XEnd,u16 YEnd)
{
	if(XEnd >= 800 || YEnd >= 480)
		return;
	TFT_Write_Cmd(0X002A);	
	TFT_Write_Date(XSta >> 8);	    			  //横向起始位置
	TFT_Write_Date(XSta & 0X00FF);
	TFT_Write_Date(XEnd >> 8);	      			  //横向终止位置
	TFT_Write_Date(XEnd & 0X00FF);

    TFT_Write_Cmd(0X002B);	
	TFT_Write_Date(YSta >> 8);	    			  //纵向起始位置
	TFT_Write_Date(YSta & 0X00FF);
	TFT_Write_Date(YEnd >> 8);	      			  //纵向终止位置
	TFT_Write_Date(YEnd & 0X00FF);
}
/************************************************************************************
* 名    称：TFT_Fill(u16 XSta,u16 YSta,u16 Xincrease,u16 Yincrease,u16 Fill_Color)
* 功    能：在TFT屏幕上指定区域内填充指定颜色
* 入口参数：XSta        	横向起始位置
*						YSta			纵向起始位置
*           Xincrease		横向增量
*						Yincrease		纵向增量
*						Fill_Color  	填充的颜色	
* 出口参数：无
* 说    明：
************************************************************************************/
void TFT_Fill(u16 XSta,u16 YSta,u16 Xincrease,u16 Yincrease,u16 Fill_Color)
{                    
    u32 num;
	
	TFT_SetDisplayWindow(XSta,YSta,XSta + Xincrease - 1,YSta + Yincrease - 1); 
	TFT_WriteRAM_Prepare();         	 	   	   
	num = (u32)(Xincrease * Yincrease);    
//	DMA_FSMC_ConfigurationAlone((u32)Fill_Color,num);
//	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == 0);
//	DMA_Cmd(DMA1_Channel1,DISABLE);
	while(num --)
		TFT_Write_Date(Fill_Color);	 
}
/************************************************************************************
* 名    称：TFT_Fill_BMP(u16 XSta,u16 YSta,u16 Xincrease,u16 Yincrease,u8 *Fill_BMP_STR)
* 功    能：在TFT屏幕上指定区域内填充指定颜色
* 入口参数：XSta        	横向起始位置
*						YSta			纵向起始位置
*           Xincrease		图片横向宽度
*						Yincrease		图片纵向高度
*						Fill_BMP_STR    填充图片数据的首地址  数据格式为U8	
* 出口参数：无
* 说    明：先将u8类型的图片数组转换成u16类型的数组，这样每次读取数组内容时是u16类型的
*			使用DMA方式向FSMC发送数据
************************************************************************************/
void TFT_Fill_BMP(u16 XSta,u16 YSta,u16 Xincrease,u16 Yincrease,u8 *Fill_BMP_STR)
{                    
    u32 num;
    u16 *value = (u16 *)Fill_BMP_STR;
	TFT_SetDisplayWindow(XSta,YSta,(XSta + Xincrease - 1),(YSta + Yincrease - 1)); 
	TFT_WriteRAM_Prepare();         	 	   	   
	num = (u32)(Xincrease * Yincrease);    
//	while(num --)
//		TFT_Write_Date(*value ++);
	DMA_FSMC_Configuration(value,num);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == 0);
	DMA_Cmd(DMA1_Channel1,DISABLE); 
}
/************************************************************************************
* 名    称：TFT_Fill_BMP(u16 XSta,u16 YSta,u16 Xincrease,u16 Yincrease,u8 *Fill_BMP_STR)
* 功    能：在TFT屏幕上指定区域内填充指定颜色
* 入口参数：XSta        	横向起始位置
*						YSta			纵向起始位置
*           Xincrease		图片横向宽度
*						Yincrease		图片纵向高度
*						Fill_BMP_STR    填充图片数据的首地址  数据格式为u16	
* 出口参数：无
* 说    明：使用DMA方式向FSMC发送数据
************************************************************************************/
void TFT_Fill_BMP_16(u16 XSta,u16 YSta,u16 Xincrease,u16 Yincrease,u16 *Fill_BMP_STR)
{                    
    u32 num;
	TFT_SetDisplayWindow(XSta,YSta,(XSta + Xincrease - 1),(YSta + Yincrease - 1)); 
	TFT_WriteRAM_Prepare();         	 	   	   
	num = (u32)(Xincrease * Yincrease);    
	DMA_FSMC_Configuration(Fill_BMP_STR,num);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == 0);
	DMA_Cmd(DMA1_Channel1,DISABLE);	 
}
/************************************************************************************
* 名    称：TFT_DrawLine(u16 XSta, u16 YSta, u16 XEnd, u16 YEnd, u16 Line_Color)
* 功    能：在TFT屏幕上任意两点之间画一条直线
* 入口参数：XSta        横向起始位置
*						YSta		纵向起始位置
*           XEnd		横向结束位置
*						YEnd		纵向结束位置
*						Line_Color  线的颜色	
* 出口参数：无
* 说    明：如果一条直线感觉不够明显，可以在起点和终点的横轴方向上增加1格再画几条直线
************************************************************************************/
void TFT_DrawLine(u16 XSta, u16 YSta, u16 XEnd, u16 YEnd, u16 Line_Color)
{
    u16 i,j,temp;	 
	if((XSta == XEnd)&&(YSta == YEnd))				  //起始位置 = 终止位置
		TFT_SetPoint(XSta, YSta, Line_Color);
	else
	{ 
		if(fabs(YEnd - YSta) > fabs(XEnd - XSta))		  //斜率大于1 
		{
			if(YSta > YEnd) 
			{
				temp = YSta;						  //交换起点/终点坐标
				YSta = YEnd;
				YEnd = temp; 
				temp = XSta;
				XSta = XEnd;
				XEnd = temp; 
			}
			for(j = YSta;j < YEnd;j ++)               //以y轴为基准
			{
				i = (u16)(j - YSta) * (XEnd - XSta) / (YEnd - YSta) + XSta;
				TFT_SetPoint(i,j,Line_Color);  
			}
		}
		else     									 //斜率小于等于1
		{
			if(XSta > XEnd)
			{
				temp = YSta;
				YSta = YEnd;
				YEnd = temp;
				temp = XSta;
				XSta = XEnd;
				XEnd = temp;
			}   
			for(i = XSta;i <= XEnd;i ++)  			//以x轴为基准 
			{
				j = (u16)(i - XSta) * (YEnd - YSta) / (XEnd - XSta) + YSta;
				TFT_SetPoint(i,j,Line_Color); 
			}
		}
	} 
}
/************************************************************************************
* 名    称：TFT_DrawCircle(u16 Xptr, u16 Yptr, u8 Radiu, u16 Color)
* 功    能：以TFT屏幕上任意一点为圆心 画任意半径的圆
* 入口参数：Xptr        横向位置
*						Yptr		纵向位置
*						Radiu		圆的半径
*						Color       画圆颜色
* 出口参数：无
* 说    明：如果感觉圆线不够明显，可以在半径上增加1，多画几个圆
************************************************************************************/
void TFT_DrawCircle(u16 Xptr, u16 Yptr, u8 Radiu, u16 Color)
{
	int a,b;
	int di;

	if((Xptr + Radiu) > TFT_7inch_XSize || (Yptr + Radiu) > TFT_7inch_YSize)
		return;
	a = 0;
	b = Radiu;		  
	di = 3 - (Radiu << 1);             				//判断下个点位置的标志
	while(a <= b)
	{
		TFT_SetPoint(Xptr - b, Yptr - a, Color);             //3           
		TFT_SetPoint(Xptr + b, Yptr - a, Color);             //0           
		TFT_SetPoint(Xptr - a, Yptr + b, Color);             //1       
		TFT_SetPoint(Xptr - b, Yptr - a, Color);             //7           
		TFT_SetPoint(Xptr - a, Yptr - b, Color);             //2             
		TFT_SetPoint(Xptr + b, Yptr + a, Color);             //4               
		TFT_SetPoint(Xptr + a, Yptr - b, Color);             //5
		TFT_SetPoint(Xptr + a, Yptr + b, Color);             //6 
		TFT_SetPoint(Xptr - b, Yptr + a, Color); 
		a ++;		     
		if(di < 0)									  //使用Bresenham算法画圆
			di += 4 * a + 6;	  					  
		else
		{
			di += 10 + 4 * (a - b);   
			b --;
		} 
		TFT_SetPoint(Xptr + a, Yptr + b, Color);
	}
}
/************************************************************************************
* 名    称：void TFT_DrawRectangle(u16 XSta, u16 YSta, u16 Xincrease, u16 Yincrease, u16 Color)
* 功    能：以TFT屏幕上任意两点之间画一个长方形
* 入口参数：XSta        		横向起始位置
*						YSta				纵向起始位置
*						Xincrease			横向增量
*						Yincrease        	纵向增量
*						Color       		长方形颜色
* 出口参数：无
* 说    明：如果感觉长方形颜色不够明显
*			X轴起始位置-1，Y轴起始位置-1
*			X轴结束位置+1，Y轴结束位置+1		 再画长方形
************************************************************************************/
void TFT_DrawRectangle(u16 XSta, u16 YSta, u16 Xincrease, u16 Yincrease, u16 Color)
{
	TFT_DrawLine(XSta, YSta, (XSta + Xincrease), YSta, Color);
	TFT_DrawLine(XSta, YSta, XSta, (YSta + Yincrease), Color);
	TFT_DrawLine(XSta, (YSta + Yincrease), (XSta + Xincrease), (YSta + Yincrease), Color);
	TFT_DrawLine((XSta + Xincrease), YSta, (XSta + Xincrease), (YSta + Yincrease), Color);
} 	
/******************************************************************************************
* 名    称：void TFT_Show_Time(void)
* 功    能：TFT屏幕上显示当前时间
* 入口参数：无
* 出口参数：无
* 说    明：格式20XX-XX-XX  XX:XX:XX
*******************************************************************************************/
void TFT_Show_Time(void)
{
	
	u8 TimeString[9];
	TimeString[0] = CurrentTime.Year / 10 + '0';
	TimeString[1] = CurrentTime.Year % 10 + '0';
	TimeString[2] = '-';
	TimeString[3] = CurrentTime.Month / 10 + '0';
	TimeString[4] = CurrentTime.Month % 10 + '0';
	TimeString[5] = '-';
	TimeString[6] = CurrentTime.Day / 10 + '0';
	TimeString[7] = CurrentTime.Day % 10 + '0';
	TimeString[8] = '\0';
	TFT_DisplayStringLine(640,30,TimeString,Character_16_24,BLUE,BACKGROND,1);

	TimeString[0] = CurrentTime.Hour / 10 + '0';
	TimeString[1] = CurrentTime.Hour % 10 + '0';
	TimeString[2] = ':';
	TimeString[3] = CurrentTime.Minute / 10 + '0';
	TimeString[4] = CurrentTime.Minute % 10 + '0';
	TimeString[5] = ':';
	TimeString[6] = CurrentTime.Second / 10 + '0';
	TimeString[7] = CurrentTime.Second % 10 + '0';
	TimeString[8] = '\0';
	TFT_DisplayStringLine(640,55,TimeString,Character_16_24,BLUE,BACKGROND,1);	  //画笔颜色红
}
