#include "tft_lcd.h"
#include "systick.h"
#include "word.h"
#include "rtc.h"
#include "math.h"


u16 DestTop_Color_Buffer[14] = {WHITE,BLACK,BLUE,BRED,GRED,GBLUE,RED,MAGENTA,GREEN,CYAN,YELLOW,BROWN,BRRED,GRAY};
/************************************************************************************
* ��    �ƣ�TFT_GPIO_Configuration(void)
* ��    �ܣ�TFT��Ļ��IO�����ú���
* ��ڲ�������
* ���ڲ�������
* ˵    �����ڴ˺�������ABC�ڵ�ʱ�� ��������TFT����CS A0 WR RD RST��ʼֵΪ�ߵ�ƽ
*			16��������   D0~D7:PC0~PC7		   D8~D15:PB8~PB15
*			CS           PA0
*			RS/A0        PA1
*			WR           PA2
*			RD			 PA3
*			RST 		 PA8
************************************************************************************/
void TFT_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = TFT_CS | TFT_A0 | TFT_WR | TFT_RD | TFT_RST;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(TFT_Control_Port,&GPIO_InitStructure);
	GPIO_SetBits(TFT_Control_Port,TFT_CS);
    GPIO_SetBits(TFT_Control_Port,TFT_A0);
	GPIO_SetBits(TFT_Control_Port,TFT_WR);
	GPIO_SetBits(TFT_Control_Port,TFT_RD);
	GPIO_SetBits(TFT_Control_Port,TFT_RST);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(TFT_DateH,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(TFT_DateL,&GPIO_InitStructure);
}
/************************************************************************************
* ��    �ƣ�TFT_Write_Date(u16 Dat)
* ��    �ܣ�TFT��Ļд����
* ��ڲ�����Dat  ��Ҫд�������
* ���ڲ�������
* ˵    ����ע��A0
************************************************************************************/
void TFT_Write_Date(u16 Dat)
{
	TFT_CS_Enable();
	TFT_ChooseDate();
	TFT_Read_Disable();
	GPIO_Write(TFT_DateL,(Dat & 0X00FF));
	GPIO_Write(TFT_DateH,(Dat & 0XFF00));
	TFT_Write_Enable();
	TFT_Write_Disable();
	TFT_CS_Disable();
}
/************************************************************************************
* ��    �ƣ�TFT_Write_Cmd(u16 Command)
* ��    �ܣ�TFT��Ļдָ��
* ��ڲ�����Command  ��Ҫд���ָ��
* ���ڲ�������
* ˵    ����ע��A0
************************************************************************************/
void TFT_Write_Cmd(u16 Command)
{
	TFT_CS_Enable();
	TFT_ChooseCommand();
	TFT_Read_Disable();
	GPIO_Write(TFT_DateL,(Command & 0X00FF));
	GPIO_Write(TFT_DateH,(Command & 0XFF00));
	TFT_Write_Enable();
	TFT_Write_Disable();
	TFT_CS_Disable();
}
/************************************************************************************
* ��    �ƣ�TFT_WriteRAM_Prepare(void)
* ��    �ܣ�дRAM����
* ��ڲ�������
* ���ڲ�������
* ˵    ������CGRAM����֮ǰҪ��д������
************************************************************************************/
void TFT_WriteRAM_Prepare(void)
{
	TFT_Write_Cmd(0X2C);
}

