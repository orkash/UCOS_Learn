UCOSII实验2：UCOSII任务的挂起，恢复和删除测试

1. 实验目的：测试UCOSII的任务的挂起，恢复和删除
2. 实现现象：开机之后，LED0,LED1闪烁，LCD上半部分部分区域循环变色，触摸屏下半部分具有触摸画板功能。
          按下KEY0按键，LED0停止闪烁，LCD停止循环变色
		  按下KEY1按键，LED0恢复闪烁，LCD恢复循环变色
		  按下KEY2(WK_UP)按键，LED0停止闪烁，LCD停止循环变色。这个时候再按下KEY1按键将无法恢复，因为任务已经进入睡眠状态，不被调度。
3. 用到的UCOSII函数简析：
这里对于之前实验讲解过的函数我们不重复讲解了，只讲解新用到的函数。

任务挂起函数：	INT8U  OSTaskSuspend (INT8U prio)；将优先级别为prio的任务挂起，挂起任务就是停止任务的运行，并触发一次调度。
任务恢复函数：		INT8U  OSTaskResume (INT8U prio)；将优先级为prio的任务恢复，
					恢复任务就是让挂起的任务进入就绪状态，并触发一次调度。
任务请求删除函数：  INT8U  OSTaskDelReq (INT8U prio)；请求删除优先级别prio的任务。
任务删除函数：      INT8U  OSTaskDel (INT8U prio)；删除优先级为prio的任务，
 					删除任务之后，任务身份吊销了，没法再运行了。

这里我们可以看到这里关于任务操作的函数的入口参数都只有一个优先级,因为优先级在UCOSII里面对于每个任务是唯一的，所以可以用来识别任务。

4. 实验描述
OSTaskCreate()函数创建TaskStart任务，
在TaskStart任务5次调用ucos任务创建函数OSTaskCreate（）创建5个任务：TaskLed，TaskLed1，TaskLCD，TaskKey，TaskTouch。然后调用OSTaskSuspend()函数将
TaskStart()任务挂起，因为在5个任务创建后，TaskStart任务该做的事情已经完毕，挂起任务。然后5个任务在循环执行；

TaskLed： 	LED0每隔500ms状态反转
TaskLed1：	LED1每隔200ms状态反转
TaskLCD:  	LCD上半部分一定区域颜色循环更换
TaskKey： 	每隔20ms扫描按键值
TaskTouch： 每隔2ms扫描触摸屏下半部分的触摸点，并显示在LCD上。也就是我们的触摸画板程序。

在任务TaskKey中，我们循环扫描键值，如果KEY0按下，那么调用
		OSTaskSuspend(LED_TASK_Prio);
		OSTaskSuspend(LCD_TASK_Prio);
将任务TaskLed和任务TaskLCD挂起，这个时候，任务将不在执行（LED0停止闪烁，LCD停止循环显示），处于等待状态直到在其他任务中调用解挂函数OSTaskResume()将任务解挂。

如果KEY1按键，将调用函数：
	    OSTaskResume(LED_TASK_Prio);
		OSTaskResume(LCD_TASK_Prio);
将任务TaskLed和任务TaskLCD恢复，这个时候，任务将重新开始进入就绪状态，并引发一次任务调度。我们便可以看到LED0恢复闪烁，LCD恢复循环显示。

如果KEY2按键按下，那么将调用函数：
        OSTaskDelReq(LED_TASK_Prio);
		OSTaskDelReq(LCD_TASK_Prio);
请求将任务TaskLed和TaskLCD删除，记住，这里只是请求而不是删除。那么在任务TaskLed和TaskLCD执行的时候，将同时调用这个方法判断是否有任务删除请求，如果有那么将执行删除操作：
	    if(OSTaskDelReq(OS_PRIO_SELF)==OS_TASK_DEL_REQ) 
		 OSTaskDel(OS_PRIO_SELF);
		 
也就是说，删除任务是分两步来执行，第一步为请求删除任务，第二步才是删除任务。这样做的好处是在系统设计的时候避免直接删除导致任务有些资源没有释放而导致系统运行不正常。

删除任务之后，任务TaskLed和TaskLCD将处于睡眠状态，将不会被系统调度。这个时候可以看到LED0不再闪烁，LCD也不会循环显示。


注意:
1,本代码可以SWD下载,但是不支持仿真,下载完后,请拔了仿真器.否则可能导致部分功能不正常.


	       购买：http://shop62103354.taobao.com
                     http://shop62057469.taobao.com
               技术支持论坛：www.openedv.com
               ALIENTEK//广州星翼电子科技有限公司