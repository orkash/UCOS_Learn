/*
*************************************************************************************
**文件说明：tft液晶操作
**文件名称： tft_lcd.c 
**创建日期：2013.11.06  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#include <math.h>
#include "user_type.h"
#include "fsmc_cfg.h"
#include "gpio_cfg.h"#include "dma_cfg.h"
#include "lcd_tft.h"
#include "num_operation.h"
#include "word.h"


#if (LCD_DMA_CLR_EN > 0) || (LCD_DMA_DAT_EN > 0)
DMA_ControlDat LcdDmaDat;
#endif




void LCD_DelayMs(Uint8 ms)
{
    Uint16 us;
    while(ms){
        for(us = 0; us < 36000; us ++);
        for(us = 0; us < 36000; us ++);
        ms --;
    }    
}
/*
*************************************************************************************
* 名    称：void LCD_InitExt(void)
* 功    能：LCD初始化
* 入口参数：无
* 出口参数：无
*************************************************************************************
*/
void LCD_InitExt(void)
{
    LcdRst();                   //发送lcd复位脉冲
    
	LCD_WriteCmd(0x00E2);		
	LCD_WriteDat(0x0012);		//SSD1963外部晶振频率为10MHz,内核要求大于250MHz,因此36倍频最后得360MHz		   
	LCD_WriteDat(0x0000);		//设置PLL频率V = 2, PLL = 360 / (V + 1) = 120MHz								 
	LCD_WriteDat(0x0004);

	LCD_WriteCmd(0x00E0);  	    //PLL开始指令
	LCD_WriteDat(0x0001);  	    //使能PLL
	LCD_DelayMs(1);
	LCD_WriteCmd(0x00E0);	  	//PLL再次开始
	LCD_WriteDat(0x0003);		//该指令过后PLL输出作为系统时钟
	LCD_DelayMs(5);

	LCD_WriteCmd(0x0001);   	//软件复位命令,没有参数
	LCD_DelayMs(5);
	LCD_WriteCmd(0x00E6);	  	//设置数据移位速度
	LCD_WriteDat(0x0004);
	LCD_WriteDat(0x0093);
	LCD_WriteDat(0x00e0);

	LCD_WriteCmd(0x00B0);	    //液晶屏工作模式设定
//================下边这指令，如果是7寸屏 用0X0000 5寸用0X0020
	LCD_WriteDat(0x0020);
	LCD_WriteDat(0x0000);		//TTL  mode
	LCD_WriteDat((TFT_7inch_XSize >> 8) & 0x00ff);  //设置横向宽度  
	LCD_WriteDat(TFT_7inch_XSize & 0X00FF);
    LCD_WriteDat((TFT_7inch_YSize >> 8) & 0x00ff);  //设置纵向宽度 
	LCD_WriteDat(TFT_7inch_YSize & 0X00FF); 
    LCD_WriteDat(0x0000);		//RGB序列

	LCD_WriteCmd(0x00B4);	           
	LCD_WriteDat((1000 >> 8) & 0X00FF);  
	LCD_WriteDat(1000 & 0X00FF);
	LCD_WriteDat((51 >> 8) & 0X00FF);  
	LCD_WriteDat(51 & 0X00FF);
	LCD_WriteDat(8);			   
	LCD_WriteDat((3 >> 8) & 0X00FF);  
	LCD_WriteDat(3 & 0X00FF);  
	LCD_WriteDat(0x0000);

	LCD_WriteCmd(0x00B6);	           
	LCD_WriteDat((530 >> 8) & 0X00FF);   
	LCD_WriteDat(530 & 0X00FF);
	LCD_WriteDat((24 >> 8) & 0X00FF);  
	LCD_WriteDat(24 & 0X00FF);
	LCD_WriteDat(3);			   
	LCD_WriteDat((23 >> 8) & 0X00FF);  
	LCD_WriteDat(23 & 0X00FF);  

	LCD_WriteCmd(0x00BA);
	LCD_WriteDat(0x0005);           

	LCD_WriteCmd(0x00B8);
	LCD_WriteDat(0x0007);    
	LCD_WriteDat(0x0001);    

	LCD_WriteCmd(0x0036);       
	LCD_WriteDat(0x0000);

	LCD_WriteCmd(0x00F0);    
	LCD_WriteDat(0x0003);
	LCD_DelayMs(5);
	LCD_WriteCmd(0x0029);        

	LCD_WriteCmd(0x00BE);        
	LCD_WriteDat(0x0006);
	LCD_WriteDat(0x0080);
	
	LCD_WriteDat(0x0001);
	LCD_WriteDat(0x00f0);
	LCD_WriteDat(0x0000);
	LCD_WriteDat(0x0000);

	LCD_WriteCmd(0x00d0);//设置动态背光控制配置 
	LCD_WriteDat(0x000d);
}