/************************************************************************************
* ��    �ƣ�TFT_SetCursor(u16 Xpos,u16 Ypos)
* ��    �ܣ�TFT��Ļ��ʾ��ʼ���λ���趨
* ��ڲ�����Xpos   X������λ��
*         	Ypos   Y������λ��
* ���ڲ�������
* ˵    ����
************************************************************************************/
void TFT_SetCursor(u16 Xpos, u16 Ypos)
{
	TFT_Write_Cmd(0X002A);	
	TFT_Write_Date(Xpos >> 8);	    			  //������ʼλ��
	TFT_Write_Date(Xpos & 0X00FF);
	TFT_Write_Date(TFT_7inch_XSize >> 8);	      //������ֹλ��
	TFT_Write_Date(TFT_7inch_XSize & 0X00FF);

    TFT_Write_Cmd(0X002B);	
	TFT_Write_Date(Ypos >> 8);	    			  //������ʼλ��
	TFT_Write_Date(Ypos & 0X00FF);
	TFT_Write_Date(TFT_7inch_YSize >> 8);	      //������ֹλ��
	TFT_Write_Date(TFT_7inch_YSize & 0X00FF);
}
/************************************************************************************
* ��    �ƣ�TFT_Clear(u16 Color)
* ��    �ܣ�TFT  ��������
* ��ڲ�����Color  ������ɫ
* ���ڲ�������
* ˵    ������TFT��Ļˢ�³��ض���ɫ
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
* ��    �ƣ�TFT_Init(void)
* ��    �ܣ�TFT  ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    �����ϵ��Ը�λ���ŵĲ����ܹؼ�   ����Ҫ��һ��������
************************************************************************************/
void TFT_Init(void)
{
	GPIO_ResetBits(TFT_Control_Port,TFT_RST);
	GPIO_SetBits(TFT_Control_Port,TFT_RST);
	TFT_Write_Cmd(0x00E2);		
	TFT_Write_Date(0x0023);		//SSD1963�ⲿ����Ƶ��Ϊ10MHz,�ں�Ҫ�����250MHz,���36��Ƶ����360MHz		   
	TFT_Write_Date(0x0002);		//����PLLƵ��V = 2, PLL = 360 / (V + 1) = 120MHz								 
	TFT_Write_Date(0x0004);

	TFT_Write_Cmd(0x00E0);  	//PLL��ʼָ��
	TFT_Write_Date(0x0001);  	//ʹ��PLL
	TFT_Write_Cmd(0x00E0);	  	//PLL�ٴο�ʼ
	TFT_Write_Date(0x0003);		//��ָ�����PLL�����Ϊϵͳʱ��


	TFT_Write_Cmd(0x0001);   	//�����λ����,û�в���

	TFT_Write_Cmd(0x00E6);	  	//����������λ�ٶ�
	TFT_Write_Date(0x0004);
	TFT_Write_Date(0x0093);
	TFT_Write_Date(0x00e0);

	TFT_Write_Cmd(0x00B0);	    //Һ��������ģʽ�趨
//================�±���ָ������7���� ��0X0000 5����0X0020
	TFT_Write_Date(0x0020);
	TFT_Write_Date(0x0000);		//TTL  mode
	TFT_Write_Date(TFT_7inch_XSize >> 8);  //���ú�����  
	TFT_Write_Date(TFT_7inch_XSize & 0X00FF);
    TFT_Write_Date(TFT_7inch_YSize >> 8);  //���������� 
	TFT_Write_Date(TFT_7inch_YSize & 0X00FF);
    TFT_Write_Date(0x0000);		//RGB����

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


	TFT_Write_Cmd(0x0029);        

	TFT_Write_Cmd(0x00BE);        
	TFT_Write_Date(0x0006);
	TFT_Write_Date(0x0080);
	
	TFT_Write_Date(0x0001);
	TFT_Write_Date(0x00f0);
	TFT_Write_Date(0x0000);
	TFT_Write_Date(0x0000);

	TFT_Write_Cmd(0x00d0);//���ö�̬����������� 
	TFT_Write_Date(0x000d);
}
/******************************************************************************************
* ��    �ƣ�TFT_Prepare(void)
* ��    �ܣ�TFT��Ļ׼��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
*******************************************************************************************/
void TFT_Prepare(void)
{
	TFT_GPIO_Configuration();
	TFT_Init();
}

