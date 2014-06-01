/*
*************************************************************************************
**文件说明：tft液晶操作
**文件名称： tft_lcd.h 
**创建日期：2013.11.06  
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#ifndef TFT_LCD_H
#define TFT_LCD_H
#include "user_type.h"
#include "dma_cfg.h"
#include "num_operation.h"


/**************************************************
说明：LCD显示操作方式  
0 关闭 液晶采用直接驱动方式
1 开启 液晶采用DMA驱动方式
**************************************************/
#define  LCD_DMA_CLR_EN       (0)
#define  LCD_DMA_DAT_EN       (0)

/**************************************************
说明：LCD显示缓存 
0 关闭 液晶采用无显存操作
1 开启 液晶采用显存操作
**************************************************/
#define  LCD_DIS_MEM_BUF_EN   (0)


/**************************************************
说明：LCD像素
**************************************************/
#define TFT_7inch_XSize       (800 - 1)
#define TFT_7inch_YSize	      (480 - 1)

#define LCD_XSIZE             (TFT_7inch_XSize)
#define LCD_YSIZE             (TFT_7inch_YSize)

/**************************************************
说明：LCD端口操作
**************************************************/
#define  LCD_ADDR_CMD         ((Uint32)0x60000002)
#define  LCD_ADDR_DAT         ((Uint32)0x60000004)

#define  LCD_WriteCmd(Cmd)    (*(__IO Uint16 *)LCD_ADDR_CMD) = (Uint16)Cmd
#define  LCD_WriteDat(Dat)    (*(__IO Uint16 *)LCD_ADDR_DAT) = (Uint16)Dat

/**************************************************
说明：LCD操作 CMD
**************************************************/
#define LCD_SOFT_RESET         (0x01)
#define LCD_SET_DISPLAY_OFF    (0x28)
#define LCD_SET_DISPLAY_ON     (0x29)
#define LCD_SET_COLUMN_ADDRESS (0x2a)
#define LCD_SET_PAGE_ADDRESS   (0x2b)
#define LCD_WRITE_MEM_START    (0x2c)
#define LCD_READ_MEM_START     (0x2e)