/*
*************************************************************************************
* 名    称：void LCD_SetCursor(LCD_CursorDat * Pos)
* 功    能：LCD光标位置设置
* 入口参数：LCD_CursorDat * Pos
* 出口参数：无
*************************************************************************************
*/
void LCD_SetCursor(LCD_CursorDat * Pos)
{
    LCD_WriteCmd(LCD_SET_COLUMN_ADDRESS);     //设置列地址
    LCD_WriteDat(Pos->X >> 8);
    LCD_WriteDat(Pos->X & 0x00ff);
    LCD_WriteDat(TFT_7inch_XSize >> 8);
    LCD_WriteDat(TFT_7inch_XSize & 0x00ff);
        LCD_WriteCmd(LCD_SET_PAGE_ADDRESS);       //设置行地址
    LCD_WriteDat(Pos->Y >> 8);
    LCD_WriteDat(Pos->Y & 0x00ff);
    LCD_WriteDat(TFT_7inch_YSize >> 8);
    LCD_WriteDat(TFT_7inch_YSize & 0x00ff);
}

/*
*************************************************************************************
* 名    称：void LCD_WriteNDat(Uint16 Num,Uint16 * DatP)
* 功    能：写Num个数据到LCD屏幕
* 入口参数：Uint16 Num,
            Uint16 * DatP
* 出口参数：无
*************************************************************************************
*/
void LCD_WriteNDat(Uint16 Num,Uint16 * DatP)
{
    LCD_WriteCmd(LCD_WRITE_MEM_START);   //开始写LCD内存命令
    while(Num --){
        LCD_WriteDat(* DatP);
        DatP ++;
    }
}
void LCD_WriteOneDat(Uint16 Dat)
{
    LCD_WriteCmd(LCD_WRITE_MEM_START);   //开始写LCD内存命令
    LCD_WriteDat(Dat);
}

/*
*************************************************************************************
* 名    称：void LCD_Clear(LCD_ColorDat Color)
* 功    能：LCD写屏幕
* 入口参数：LCD_ColorDat Color
* 出口参数：无
*************************************************************************************
*/
void LCD_Clear(LCD_ColorDat Color)
{
    Uint32 Index;
  
    LCD_CursorDat Point;
    Point.X = 0;
    Point.Y = 0;
    LCD_SetCursor(& Point);              //设置光标零点
    LCD_WriteCmd(LCD_WRITE_MEM_START);   //开始写LCD内存命令

    for(Index = 0; Index < 800 * 480; Index ++){
        LCD_WriteDat(Color);
    }
   
}


/*
*************************************************************************************
* 名    称：Uint8 LCD_FillRect(LCD_FillDat * FillDat)
* 功    能：LCD屏幕矩形填充
* 入口参数：LCD_FillDat * DatP  填充数据参数指针
* 出口参数：ERROR  /  SUCCESS
*************************************************************************************
*/
Uint8 LCD_FillRect(LCD_FillDat * FillDat)
{
#if LCD_DMA_DAT_EN == 0
    Uint16 Index;
#endif
    if((FillDat->XSize > LCD_XSIZE) || (FillDat->YSize > LCD_YSIZE)){  //填充面积超范围
        return ERROR;
    }
#if LCD_DMA_DAT_EN == 0                               //
    else if(FillDat->XSize == LCD_XSIZE){          //Xsize为满屏地址，则光标只需设置首地址
        LCD_SetCursor(& FillDat->StartPoint);      //设置光标起始地址
        LCD_WriteNDat(FillDat->XSize * FillDat->YSize, 
                      FillDat->DatBuf);            //直接写入数据
    }
    else {
        for(Index = 0; Index < FillDat->YSize; Index ++){
            LCD_SetCursor(& FillDat->StartPoint);   //设置光标起始地址
            FillDat->StartPoint.Y ++;               //光标下移一格
            LCD_WriteNDat(FillDat->XSize,           //直接写入数据
                          FillDat->DatBuf + Index * FillDat->XSize);
        }
    }
#else

#endif    
    return SUCCESS;
}
/*
*************************************************************************************
* 名    称：void LCD_DrawPoint(LCD_PointDat * PointDat)
* 功    能：LCD屏幕打点
* 入口参数：LCD_PointDat * PointDat  点数据参数指针
* 出口参数：无
*************************************************************************************
*/
void LCD_DrawPoint(LCD_PointDat * PointDat)
{
    LCD_SetCursor(&PointDat->Point);       //设置光标
    LCD_WriteCmd(LCD_WRITE_MEM_START);     //开始写LCD内存命令
    LCD_WriteDat(PointDat->Color);         //写入颜色
}


