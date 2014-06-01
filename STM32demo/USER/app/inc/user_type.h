/*
*************************************************************************************
**文件说明：数据类型定义文件
**文件名称：user_type.h 
**创建日期：2009.07.21  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/


#ifndef USER_TYPE_H
#define USER_TYPE_H
/*
------------------------------------------------------------------------------------
说明：数据类型，位长度。
------------------------------------------------------------------------------------
*/
typedef unsigned       char  Bool;
typedef unsigned       char  Uint8;
typedef unsigned short int   Uint16;
typedef unsigned       int   Uint32;
typedef signed         char  Int8;
typedef signed short   int   Int16;
typedef signed         int   Int32;
typedef float                Float32;

/*
-------------------------------------------------------------------------------------
说明: 真假值
-------------------------------------------------------------------------------------

typedef enum {TRUE = 1, FLASE = !TRUE} BoolStatus;
*/

/*
-------------------------------------------------------------------------------------
说明: 8bit位掩码 
-------------------------------------------------------------------------------------
*/
typedef enum{
	MASK_BIT0 = 0x01,
	MASK_BIT1 = 0x02,
	MASK_BIT2 = 0x04,
	MASK_BIT3 = 0x08,
	MASK_BIT4 = 0x10,
	MASK_BIT5 = 0x20,
	MASK_BIT6 = 0x40,
	MASK_BIT7 = 0x80,
}BIT_MASK;

//---------------------------------------------------------------------------------
//说明：  Set or clear all bits in the mask
//---------------------------------------------------------------------------------

#define ClearMask(addr,mask) addr = (addr & ~(mask))
#define SetMask(addr,mask) addr = (addr | (mask))
#define SeeMask(addr,mask) addr = (addr & mask) 

#define ReadMask(addr,mask) (addr & (mask))
#define WriteMask(addr,data,mask) addr = (addr & ~(mask)) | (data)
#define AntiMask(addr,mask) addr=(addr | mask) & ~(addr & mask) 

/*
-------------------------------------------------------------------------------------
说明: USB命令码
-------------------------------------------------------------------------------------
*/
typedef struct 
{
	Uint8 Cmd;
	Uint8 CmdPar[7];
}USBCmd;


/**************************************************
说明：DMA管理数据类型
**************************************************/
typedef struct 
{
    Uint32 DMA_PeripheralBaseAddr;
    Uint32 DMA_MemoryBaseAddr;
    Uint16 DMA_BufferSize;
    Uint16 DMA_Mode;
    Uint8  DMA_InterruptCnt;
    Uint8  DMA_InterruptMax;
}DMA_ControlDat;



/**************************************************
说明：LCD色彩数据   据彩色液晶色彩长度定义
**************************************************/
typedef  Uint16          LCD_ColorDat;

/**************************************************
说明：LCD光标数据格式
(0,0)
---------------------->X
|
|
|
|
|
|
|
Y
**************************************************/
typedef struct
{
    Uint16 X;
    Uint16 Y;
}LCD_CursorDat;

/**************************************************
说明: LCD填充数据格式 (矩形填充)
**************************************************/
typedef struct 
{
    LCD_CursorDat StartPoint;  //起始点
    Uint16 XSize;              //X方向长度(像素单位)
    Uint16 YSize;              //Y方向长度(像素单位)
    Uint16 * DatBuf;           //填充数据指针
}LCD_FillDat;

/**************************************************
说明：绘图类型
**************************************************/
#define  DARW_CLASS_LINE              ((Uint8)0x01)
#define  DARW_CLASS_RECT              ((Uint8)0x02)
#define  DARW_CLASS_CIRCLE            ((Uint8)0x04)
#define  DARW_CLASS_OVAL              ((Uint8)0x08)
/**************************************************
说明: LCD画图数据格式
**************************************************/
typedef struct
{
    LCD_CursorDat StartPoint;       //图形起始点
    LCD_CursorDat EndPoint;         //图形结束点
    LCD_ColorDat  LineColor;        //图形线颜色
    LCD_ColorDat  BackgroundColor;  //图形背景色
    Uint8         Shape;            //图形形状 参见绘图类型
    Uint16      * DatBuf;           //绘图缓存
}LCD_DrawDat;

/**************************************************
说明: LCD点数据格式
**************************************************/
typedef struct
{
    LCD_CursorDat Point;             //点位置
    LCD_ColorDat  Color;             //点颜色
}LCD_PointDat;

/**************************************************
说明: LCD点数据格式
**************************************************/
typedef struct
{
    LCD_CursorDat Place;             //位置
    LCD_ColorDat  WordColor;         //字体颜色
    LCD_ColorDat  BackgrdColor;      //字体背景颜色
}LCD_WordDat;




/***************************************************
说明：RTC数据格式
****************************************************/
typedef struct
{
	Uint8 Year;    //年
	Uint8 Month;   //月
	Uint8 Day;     //日
	Uint8 Hour;    //时
	Uint8 Minute;  //分
	Uint8 Second;  //秒
}TimeDat;
/**************************************************
说明: 数字显示缓存
**************************************************/
typedef struct
{
    Uint8 Index;                //数字索引
    Uint8 NumSize;              //数字大小 参见word.h
    Uint8 DisN;                 //显示个数
    Uint8 ZeroN;                //首位0个数
    Uint8 Offset;               //缓存中的偏移
    Uint8 BcdNum[20];           //BCD码缓存
    const Uint8 *DisBuf[20];    //显示字库缓存
    LCD_WordDat DisControl;     //显示控制 位置及颜色
}NUM_DisBuf;


/**************************************************
说明: 汉字显示缓存
**************************************************/
typedef struct
{
    Uint8 Index;                //汉字索引
    Uint8 WordSize;             //汉字大小 参见word.h
    Uint8 DisN;                 //显示个数
    const Uint8 **DisBuf;       //显示字库指针
    LCD_WordDat DisControl;     //显示控制 位置及颜色
}WordDisBuf;


/**************************************************
说明: 坐标绘制控制数据
**************************************************/
typedef struct
{
    LCD_CursorDat Origin;       //坐标原点
    Uint16 YMax;                //Y轴顶点
    Uint16 XMax;                //X轴顶点
    Uint32 YMaxDat;             //Y最大值
    Uint32 XMaxDat;             //X最大值
    LCD_ColorDat  PosColor;     //坐标颜色
    NUM_DisBuf CoorDisDat;      //坐标值显示控制缓存
}CoordinateDat;
		
#endif