/**************************************************
说明：16位TFT液晶颜色索引
**************************************************/
#define  BLACK  0
#define  DIMGRAY  27469
#define  GRAY  33808
#define  DARK_GRAY  44373
#define  SILVER  50712
#define  LIGHT_GRAY  54938
#define  GAINSBORO  57051
#define  WHITE_SMOKE  63390
#define  WHITE  65503
#define  SNOW  65503
#define  IRON_GRAY  25290
#define  SAND_BEIGE  58904
#define  ROSY_BROWN  48209
#define  LIGHT_CORAL  62480
#define  INDIAN_RED  51915
#define  BROWN  41285
#define  FIRE_BRICK  45316
#define  MAROON  32768
#define  DARK_RED  34816
#define  STRONG_RED  57344
#define  RED  63488
#define  PERSIMMON  64072
#define  MISTY_ROSE  65308
#define  SALMON  64526
#define  SCARLET  63744
#define  TOMATO  64264
#define  DARK_SALMON  60559
#define  CORAL  64458
#define  ORANGE_RED  64000
#define  LIGHT_SALMON  64783
#define  VERMILION  64064
#define  SIENNA  41605
#define  TROPICAL_ORANGE  64518
#define  CAMEL  41800
#define  APRICOT  58572
#define  COCONUT_BROWN  18624
#define  SEASHELL  65437
#define  SADDLE_BROWN  35330
#define  CHOCOLATE  54083
#define  BURNT_ORANGE  51840
#define  SUN_ORANGE  64384
#define  PEACH_PUFF  65239
#define  SAND_BROWN  62732
#define  BRONZE  48006
#define  LINEN  65436
#define  HONEY_ORANGE  64908
#define  PERU  52231
#define  SEPIA  29186
#define  OCHER  52100
#define  BISQUE  65304
#define  TANGERINE  62464
#define  DARK_ORANGE  64576
#define  ANTIQUE_WHITE  65370
#define  TAN  54673
#define  BURLY_WOOD  56784
#define  BLANCHED_ALMOND  65369
#define  NAVAJO_WHITE  65237
#define  MARIGOLD  64704
#define  PAPAYA_WHIP  65370
#define  PALE_OCRE  52625
#define  KHAKI  39747
#define  MOCCASIN  65302
#define  OLD_LACE  65436
#define  WHEAT  63190
#define  PEACH  65302
#define  ORANGE  64768
#define  FLORAL_WHITE  65502
#define  GOLDENROD  56580
#define  DARK_GOLDENROD  48129
#define  COFFEE  18880
#define  JASMINE  58891
#define  AMBER  64960
#define  CORNSILK  65499
#define  CHROME_YELLOW  58816
#define  GOLDEN  65152
#define  LEMON_CHIFFON  65497
#define  LIGHT_KHAKI  63249
#define  PALE_GOLDENROD  61269
#define  DARK_KHAKI  48525
#define  MIMOSA  59078
#define  CREAM  65498
#define  IVORY  65502
#define  BEIGE  63387
#define  LIGHT_YELLOW  65500
#define  LIGHT_GOLDENROD_YELLOW  65498
#define  CHAMPAGNE_YELLOW  65491
#define  MUSTARD  52809
#define  MOON_YELLOW  65481
#define  OLIVE  33792
#define  CANARY_YELLOW  65472
#define  YELLOW  65472
#define  MOSS_GREEN  27524
#define  LIGHT_LIME  53184
#define  OLIVE_DRAB  27716
#define  YELLOW_GREEN  40518
#define  DARK_OLIVE_GREEN  21317
#define  APPLE_GREEN  36608
#define  GREEN_YELLOW  44997
#define  GRASS_GREEN  40713
#define  LAWN_GREEN  32704
#define  CHARTREUSE  32704
#define  FOLIAGE_GREEN  30151
#define  FRESH_LEAVES  40905
#define  BRIGHT_GREEN  26560
#define  COBALT_GREEN  26571
#define  HONEYDEW  63454
#define  DARK_SEA_GREEN  36305
#define  LIGHT_GREEN  38738
#define  PALE_GREEN  40915
#define  IVY_GREEN  13766
#define  FOREST_GREEN  9284
#define  LIME_GREEN  13894
#define  DARK_GREEN  768
#define  GREEN  1024
#define  LIME  1984
#define  MALACHITE  9733
#define  MINT  5317
#define  CELADON_GREEN  30481
#define  EMERALD  22095
#define  TURQUOISE_GREEN  20240
#define  VERIDIAN  4998
#define  HORIZON_BLUE  42969
#define  SEA_GREEN  11338
#define  MEDIUM_SEA_GREEN  15758
#define  MINT_CREAM  63455
#define  SPRING_GREEN  2000
#define  PEACOCK_GREEN  1291
#define  MEDIUM_SPRING_GREEN  2003
#define  MEDIUM_AQUAMARINE  26197
#define  AQUAMARINE  32730
#define  CYAN_BLUE  3537
#define  AQUA_BLUE  26588
#define  TURQUOISE_BLUE  14105
#define  TURQUOISE  13977
#define  LIGHT_SEA_GREEN  9621
#define  MEDIUM_TURQUOISE  20121
#define  LIGHT_CYAN  59359
#define  BABY_BLUE  59359
#define  PALE_TURQUOISE  44893
#define  DARK_SLATE_GRAY  10825
#define  TEAL  1040
#define  DARK_CYAN  1105
#define  CYAN  2015
#define  AQUA  44764
#define  DARK_TURQUOISE  1626
#define  CADET_BLUE  23764
#define  PEACOCK_BLUE  1041
#define  POWDER_BLUE  46876
#define  STRONG_BLUE  782
#define  LIGHT_BLUE  44764
#define  PALE_BLUE  32217
#define  SAXE_BLUE  17622
#define  DEEP_SKY_BLUE  1503
#define  SKY_BLUE  34397
#define  LIGHT_SKY_BLUE  34399
#define  MARINE_BLUE  527
#define  PRUSSIAN_BLUE  394
#define  STEEL_BLUE  17430
#define  ALICE_BLUE  63455
#define  SLATE_GRAY  29714
#define  LIGHT_SLATE_GRAY  29779
#define  DODGER_BLUE  7327
#define  MINERAL_BLUE  595
#define  AZURE  991
#define  WEDGWOOD_BLUE  21527
#define  LIGHT_STEEL_BLUE  46619
#define  COBALT_BLUE  533
#define  PALE_DENIM  23576
#define  CORNFLOWER_BLUE  25757
#define  SALVIA_BLUE  19484
#define  DARK_POWDER_BLUE  403
#define  SAPPHIRE  2316
#define  INTERNATIONAL_KLEIN_BLUE  340
#define  CERULEAN_BLUE  10903
#define  ROYAL_BLUE  17244
#define  DARK_MINERAL_BLUE  8591
#define  ULTRAMARINE  415
#define  LAPIS_LAZULI  2463
#define  GHOST_WHITE  65503
#define  LAVENDER  59167
#define  PERIWINKLE  52831
#define  MIDNIGHT_BLUE  6350
#define  NAVY_BLUE  16
#define  DARK_BLUE  17
#define  MEDIUM_BLUE  25
#define  BLUE  31
#define  WISTERIA  23196
#define  DARK_SLATE_BLUE  18897
#define  SLATE_BLUE  27353
#define  MEDIUM_SLATE_BLUE  31581
#define  MAUVE  25119
#define  LILAC  46303
#define  MEDIUM_PURPLE  37787
#define  AMETHYST  24985
#define  GRAYISH_PURPLE  33684
#define  HELIOTROPE  20503
#define  MINERAL_VIOLET  48409
#define  BLUE_VIOLET  35164
#define  VIOLET  34847
#define  INDIGO  18448
#define  DARK_ORCHID  39321
#define  DARK_VIOLET  36890
#define  PANSY  28692
#define  MALLOW  55903
#define  OPERA_MAUVE  58399
#define  MEDIUM_ORCHID  47770
#define  PAIL_LILAC  58972
#define  THISTLE  56795
#define  CLEMATIS  52505
#define  PLUM  56603
#define  LIGHT_VIOLET  60445
#define  PURPLE  32784
#define  DARK_MAGENTA  34833
#define  MAGENTA  63519
#define  FUCHSIA  61460
#define  ORCHID  56218
#define  PEARL_PINK  64924
#define  OLD_ROSE  47763
#define  ROSE_PINK  64281
#define  MEDIUM_VIOLET_RED  49296
#define  MAGENTA_ROSE  63572
#define  ROSE  63503
#define  RUBY  51216
#define  CAMELLIA  57810
#define  DEEP_PINK  63634
#define  FLAMINGO  58455
#define  CORAL_PINK  64535
#define  HOT_PINK  64342
#define  BURGUNDY  16388
#define  SPINEL_RED  64406
#define  CARMINE  57355
#define  BABY_PINK  65244
#define  CARDINAL_RED  38918
#define  LAVENDER_BLUSH  65438
#define  PALE_VIOLET_RED  56210
#define  CERISE  55692
#define  SALMON_PINK  64531
#define  CRIMSON  55431
#define  PINK  65049
#define  LIGHT_PINK  64920
#define  SHELL_PINK  64919
#define  ALIZARIN_CRIMSON  57606