/************************************************************************************
* ��    �ƣ�TFT_FILL_DestTop(u8 Fill_Block)
* ��    �ܣ�TFT��Ļ�ֿ������ɫ
* ��ڲ�����Fill_Block  �ֿ���
* ���ڲ�������
* ˵    ��������Fill_Block�Ĵ�С ��������Ļ�ֳ����ɿ� �ֱ���䲻ͬ����ɫ
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
* ��    �ƣ�TFT_SetPoint(u16 Point_x,u16 Point_y,u16 Point_Color)
* ��    �ܣ�TFT��Ļָ��λ������ʾһ���ض���ɫ�ĵ�
* ��ڲ�����Point_x  		��X������
*           Point_y			��Y������
*			Point_Color     ����ɫ
* ���ڲ�������
* ˵    ����
************************************************************************************/
void TFT_SetPoint(u16 Point_x,u16 Point_y,u16 Point_Color)
{
  if ((Point_x >= 800) || (Point_y >= 480)) 
  	return;
  TFT_SetCursor(Point_x,Point_y);
  TFT_WriteRAM_Prepare();
  TFT_Write_Date(Point_Color);
}
/************************************************************************************
* ��    �ƣ�TFT_PutCharacter(u16 Char_PX,u16 Char_PY,u8 Put_Char,u8 Char_Size,u16 CharColor,u16 BkColor,u8 Cover)
* ��    �ܣ�TFT��Ļ��ָ��λ�ÿ�ʼ ��ʾ�����С��ʽ���ַ�
* ��ڲ�����Char_PX  		X����ʼ����
*           Char_PY			Y����ʼ����
*			Put_Char        ��Ҫ��ʾ���ַ�
* 			Char_Size		�ַ���С   Character_16_24     Character_16_32
*			CharColor       �ַ���ɫ
*			BkColor			������ɫ
*			Cover           ����ɫ�Ƿ���ʾ  1������ԭ����ɫ  0��������ԭ����ɫ
* ���ڲ�������
* ˵    ����
************************************************************************************/
void TFT_PutCharacter(u16 Char_PX,u16 Char_PY,u8 Put_Char,u8 Char_Size,u16 CharColor,u16 BkColor,u8 Cover)
{
	u8 i,j,k;
  	u8 hor_size,ver_size;									//hor_size��������  ver_size��������
	u8 *Character_addr;
	Character_addr = (u8 *)ASCII[Char_Size][Put_Char];		//�����ַ�ASCII������ʼ��ַ
  	switch(Char_Size)
	{
		case 0:hor_size = 16;ver_size = 24;break;			//16 * 24
		case 1:hor_size = 16;ver_size = 32;break;			//16 * 32
		default:break;
	}
  	k = hor_size >> 3;										//�����ֳ�
	for(i = 0;i < ver_size;i ++)
	{
		for(j = 0;j < hor_size;j ++)
		{
			if((Character_addr[i * k + j / 8] << (j % 8)) & 0x80)  //�����ʾ��ʾ�ַ���ɫ
				TFT_SetPoint(Char_PX + j,Char_PY + i,CharColor);
			else if(Cover)										   //����ʾ������ɫ  
				TFT_SetPoint(Char_PX + j,Char_PY + i,BkColor);
		}
	}
}
/************************************************************************************
* ��    �ƣ�TFT_DisplayStringLine(u16 Str_PX,u16 Str_PY,u8 *Str_PTR,u8 Char_Size,u16 CharColor,u16 BkColor)
* ��    �ܣ�TFT��Ļ��ָ��λ�ÿ�ʼ ��ʾ�����С��ʽ��һ���ַ�
* ��ڲ�����Str_PX  		X����ʼ����
*           Str_PY			Y����ʼ����
*			*Str_PTR        �ַ�����
* 			Char_Size		�ַ���С	 Character_16_24     Character_16_32
*			CharColor       �ַ���ɫ
*			BkColor			������ɫ
*			Cover           ����ɫ�Ƿ���ʾ  1������ԭ����ɫ  0��������ԭ����ɫ
* ���ڲ�������
* ˵    ����
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
* ��    �ƣ�TFT_PutChineseLine(u16 Chinese_X, u16 Chinese_Y,u8 **ChineseLine_PTR,u8 Chinese_num,u8 Chinese_Size,u16 ChineseColor,u16 BkColor,u8 Cover)
* ��    �ܣ�TFT��Ļ��ָ��λ�ÿ�ʼ ��ʾ�����С��ʽ��һ������
* ��ڲ�����Chinese_X  				X����ʼ����
*           Chinese_Y				Y����ʼ����
*			**ChineseLine_PTR       ��������
*			Chinese_num				��ʾ���ָ���
* 			Chinese_Size			���ִ�С	 Chinese_32_32     Chinese_48_48
*			ChineseColor       		������ɫ
*			BkColor					������ɫ
*			Cover           		����ɫ�Ƿ���ʾ  1������ԭ����ɫ  0��������ԭ����ɫ
* ���ڲ�������
* ˵    ����
************************************************************************************/
void TFT_PutChineseLine(u16 Chinese_X, u16 Chinese_Y,u8 **ChineseLine_PTR,u8 Chinese_num,u8 Chinese_Size,u16 ChineseColor,u16 BkColor,u8 Cover)
{
    u8 i,j,k,l;
  	u8 hor_size,ver_size;									//hor_size��������  ver_size��������
  	switch(Chinese_Size)
	{
		case 0:hor_size = 32;ver_size = 32;break;			//32 * 32
		case 1:hor_size = 48;ver_size = 48;break;			//48 * 48
		default:break;
	}
  	k = hor_size >> 3;										//�����ֳ�
	for(l = 0;l < Chinese_num;l ++)
	{
		for(i = 0;i < ver_size;i ++)
		{
			for(j = 0;j < hor_size;j ++)
			{
				if((ChineseLine_PTR[l][i * k + j / 8] << (j % 8)) & 0x80)  //�����ʾ��ʾ�ַ���ɫ
					TFT_SetPoint(Chinese_X + j + l * hor_size,Chinese_Y + i,ChineseColor);
				else if(Cover)										       //����ʾ������ɫ  
					TFT_SetPoint(Chinese_X + j + l * hor_size,Chinese_Y + i,BkColor);
			}
		}
	}
}
/************************************************************************************
* ��    �ƣ�void TFT_WriteBMP(u8 Xpos, u16 Ypos, u8 Height, u16 Width, u8 *bitmap)
* ��    �ܣ�TFT��Ļ����ʾͼƬ
* ��ڲ�����Xpos        X����ʼλ��
*			Ypos		Y����ʼλ��
*           Height		X������
*			Width		Y������
*			*bitmap		ͼƬ������ʼλ��
* ���ڲ�������
* ˵    �����˺����н�ָ��bitmapǿ��ת����16���ƣ����ÿ��ȡ*bitmap_ptrʱʵ��
*			ȡ������һ��16λ����,����RGB��׼
************************************************************************************/
void TFT_WriteBMP(u16 Xpos, u16 Ypos, u16 Width, u16 Height,u8 *bitmap)
{
	u32 index;
	u32 size = Height * Width;
	u16 *bitmap_ptr = (u16 *)bitmap;
	
	TFT_SetDisplayWindow(Xpos,Ypos,Width,Height);
	TFT_WriteRAM_Prepare(); 
	for(index = 0; index < size; index ++)		 
		TFT_Write_Date(*bitmap_ptr ++);
	TFT_SetDisplayWindow(0,0,799,479);	  
}
/************************************************************************************
* ��    �ƣ�TFT_ShowBMP_Formflash(u16 size,u8 *bitmap)
* ��    �ܣ�TFT��Ļ����ʾͼƬ  ���ݴ�Ƭ��Flash(W25Q16)�ж�ȡ
* ��ڲ�����size  		һ����ʾ���ݸ���
*			*bitmap  	ͼƬ�����ַ
* ���ڲ�������
* ˵    �������ô˺���֮ǰ Ҫ���ù��λ��TFT_SetCursor  дGRAMʹ��TFT_WriteRAM_Prepare
************************************************************************************/
void TFT_ShowBMP_Formflash(u16 size,u8 *bitmap)
{
	u32 index;
	u16 *bitmap_ptr = (u16 *)bitmap;
	 
	for(index = 0; index < size; index ++)		 
		TFT_Write_Date(*bitmap_ptr ++);	  
}
/************************************************************************************
* ��    �ƣ�TFT_SetDisplayWindow(u8 Xpos, u16 Ypos, u8 Height, u16 Width)
* ��    �ܣ��趨TFT��Ļ�������ɫ������Χ
* ��ڲ�����XSta        ������ʼλ��
*			YSta		������ʼλ��
*           XEnd		�������λ��
*			YEnd		�������λ��	
* ���ڲ�������
* ˵    ����
************************************************************************************/
void TFT_SetDisplayWindow(u16 XSta,u16 YSta,u16 XEnd,u16 YEnd)
{
	if(XEnd >= 800 || YEnd >= 480)
		return;
	TFT_Write_Cmd(0X002A);	
	TFT_Write_Date(XSta >> 8);	    			  //������ʼλ��
	TFT_Write_Date(XSta & 0X00FF);
	TFT_Write_Date(XEnd >> 8);	      			  //������ֹλ��
	TFT_Write_Date(XEnd & 0X00FF);

    TFT_Write_Cmd(0X002B);	
	TFT_Write_Date(YSta >> 8);	    			  //������ʼλ��
	TFT_Write_Date(YSta & 0X00FF);
	TFT_Write_Date(YEnd >> 8);	      			  //������ֹλ��
	TFT_Write_Date(YEnd & 0X00FF);
}
/************************************************************************************
* ��    �ƣ�TFT_Fill(u16 XSta,u16 YSta,u16 XEnd,u16 YEnd,u16 Fill_Color)
* ��    �ܣ���TFT��Ļ��ָ�����������ָ����ɫ
* ��ڲ�����XSta        ������ʼλ��
*			YSta		������ʼλ��
*           XEnd		�������λ��
*			YEnd		�������λ��
*			Fill_Color  ������ɫ	
* ���ڲ�������
* ˵    ����
************************************************************************************/
void TFT_Fill(u16 XSta,u16 YSta,u16 XEnd,u16 YEnd,u16 Fill_Color)
{                    
    u32 num;
	
	TFT_SetDisplayWindow(XSta,YSta,XEnd,YEnd); 
	TFT_WriteRAM_Prepare();         	 	   	   
	num = (u32)(YEnd - YSta + 1) * (XEnd - XSta + 1);    
	while(num --)
		TFT_Write_Date(Fill_Color);	 
}
/************************************************************************************
* ��    �ƣ�TFT_DrawLine(u16 XSta, u16 YSta, u16 XEnd, u16 YEnd, u16 Line_Color)
* ��    �ܣ���TFT��Ļ����������֮�仭һ��ֱ��
* ��ڲ�����XSta        ������ʼλ��
*			YSta		������ʼλ��
*           XEnd		�������λ��
*			YEnd		�������λ��
*			Line_Color  �ߵ���ɫ	
* ���ڲ�������
* ˵    �������һ��ֱ�߸о��������ԣ������������յ�ĺ��᷽��������1���ٻ�����ֱ��
************************************************************************************/
void TFT_DrawLine(u16 XSta, u16 YSta, u16 XEnd, u16 YEnd, u16 Line_Color)
{
    u16 i,j,temp;	 
	if((XSta == XEnd)&&(YSta == YEnd))				  //��ʼλ�� = ��ֹλ��
		TFT_SetPoint(XSta, YSta, Line_Color);
	else
	{ 
		if(abs(YEnd - YSta) > abs(XEnd - XSta))		  //б�ʴ���1 
		{
			if(YSta > YEnd) 
			{
				temp = YSta;						  //�������/�յ�����
				YSta = YEnd;
				YEnd = temp; 
				temp = XSta;
				XSta = XEnd;
				XEnd = temp; 
			}
			for(j = YSta;j < YEnd;j ++)               //��y��Ϊ��׼
			{
				i = (u32)(j - YSta) * (XEnd - XSta) / (YEnd - YSta) + XSta;
				TFT_SetPoint(i,j,Line_Color);  
			}
		}
		else     									 //б��С�ڵ���1
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
			for(i = XSta;i <= XEnd;i ++)  			//��x��Ϊ��׼ 
			{
				j = (u32)(i - XSta) * (YEnd - YSta) / (XEnd - XSta) + YSta;
				TFT_SetPoint(i,j,Line_Color); 
			}
		}
	} 
}
/************************************************************************************
* ��    �ƣ�TFT_DrawCircle(u16 Xptr, u16 Yptr, u8 Radiu, u16 Color)
* ��    �ܣ���TFT��Ļ������һ��ΪԲ�� ������뾶��Բ
* ��ڲ�����Xptr        ����λ��
*			Yptr		����λ��
*			Radiu		Բ�İ뾶
*			Color       ��Բ��ɫ
* ���ڲ�������
* ˵    ��������о�Բ�߲������ԣ������ڰ뾶������1���໭����Բ
************************************************************************************/
void TFT_DrawCircle(u16 Xptr, u16 Yptr, u8 Radiu, u16 Color)
{
	int a,b;
	int di;

	if((Xptr + Radiu) > TFT_7inch_XSize || (Yptr + Radiu) > TFT_7inch_YSize)
		return;
	a = 0;
	b = Radiu;		  
	di = 3 - (Radiu << 1);             				//�ж��¸���λ�õı�־
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
		if(di < 0)									  //ʹ��Bresenham�㷨��Բ
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
* ��    �ƣ�TFT_DrawRectangle(u16 XSta, u16 YSta, u16 XEnd, u16 YEnd, u16 Color)
* ��    �ܣ���TFT��Ļ����������֮�仭һ��������
* ��ڲ�����XSta        ������ʼλ��
*			YSta		������ʼλ��
*			XEnd		�������λ��
*			YEnd        �������λ��
*			Color       ��������ɫ
* ���ڲ�������
* ˵    ��������о���������ɫ��������
*			X����ʼλ��-1��Y����ʼλ��-1
*			X�����λ��+1��Y�����λ��+1		 �ٻ�������
************************************************************************************/
void TFT_DrawRectangle(u16 XSta, u16 YSta, u16 XEnd, u16 YEnd, u16 Color)
{
	TFT_DrawLine(XSta, YSta, XEnd, YSta, Color);
	TFT_DrawLine(XSta, YSta, XSta, YEnd, Color);
	TFT_DrawLine(XSta, YEnd, XEnd, YEnd, Color);
	TFT_DrawLine(XEnd, YSta, XEnd, YEnd, Color);
} 	
/******************************************************************************************
* ��    �ƣ�void TFT_Show_Time(void)
* ��    �ܣ�TFT��Ļ����ʾ��ǰʱ��
* ��ڲ�������
* ���ڲ�������
* ˵    ������ʽ20XX-XX-XX  XX:XX:XX
*******************************************************************************************/
void TFT_Show_Time(void)
{
	
	u8 TimeString[21];
	TimeString[0] = '2';
	TimeString[1] = '0';
	TimeString[2] = CurrentTime.Year / 10 + '0';
	TimeString[3] = CurrentTime.Year % 10 + '0';
	TimeString[4] = '-';
	TimeString[5] = CurrentTime.Month / 10 + '0';
	TimeString[6] = CurrentTime.Month % 10 + '0';
	TimeString[7] = '-';
	TimeString[8] = CurrentTime.Day / 10 + '0';
	TimeString[9] = CurrentTime.Day % 10 + '0';
	TimeString[10] = ' ';
	TimeString[11] = ' ';
	TimeString[12] = CurrentTime.Hour / 10 + '0';
	TimeString[13] = CurrentTime.Hour % 10 + '0';
	TimeString[14] = ':';
	TimeString[15] = CurrentTime.Minute / 10 + '0';
	TimeString[16] = CurrentTime.Minute % 10 + '0';
	TimeString[17] = ':';
	TimeString[18] = CurrentTime.Second / 10 + '0';
	TimeString[19] = CurrentTime.Second % 10 + '0';
	TimeString[20] = '\0';
	TFT_DisplayStringLine(0,448,TimeString,Character_16_32,RED,WHITE,1);	  //������ɫ��  ������ɫ��
}