Uint8 LCD_DrawLine(LCD_CursorDat StartPoint,LCD_CursorDat EndPoint,LCD_ColorDat Color)
{
    Float32 increx,increy,tmpx,tmpy;
    Uint16 Index,steps;
    LCD_PointDat  PointDat;
    if((EndPoint.X > LCD_XSIZE) || (EndPoint.Y > LCD_YSIZE)){
        return ERROR;                         //非法参数超过屏幕最大范围
    }
    
    PointDat.Color = Color;
    
    if(EndPoint.X == StartPoint.X){           //X1=X2则为平行Y轴直线
        if(StartPoint.Y < EndPoint.Y){
            PointDat.Point.X = StartPoint.X;  //X坐标不变
            for(PointDat.Point.Y = StartPoint.Y ;
                PointDat.Point.Y <= EndPoint.Y ; PointDat.Point.Y ++){
                
                LCD_DrawPoint(&PointDat);
            }
        }
        else{
            PointDat.Point.X = StartPoint.X;  //X坐标不变
            for(PointDat.Point.Y = EndPoint.Y ;
                PointDat.Point.Y <= StartPoint.Y ; PointDat.Point.Y ++){
                
                LCD_DrawPoint(&PointDat);
            }
        }
    }
    else if(EndPoint.Y == StartPoint.Y){      //Y1=Y2则为平行X轴直线
        if(StartPoint.X < EndPoint.X){
            PointDat.Point.Y = StartPoint.Y;  //Y坐标不变
            for(PointDat.Point.X = StartPoint.X ;
                PointDat.Point.X <= EndPoint.X ; PointDat.Point.X ++){
                
                LCD_DrawPoint(&PointDat);
            }
        }
        else {
            PointDat.Point.Y = StartPoint.Y;  //Y坐标不变
            for(PointDat.Point.X = EndPoint.X ;
                PointDat.Point.X <= StartPoint.X ; PointDat.Point.X ++){
                
                LCD_DrawPoint(&PointDat);
            }            
        }
    }
    else {                                    //常规参数 　DDA算法生成直线
        if(abs(EndPoint.X - StartPoint.X) > abs(EndPoint.Y - StartPoint.Y)){
            steps = abs(EndPoint.X - StartPoint.X);
        }
        else{
            steps = abs(EndPoint.Y - StartPoint.Y);
        }

        increx = (Float32)(EndPoint.X - StartPoint.X) / steps;
        increy = (Float32)(EndPoint.Y - StartPoint.Y) / steps;
        
        tmpx = StartPoint.X;
        tmpy = StartPoint.Y;
        for(Index = 1;Index <= steps;Index++){
            LCD_DrawPoint(&PointDat);         //画点
            tmpx += increx;
            tmpy += increy;
            PointDat.Point.X = tmpx;
            PointDat.Point.Y = tmpy;
        }       
    }
    return SUCCESS;
}
void putdot(LCD_PointDat * Center,Int16 x,Int16 y)
{
    LCD_PointDat TmpPoint;
    
    TmpPoint = * Center;
    TmpPoint.Point.X += x;
    TmpPoint.Point.Y += y;
    LCD_DrawPoint(&TmpPoint);
    TmpPoint = * Center;
    TmpPoint.Point.X += x;
    TmpPoint.Point.Y -= y;
    LCD_DrawPoint(&TmpPoint);
    
    TmpPoint = * Center;
    TmpPoint.Point.X -= x;
    TmpPoint.Point.Y += y;
    LCD_DrawPoint(&TmpPoint);
    
    TmpPoint = * Center;
    TmpPoint.Point.X -= x;
    TmpPoint.Point.Y -= y;
    LCD_DrawPoint(&TmpPoint);

    TmpPoint = * Center;
    TmpPoint.Point.X += y;
    TmpPoint.Point.Y += x;
    LCD_DrawPoint(&TmpPoint);
    
    TmpPoint = * Center;
    TmpPoint.Point.X += y;
    TmpPoint.Point.Y -= x;
    LCD_DrawPoint(&TmpPoint);

    TmpPoint = * Center;
    TmpPoint.Point.X -= y;
    TmpPoint.Point.Y += x;
    LCD_DrawPoint(&TmpPoint);
    
    TmpPoint = * Center;
    TmpPoint.Point.X -= y;
    TmpPoint.Point.Y -= x;
    LCD_DrawPoint(&TmpPoint);

    
}
void LCD_DrawCircle(LCD_CursorDat * Center,LCD_CursorDat * Any,Uint16 Color)
{
    Int16 x,y,r;
    Float32 d;
    LCD_PointDat CenterPoint;
    CenterPoint.Point = * Center;
    CenterPoint.Color = Color;
    x = Any->X - Center->X;
    y = Any->Y - Center->Y;
    r = sqrt(x * x + y * y);             //已知圆心及圆上一点，求半径
    x = 0;
    y = r;
    d = 5.0 / 4 - r;
    while(x <= y){
        putdot(&CenterPoint,x,y);
        if(d<0){
            d += x * 2.0 + 3;
        }
        else{
            d += 2.0 * (x - y) + 5;
            y --;
        } 
        x ++;
    }
}
      