/**************************************************
说明: LCD DMA控制数据格式
**************************************************/
#if (LCD_DMA_CLR_EN > 0) || (LCD_DMA_DAT_EN > 0)
extern DMA_ControlDat LcdDmaDat;
#endif

/*
*************************************************************************************
* 名    称：void LCD_SetCursor(LCD_CursorDat * Pos)
* 功    能：LCD光标位置设置
* 入口参数：LCD_CursorDat * Pos
* 出口参数：无
*************************************************************************************
*/
extern void LCD_SetCursor(LCD_CursorDat * Pos);

/*
*************************************************************************************
* 名    称：void LCD_Clear(LCD_ColorDat Color)
* 功    能：LCD写屏幕
* 入口参数：LCD_ColorDat Color
* 出口参数：无
*************************************************************************************
*/
extern void LCD_Clear(LCD_ColorDat Color);

/*
*************************************************************************************
* 名    称：void FSMC_Config(FunctionalState NewState)
* 功    能：FSMC配置
* 入口参数：FunctionalState NewState
            ENABLE    开启
            DISABLE   关闭
* 出口参数：无
*************************************************************************************
*/
extern void LCD_Init(void);

/*
*************************************************************************************
* 名    称：Uint8 LCD_FillRect(LCD_FillDat * FillDat)
* 功    能：LCD屏幕矩形填充
* 入口参数：LCD_FillDat * DatP  填充数据参数指针
* 出口参数：ERROR  /  SUCCESS
*************************************************************************************
*/
extern Uint8 LCD_FillRect(LCD_FillDat * FillDat);


/*
*************************************************************************************
* 名    称：Uint8 LCD_DrawShape(LCD_DrawDat * DatP)
* 功    能：在给定绘图缓存中绘制图形  
* 入口参数：LCD_DrawDat * DatP  绘图参数指针
* 出口参数：Uint8 err           成功/失败
*************************************************************************************
*/
extern Uint8 LCD_DrawShape(LCD_DrawDat * DrawDat);

/*
*************************************************************************************
* 名    称：void LCD_WriteNum(NUM_DisBuf * Disp)
* 功    能：写ASIIC码到LCD，以逐行方式写。
* 入口参数：NUM_DisBuf * Disp 显示参数指针
* 出口参数：无
*************************************************************************************
*/
extern void LCD_WriteNum(NUM_DisBuf * Disp);


/*
*************************************************************************************
* 名    称：void LCD_WriteWord(WordDisBuf * Disp)
* 功    能：写汉字到LCD，以逐行方式写。
* 入口参数：WordDisBuf * Disp 显示参数指针
* 出口参数：无
*************************************************************************************
*/
extern void LCD_WriteWord(WordDisBuf * Disp);

/*
*************************************************************************************
* 名    称：Uint8 LCD_DrawCoordinate(CoordinateDat * Disp)
* 功    能：绘制坐标系
* 入口参数：CoordinateDat * Disp 绘制坐标控制数据
* 出口参数：无
*************************************************************************************
*/
extern Uint8 LCD_DrawCoordinate(CoordinateDat * Disp);




#endif
