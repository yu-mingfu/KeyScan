#include "key.h"
#include "INPUT.h"

hw_timer_t *timer;  

/* 定时器中断函数*/
void IRAM_ATTR onTimer() 
{   
    uint16_t key[16];

    key[0] = digitalRead(KEY1);
    key[1] = digitalRead(KEY2);
    key[2] = digitalRead(BOOT);
    Key_Scan(key);
}

/* 按键初始化 */
void key_init()
{
    pinMode(KEY1, INPUT_PULLUP);
    pinMode(KEY2, INPUT_PULLUP);
    pinMode(BOOT, INPUT_PULLUP);

    Set_Key_Number(3);                             //设置按键数量
    
    Set_Key_Trigger_Level(0,TRIGGER_LEVEL_LOW);    //设置触发电平
    Set_Key_LongPress_Threshold(0,1000);           //设置长按触发阈值(ms)
    Set_Key_Repetition_Threshold(0,500);           //设置长按重复阈值(ms)

    Set_Key_Trigger_Level(1,TRIGGER_LEVEL_LOW);
    Set_Key_LongPress_Threshold(1,300);
    Set_Key_Repetition_Threshold(1,200);
    
    Set_Key_Trigger_Level(2,TRIGGER_LEVEL_LOW);
    Set_Key_LongPress_Threshold(2,300);
    Set_Key_Repetition_Threshold(2,200);
    
    
    timer = timerBegin(0, 80, true);                //定时器初始化
    timerAttachInterrupt(timer, &onTimer, true);    //调用中断函数
    timerAlarmWrite(timer, 2000, true);             //timerBegin的参数二 80位80MHZ，这里为1000 意思为2毫秒
    timerAlarmEnable(timer);                        //定时器使能
}