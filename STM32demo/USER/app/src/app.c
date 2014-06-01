#include "user_cfg.h"
#include "includes.h"#include "sys_about.h"
#include "task.h"

//任务堆栈
OS_STK  TASK_START_STK[START_STK_SIZE];

int main(void)
 {   
    SysInit();
    
    OSInit();    //UCOSII初始化
    OSTaskCreate( TaskStart,  //创建开始任务
                  (void *)0,  //parameter
                  (OS_STK *)&TASK_START_STK[START_STK_SIZE-1],  //task stack top pointer
                  START_TASK_Prio );  //task priority
    OSStart();  //UCOSII系统启动
    return 0;
}  
