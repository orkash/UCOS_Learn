UCOSII实验4:消息邮箱使用测试

 1. 实验目的：消息邮箱创建请求发送测试
 2. 实验现象：  KEY0按键按下，LED0,LED1没有变化
            	KEY1按键按下，LED1状态反转
				KEY2(WK_UP)按下，LED0,LED1状态反转
		  
用到的UCOSII函数
消息邮箱创建函数：  
OS_EVENT  *OSMboxCreate (void *msg)
请求消息邮箱函数:   
void  *OSMboxPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
向邮箱发送消息函数: 
INT8U  OSMboxPost (OS_EVENT *pevent, void *msg)//向等待任务表中高优先级的任务发送消息
广播邮箱消息函数：  
INT8U  OSMboxPostOpt (OS_EVENT *pevent, void *msg, INT8U opt)//向等待任务表中所有任务发送消息

OSTaskCreate()函数创建TaskStart任务，
在TaskStart任务5次调用ucos任务创建函数OSTaskCreate()创建5个任务：TaskLed，TaskLed1，TaskLCD，TaskKey，TaskTouch。同时创建消息邮箱Str_Box = OSMboxCreate ((void*)0);。
然后调用OSTaskSuspend()函数将TaskStart()任务挂起，因为在5个任务创建后，TaskStart任务该做的事情已经完毕，挂起任务。然后5个任务在开始执行执行；

TaskLed： 	如果收到消息1或3，那么LED0反转
TaskLed1：	如果收到消息2或3，那么LED1反转
TaskLCD:  	LCD上半部分一定区域颜色循环更换
TaskKey： 	每隔20ms扫描按键值 。
TaskTouch： 每隔2ms扫描触摸屏下半部分的触摸点，并显示在LCD上。也就是我们的触摸画板程序。

按键扫描任务中，如果KEY0被按下，那么将向消息邮箱Str_Box发送消息1，
 	   i=1;
	   OSMboxPost(Str_Box,&i); //发送消息1
 如果KEY1被按下，那么将发送消息2，
 	   i=2;
	   OSMboxPost(Str_Box,&i); //发送消息2
 如果KEY2(WK_UP)被按下，将向所有等待任务表中所有任务发送消息3，
 	   i=3;
	   OSMboxPostOpt(Str_Box,&i,OS_POST_OPT_BROADCAST); //向所有任务广播消息3
	   
我们可以看到，如果我们按下KEY0,因为TaskLed1的优先级别高于TaskLed，所以当两个任务都在等待的时候，只有TaskLed1可以收到消息1，所以两个LED都不反转。
如果按下KEY1,那么TaskLed1收到消息2，状态反转。
如果按下KEY2，那么TaskLed1和TaskLed都会收到消息，这个时候两个LED状态都会反转。

OSMboxPostOpt()和OSMboxPost()的区别在于前者是广播消息，所有等待任务都可以收到，后者只会高优先级的任务收到。


注意:
1,本代码可以SWD下载,但是不支持仿真,下载完后,请拔了仿真器.否则可能导致部分功能不正常.


	       购买：http://shop62103354.taobao.com
                     http://shop62057469.taobao.com
               技术支持论坛：www.openedv.com
               ALIENTEK//广州星翼电子科技有限公司
