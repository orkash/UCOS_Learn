UCOSIIʵ��4:��Ϣ����ʹ�ò���

 1. ʵ��Ŀ�ģ���Ϣ���䴴�������Ͳ���
 2. ʵ������  KEY0�������£�LED0,LED1û�б仯
            	KEY1�������£�LED1״̬��ת
				KEY2(WK_UP)���£�LED0,LED1״̬��ת
		  
�õ���UCOSII����
��Ϣ���䴴��������  
OS_EVENT  *OSMboxCreate (void *msg)
������Ϣ���亯��:   
void  *OSMboxPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
�����䷢����Ϣ����: 
INT8U  OSMboxPost (OS_EVENT *pevent, void *msg)//��ȴ�������и����ȼ�����������Ϣ
�㲥������Ϣ������  
INT8U  OSMboxPostOpt (OS_EVENT *pevent, void *msg, INT8U opt)//��ȴ��������������������Ϣ

OSTaskCreate()��������TaskStart����
��TaskStart����5�ε���ucos���񴴽�����OSTaskCreate()����5������TaskLed��TaskLed1��TaskLCD��TaskKey��TaskTouch��ͬʱ������Ϣ����Str_Box = OSMboxCreate ((void*)0);��
Ȼ�����OSTaskSuspend()������TaskStart()���������Ϊ��5�����񴴽���TaskStart��������������Ѿ���ϣ���������Ȼ��5�������ڿ�ʼִ��ִ�У�

TaskLed�� 	����յ���Ϣ1��3����ôLED0��ת
TaskLed1��	����յ���Ϣ2��3����ôLED1��ת
TaskLCD:  	LCD�ϰ벿��һ��������ɫѭ������
TaskKey�� 	ÿ��20msɨ�谴��ֵ ��
TaskTouch�� ÿ��2msɨ�败�����°벿�ֵĴ����㣬����ʾ��LCD�ϡ�Ҳ�������ǵĴ����������

����ɨ�������У����KEY0�����£���ô������Ϣ����Str_Box������Ϣ1��
 	   i=1;
	   OSMboxPost(Str_Box,&i); //������Ϣ1
 ���KEY1�����£���ô��������Ϣ2��
 	   i=2;
	   OSMboxPost(Str_Box,&i); //������Ϣ2
 ���KEY2(WK_UP)�����£��������еȴ��������������������Ϣ3��
 	   i=3;
	   OSMboxPostOpt(Str_Box,&i,OS_POST_OPT_BROADCAST); //����������㲥��Ϣ3
	   
���ǿ��Կ�����������ǰ���KEY0,��ΪTaskLed1�����ȼ������TaskLed�����Ե����������ڵȴ���ʱ��ֻ��TaskLed1�����յ���Ϣ1����������LED������ת��
�������KEY1,��ôTaskLed1�յ���Ϣ2��״̬��ת��
�������KEY2����ôTaskLed1��TaskLed�����յ���Ϣ�����ʱ������LED״̬���ᷴת��

OSMboxPostOpt()��OSMboxPost()����������ǰ���ǹ㲥��Ϣ�����еȴ����񶼿����յ�������ֻ������ȼ��������յ���


ע��:
1,���������SWD����,���ǲ�֧�ַ���,�������,����˷�����.������ܵ��²��ֹ��ܲ�����.


	       ����http://shop62103354.taobao.com
                     http://shop62057469.taobao.com
               ����֧����̳��www.openedv.com
               ALIENTEK//����������ӿƼ����޹�˾