/*
*************************************************************************************
* 名    称：void LCD_DrawShape(LCD_DrawDat * DatP)
* 功    能：在给定绘图缓存中绘制图形  
* 入口参数：LCD_DrawDat * DatP  绘图参数指针
* 出口参数：成功/失败
*************************************************************************************
*/
Uint8 LCD_DrawShape(LCD_DrawDat * DrawDat)
{
    Uint8 err;
    LCD_CursorDat NowPoint;
    LCD_CursorDat NextPoint;
    
    switch(DrawDat->Shape){
        case DARW_CLASS_LINE:        //画直线
            err = LCD_DrawLine(DrawDat->StartPoint,
                               DrawDat->EndPoint,
                               DrawDat->LineColor);
            break;                   
        case DARW_CLASS_RECT:        //画矩形
            NowPoint    = DrawDat->StartPoint;
            NextPoint   = DrawDat->StartPoint;
            NextPoint.X = DrawDat->EndPoint.X;
            err = LCD_DrawLine(NowPoint,NextPoint,DrawDat->LineColor);
            
            NowPoint    = DrawDat->StartPoint;
            NextPoint   = DrawDat->EndPoint;
            NowPoint.X  = DrawDat->EndPoint.X;
            err = LCD_DrawLine(NowPoint,NextPoint,DrawDat->LineColor);
            NowPoint    = DrawDat->StartPoint;
            NextPoint   = DrawDat->StartPoint;
            NextPoint.Y = DrawDat->EndPoint.Y;
            err = LCD_DrawLine(NowPoint,NextPoint,DrawDat->LineColor);

            NowPoint    = DrawDat->EndPoint;
            NextPoint   = DrawDat->EndPoint;
            NowPoint.X  = DrawDat->StartPoint.X;
            err = LCD_DrawLine(NowPoint,NextPoint,DrawDat->LineColor);
                
            break;
        case DARW_CLASS_CIRCLE:      //画正圆
            LCD_DrawCircle(&DrawDat->StartPoint,&DrawDat->EndPoint,DrawDat->LineColor);
            break;
        case DARW_CLASS_OVAL:        //画椭圆
            break;
        default : break;    
    }
    return err;
}

