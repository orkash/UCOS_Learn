#ifndef __TFT_LCD_H__
#define __TFT_LCD_H__
#include "stm32f10x.h"


#define TFT_7inch_XSize  799
#define TFT_7inch_YSize	 479

extern u16 DestTop_Color_Buffer[14];
#define TFT_DateH GPIOB
#define TFT_DateL GPIOC
#define TFT_Control_Port GPIOA
#define TFT_CS GPIO_Pin_0
#define TFT_A0 GPIO_Pin_1		   //1:数据  0：指令
#define TFT_WR GPIO_Pin_2
#define TFT_RD GPIO_Pin_3
#define TFT_RST GPIO_Pin_8 


#define TFT_CS_Enable()        GPIO_ResetBits(TFT_Control_Port,TFT_CS)
#define TFT_CS_Disable()       GPIO_SetBits(TFT_Control_Port,TFT_CS)
#define TFT_ChooseDate()       GPIO_SetBits(TFT_Control_Port,TFT_A0)
#define TFT_ChooseCommand()    GPIO_ResetBits(TFT_Control_Port,TFT_A0)
#define TFT_Write_Enable()     GPIO_ResetBits(TFT_Control_Port,TFT_WR)
#define TFT_Write_Disable()    GPIO_SetBits(TFT_Control_Port,TFT_WR)
#define TFT_Read_Enalbe()      GPIO_ResetBits(TFT_Control_Port,TFT_RD)
#define TFT_Read_Disable()     GPIO_SetBits(TFT_Control_Port,TFT_RD)

/*--------16位颜色值---------------*/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XFF00
#define GRED 			 0XC618
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xB81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色


void TFT_GPIO_Configuration(void);
void TFT_Write_Date(u16 Dat);
void TFT_Write_Cmd(u16 Command);
void TFT_WriteRAM_Prepare(void);
void TFT_SetCursor(u16 Xpos, u16 Ypos);
void TFT_Clear(u16 Color);
void TFT_Init(void);
void TFT_Prepare(void);
void TFT_FILL_DestTop(u8 Fill_Block);
void TFT_SetPoint(u16 Point_x,u16 Point_y,u16 Point_Color);
void TFT_PutCharacter(u16 Char_PX,u16 Char_PY,u8 Put_Char,u8 Char_Size,u16 CharColor,u16 BkColor,u8 Cover);
void TFT_DisplayStringLine(u16 Str_PX,u16 Str_PY,u8 *Str_PTR,u8 Char_Size,u16 CharColor,u16 BkColor,u8 Cover);
void TFT_PutChineseLine(u16 Chinese_X, u16 Chinese_Y,u8 **ChineseLine_PTR,u8 Chinese_num,u8 Chinese_Size,u16 ChineseColor,u16 BkColor,u8 Cover);
void TFT_WriteBMP(u16 Xpos, u16 Ypos, u16 Height, u16 Width, u8 *bitmap);
void TFT_ShowBMP_Formflash(u16 size,u8 *bitmap);
void TFT_SetDisplayWindow(u16 XSta,u16 YSta,u16 XEnd,u16 YEnd);
void TFT_Fill(u16 XSta,u16 YSta,u16 XEnd,u16 YEnd,u16 Fill_Color);
void TFT_DrawLine(u16 XSta, u16 YSta, u16 XEnd, u16 YEnd, u16 Line_Color);
void TFT_DrawCircle(u16 Xptr, u16 Yptr, u8 Radiu, u16 Color);
void TFT_DrawRectangle(u16 XSta, u16 YSta, u16 XEnd, u16 YEnd, u16 Color);
void TFT_Show_Time(void);



#endif

