/*
*************************************************************************************
**文件说明：任务
**文件名称：task.c
**创建日期：2013.11.07
**创 建 人：王玮
**-----------------------------------------------------------------------------------
**修改日期：2013.xx.xx
**修改说明：
*************************************************************************************
*/
#include "user_cfg.h"
#include "user_type.h"

#include "drivers.h"
#include "task.h"
#include "lcd_tft.h"
#include "memory.h"
#include "word.h"
#include "guidemo.h"
#include "GUI.h"



#include "task_lcd.c"
#include "task_rtc.c"




/***************************************************
任务堆栈
****************************************************/
OS_STK  TASK_LED0_STK[LED_STK_SIZE];
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED2_STK[LED_STK_SIZE];


/***************************************************
任务声明
****************************************************/
void TaskLed0(void *pdata);
void TaskLed1(void *pdata);
void TaskLed2(void *pdata);



void SetFont(void)
{
    GUI_SetFont(&GUI_Font8x16);
    GUI_DispStringAt("This text is 8 by 16 pixels\n",0,30);
    GUI_SetFont(&GUI_Font6x8);
    GUI_DispStringAt("This text is 6 by 8 pixels\n",0,50);
    GUI_SetFont(&GUI_Font32B_ASCII);
    GUI_DispStringAt("This text is 48 by 64 pixels\n",0,90);
}

/***************************************************
开始任务
****************************************************/
void TaskStart(void * pdata)
{
    Uint8 err;
    pdata = pdata;
    OS_CPU_SR  cpu_sr;

    FSMCMutex  = OSMutexCreate(2,&err);                     //创建FSMC互斥型信号量
    MEMPointer = OSMemCreate(MEMPartition,PARTITION_NUM,PARTITION_LENGTH,&err);	    //创建内存分区

    OS_ENTER_CRITICAL();
//    OSTaskCreate(TaskLed0, (void * )0, (OS_STK *)&TASK_LED0_STK[LED_STK_SIZE-1], LED0_TASK_Prio);
//    OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED_STK_SIZE-1], LED1_TASK_Prio);
//    OSTaskCreate(TaskLed2, (void * )0, (OS_STK *)&TASK_LED2_STK[LED_STK_SIZE-1], LED2_TASK_Prio);

//    OSTaskCreate(TaskRTCUpdate, (void * )0,                           //创建RTC更新任务
//                 (OS_STK *)&TASK_RTC_DIS_UPDATE_STK[RTC_STK_SIZE-1],
//                 RTC_TASK_Prio);
    OS_EXIT_CRITICAL();

    //WM_SetCreateFlags(WM_CF_MEMDEV);

    // Init the STemWin GUI Library
    GUI_Init();

    GUI_DispString("Hello STemWin!\n");
    GUI_DispString("Hello STemWin!\n");

    SetFont();

    GUIDEMO_Main();
    OSTaskDel(OS_PRIO_SELF);      //开始任务完成，删除自身。
}