/*
*************************************************************************************
* 名    称：void LCD_WriteNum(NUM_DisBuf * Disp)
* 功    能：写ASIIC码到LCD，以逐行方式写。
* 入口参数：NUM_DisBuf * Disp 显示参数指针
* 出口参数：无
*************************************************************************************
*/
const Uint16 ASIICSizeTable[] = {0x0808,0x080f}; //ASCII字库字体size表，索引标示见word.h
void LCD_WriteNum(NUM_DisBuf * Disp)
{
    Uint8 n,x,y;
    Uint8 Temp;
    for(n = Disp->Offset;n < Disp->DisN + Disp->Offset;n++){
        for(y = 0; y < (ASIICSizeTable[Disp->NumSize] & 0x00ff); y++){
            LCD_SetCursor(&Disp->DisControl.Place);             //写入行光标，列光标自动递增。
            LCD_WriteCmd(LCD_WRITE_MEM_START);                  //开始写LCD内存命令
            Disp->DisControl.Place.Y ++;                        //逐行写入
            for(x = 0;x < (ASIICSizeTable[Disp->NumSize] >> 11); x++){
                Temp = Disp->DisBuf[n][y * (ASIICSizeTable[Disp->NumSize] >> 11) + x];   //取1字节
                for(Disp->Index = 0; Disp->Index < 8; Disp->Index ++ ){ //8 bit逐点取出
                    if(Temp & 0x80){   //写字体色
                        LCD_WriteDat(Disp->DisControl.WordColor);
                    }
                    else{              //写背景色
                        LCD_WriteDat(Disp->DisControl.BackgrdColor);
                    }
                    Temp = Temp << 1;
                }
            }
        }
        Disp->DisControl.Place.Y -= (ASIICSizeTable[Disp->NumSize] & 0x00ff);   //行光标复原
        Disp->DisControl.Place.X += (ASIICSizeTable[Disp->NumSize] >> 8);       //列光标跨字
    }        
}

/*
*************************************************************************************
* 名    称：void LCD_WriteWord(WordDisBuf * Disp)
* 功    能：写汉字到LCD，以逐行方式写。
* 入口参数：WordDisBuf * Disp 显示参数指针
* 出口参数：无
*************************************************************************************
*/
const Uint16 WordSizeTable[] = {0x0f0f,0x1818,0x2828,0x3030}; //汉字字库字体size表，索引标示见word.h
void LCD_WriteWord(WordDisBuf * Disp){
    Uint8 n,x,y;
    Uint8 Temp;
    for(n = 0; n < Disp->DisN; n ++){
        for(y = 0; y < (WordSizeTable[Disp->WordSize] & 0x00ff); y++){
            LCD_SetCursor(&Disp->DisControl.Place);             //写入行光标，列光标自动递增。
            LCD_WriteCmd(LCD_WRITE_MEM_START);                  //开始写LCD内存命令
            Disp->DisControl.Place.Y ++;                        //逐行写入
            for(x = 0;x < (WordSizeTable[Disp->WordSize] >> 11); x++){
                Temp = Disp->DisBuf[n][y * (WordSizeTable[Disp->WordSize] >> 11) + x];   //取1字节
                for(Disp->Index = 0; Disp->Index < 8; Disp->Index ++ ){ //8 bit逐点取出
                    if(Temp & 0x80){   //写字体色
                        LCD_WriteDat(Disp->DisControl.WordColor);
                    }
                    else{              //写背景色
                        LCD_WriteDat(Disp->DisControl.BackgrdColor);
                    }
                    Temp = Temp << 1;
                }
            }
        }
        Disp->DisControl.Place.Y -= (WordSizeTable[Disp->WordSize] & 0x00ff);   //行光标复原
        Disp->DisControl.Place.X += (WordSizeTable[Disp->WordSize] >> 8);       //列光标跨字
    }
}