LCD_DrawDat  DrawDat0;
LCD_FillDat  FillDat0;
LCD_DrawDat  DrawDat3;
/***************************************************
任务1 控制DS0的亮灭.
****************************************************/
void TaskLed0(void *pdata)
{
    Uint8 Flag;
    Uint8 err;

    DrawDat0.StartPoint.X = 0;
    DrawDat0.StartPoint.Y = 0;
    DrawDat0.EndPoint.X   = 100;
    DrawDat0.EndPoint.Y   = 100;
    DrawDat0.LineColor    = RED;
    DrawDat0.Shape        = DARW_CLASS_LINE;

    DrawDat3.StartPoint.X = 0;
    DrawDat3.StartPoint.Y = 200;
    DrawDat3.EndPoint.X   = 100;
    DrawDat3.EndPoint.Y   = 100;
    DrawDat3.LineColor    = RED;
    DrawDat3.Shape        = DARW_CLASS_LINE;
    while(1){
        if(Flag & 0x01){
            LedRedOn();
        }
        else {
            LedRedOff();
        }
        Flag ++;
        OSMutexPend(FSMCMutex,0,&err);     //等待FSMC互斥信号量
        //LCD_Clear(RED);                   //使用FSMC

        //DrawDat0.StartPoint.X ++;
        //DrawDat0.StartPoint.Y ++;
        DrawDat0.EndPoint.X ++;
        DrawDat3.EndPoint.X ++;
        //DrawDat0.EndPoint.Y ++;

        if(DrawDat0.EndPoint.X > LCD_XSIZE || DrawDat0.EndPoint.Y > LCD_YSIZE){
            DrawDat0.StartPoint.X = 0;
            DrawDat0.StartPoint.Y = 0;
            DrawDat0.EndPoint.X   = 100;
            DrawDat0.EndPoint.Y   = 100;

            DrawDat3.StartPoint.X = 0;
            DrawDat3.StartPoint.Y = 200;
            DrawDat3.EndPoint.X   = 100;
            DrawDat3.EndPoint.Y   = 100;


            LCD_Clear(LCD_BACKGROUND_COLOR);
            OSMutexPost(FSMCMutex);            //释放FSMC互斥信号量
            OSTaskSuspend(OS_PRIO_SELF);       //suspend but not delete

//            FillDat0.StartPoint.X = 100;
//            FillDat0.StartPoint.Y = 100;
//            FillDat0.XSize = 200;
//            FillDat0.YSize = 200;
//            FillDat0.DatBuf = (Uint16 *)gImage_B;
//            LCD_FillRect(&FillDat0);
        }

        LCD_DrawShape(&DrawDat0);
        LCD_DrawShape(&DrawDat3);

        OSMutexPost(FSMCMutex);            //释放FSMC互斥信号量
        OSTimeDlyHMSM(0,0,0,20);
    }
}
LCD_DrawDat DrawDat1;
/***************************************************
任务2 控制DS1的亮灭.
****************************************************/
void TaskLed1(void *pdata)
{
    Uint8 Flag;
    Uint8 err;

    DrawDat1.StartPoint.X = 0;
    DrawDat1.StartPoint.Y = 240;
    DrawDat1.EndPoint.X   = 20;
    DrawDat1.EndPoint.Y   = 260;
    DrawDat1.LineColor    = RED;
    DrawDat1.Shape        = DARW_CLASS_RECT;

    while(1){
        if(Flag & 0x01){
            LedBlueOff();
        }
        else {
            LedBlueOn();
        }
        Flag ++;

        OSMutexPend(FSMCMutex,0,&err);     //等待FSMC互斥信号量
        //LCD_Clear(RED);                   //使用FSMC

        DrawDat1.StartPoint.X ++;
        //DrawDat1.StartPoint.Y ++;
        DrawDat1.EndPoint.X ++;
        //DrawDat1.EndPoint.Y ++;
        DrawDat1.LineColor    += 10;

        if(DrawDat1.EndPoint.X > LCD_XSIZE || DrawDat1.EndPoint.Y > LCD_YSIZE){
            DrawDat1.StartPoint.X = 0;
            DrawDat1.StartPoint.Y = 240;
            DrawDat1.EndPoint.X   = 20;
            DrawDat1.EndPoint.Y   = 250;
            DrawDat1.LineColor    = GREEN;
            LCD_Clear(LCD_BACKGROUND_COLOR);
            OSMutexPost(FSMCMutex);            //释放FSMC互斥信号量
            OSTaskSuspend(OS_PRIO_SELF);       //suspend but not delete
        }

        LCD_DrawShape(&DrawDat1);

        OSMutexPost(FSMCMutex);            //释放FSMC互斥信号量
        OSTimeDlyHMSM(0,0,0,30);
    }
}
/***************************************************
任务3 控制DS2的亮灭.
****************************************************/
LCD_DrawDat DrawDat2;
void TaskLed2(void *pdata)
{
    Uint8 Flag;
    Uint8 err;
    DrawDat2.StartPoint.X = 400;
    DrawDat2.StartPoint.Y = 240;
    DrawDat2.EndPoint.X   = 400;
    DrawDat2.EndPoint.Y   = 240;
    DrawDat2.LineColor    = COCONUT_BROWN;
    DrawDat2.Shape        = DARW_CLASS_CIRCLE;

    while(1){
        if(Flag & 0x01){
            LedGreenOff();
        }
        else {
            LedGreenOn();
        }
        Flag ++;

        OSMutexPend(FSMCMutex,0,&err);     //等待FSMC互斥信号量

        //DrawDat2.StartPoint.X ++;
        //DrawDat2.StartPoint.Y ++;
        //DrawDat2.EndPoint.X ++;
        DrawDat2.EndPoint.Y ++;
        DrawDat2.LineColor += 10;

        if(DrawDat2.EndPoint.X > LCD_XSIZE || DrawDat2.EndPoint.Y > LCD_YSIZE){
            DrawDat2.StartPoint.X = 400;
            DrawDat2.StartPoint.Y = 240;
            DrawDat2.EndPoint.X   = 400;
            DrawDat2.EndPoint.Y   = 240;
            //DrawDat2.LineColor    = RED;
            LCD_Clear(LCD_BACKGROUND_COLOR);
            OSMutexPost(FSMCMutex);            //释放FSMC互斥信号量


            OSTaskSuspend(OS_PRIO_SELF);       //suspend but not delete
            //OSTaskDel(OS_PRIO_SELF);
        }

        LCD_DrawShape(&DrawDat2);

        OSMutexPost(FSMCMutex);            //释放FSMC互斥信号量
        OSTimeDlyHMSM(0,0,0,30);
    }
}