/*
*************************************************************************************
* 名    称：Uint8 LCD_DrawCoordinate(CoordinateDat * Disp)
* 功    能：绘制坐标系
* 入口参数：CoordinateDat * Disp 绘制坐标控制数据
* 出口参数：无
*************************************************************************************
*/
Uint8 LCD_DrawCoordinate(CoordinateDat * Disp)
{
    LCD_CursorDat XEnd;
    LCD_CursorDat YEnd;
    LCD_CursorDat Origin;
    LCD_CursorDat Tmp;
    Uint8 i;
    if((Disp->Origin.X + Disp->XMax>= LCD_XSIZE) || 
       (Disp->Origin.Y + Disp->YMax>= LCD_YSIZE)){
        return ERROR;                           //非法参数超过屏幕最大范围
    }
    Origin = Disp->Origin;

    Origin.Y = LCD_YSIZE + 1 - Origin.Y;        //取左下方原点
    
    XEnd   = Origin;                            //定义X，Y轴端点坐标
    YEnd   = Origin;
    XEnd.X = Origin.X + Disp->XMax;
    YEnd.Y = Origin.Y - Disp->YMax;
    
    LCD_DrawLine(Origin,XEnd,Disp->PosColor);   //坐标x轴
    LCD_DrawLine(Origin,YEnd,Disp->PosColor);   //坐标y轴
    
    Tmp = Origin;                               //重画坐标加粗坐标线
    Tmp.X ++;
    Tmp.Y --;
    XEnd.Y --;
    YEnd.X ++;    
    LCD_DrawLine(Tmp,XEnd,Disp->PosColor);   //坐标x轴
    LCD_DrawLine(Tmp,YEnd,Disp->PosColor);   //坐标y轴
    
    Tmp    = XEnd;
    Tmp.X -= 20;
    for(i = 0; i < 10; i ++){
        Tmp.Y --;
        LCD_DrawLine(XEnd,Tmp,Disp->PosColor);   //坐标x轴箭头        
    }
    
    Tmp    = YEnd;
    Tmp.Y += 20; //
    for(i = 0; i < 10; i ++){
        Tmp.X ++;
        LCD_DrawLine(YEnd,Tmp,Disp->PosColor);   //坐标y轴箭头        
    }
    Disp->CoorDisDat.NumSize = ASCII_5_8;
    Disp->CoorDisDat.DisControl.Place = YEnd;
    Disp->CoorDisDat.DisControl.Place.Y -= 8;
    Disp->CoorDisDat.DisControl.WordColor = Disp->PosColor;
    Disp->CoorDisDat.DisControl.BackgrdColor = MEDIUM_PURPLE;
    NUM_LongHexToBCD(Disp->YMaxDat,&Disp->CoorDisDat); //YMaxDat到显示缓存 并计算首位0个数
    Disp->CoorDisDat.Offset = Disp->CoorDisDat.ZeroN;  //0偏移 显示右对齐
    Disp->CoorDisDat.DisN  -= Disp->CoorDisDat.ZeroN;  //0不显示
    LCD_WriteNum(&Disp->CoorDisDat);                   //显示YMaxDat
    
    Disp->CoorDisDat.NumSize = ASCII_5_8;
    Disp->CoorDisDat.DisControl.Place = XEnd;
    Disp->CoorDisDat.DisControl.Place.Y -= 8;
    Disp->CoorDisDat.DisControl.WordColor = Disp->PosColor;
    Disp->CoorDisDat.DisControl.BackgrdColor = MEDIUM_PURPLE;
    NUM_LongHexToBCD(Disp->XMaxDat,&Disp->CoorDisDat); //YMaxDat到显示缓存 并计算首位0个数
    Disp->CoorDisDat.Offset = Disp->CoorDisDat.ZeroN;  //0偏移 显示右对齐
    Disp->CoorDisDat.DisN  -= Disp->CoorDisDat.ZeroN;  //0不显示
    LCD_WriteNum(&Disp->CoorDisDat);                   //显示YMaxDat    
    
    
    return SUCCESS;
}

/*
*************************************************************************************
* 名    称：void LCD_Init(void)
* 功    能：LCD初始化
* 入口参数：无
* 出口参数：无
*************************************************************************************
*/
void LCD_Init(void)
{   
    FSMC_Config(EN_FSMC_PSRAM,ENABLE);       //开启PSRAM FSMC接口
    GPIO_Config(EN_GPIO_FSMC | EN_GPIO_LCD,
                0);                          //开启lcd控制及FSMC GPIO
    LCD_InitExt();                           //LCD命令初始化
    LCD_Clear(BLACK);                        //纯黑底色
}






