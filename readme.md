## 函数说明

该按键扫描函数支持**独立按键**的扫描，默认最大支持16个按键，可以实现每个按键的短按、长按、长按重复触发，配合定时器还可以实现按键的双击。提供的函数如下（INPUT.c 、INPUT.h）:

| 函数                                                         | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| void Set_Key_Number(uint8_t number);                         | 设置按键数量                                                 |
| void Set_Key_Trigger_Level(uint8_t keyId, uint8_t keyLevel); | 设置按键触发电平   0(按下为低电平) 1(按下为高电平)           |
| void Set_Key_LongPress_Threshold(uint8_t keyId, uint16_t thresholdValue); | 设置按键长按时间阈值   keyId: 按键id thresholdValue: 时间阈值(ms) |
| void Set_Key_Repetition_Threshold(uint8_t keyId, uint16_t thresholdValue); | 设置按键长按时间间隔   keyId: 按键id thresholdValue: 时间间隔(ms) |
| void Key_Driver(void (*pValue)(uint8_t, uint8_t));           | 按键函数，需要在循环中调用                            用户自定义功能的回调函数，返回id 和按键类型 0 (短按)   1(长按) |
| void Key_Scan(uint16_t* keyStatus);                          | 输入扫描函数，需要2ms定时器中断中调用   keyStatus按键电平值  |

## 使用示例

### key.cpp

``` c++
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
```

### key.h

``` c++
#ifndef __KEY_H
#define __KEY_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define     KEY1    16
#define     KEY2    4
#define     BOOT    0
void key_init();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
```

### main.cpp

``` c++
#include <Arduino.h>
#include "INPUT.h"
#include "key.h"


void setup() {
  key_init();
  Serial.begin(115200);
  Serial.println("esp32 init");
}

void callback(uint8_t id, uint8_t type)
{
  Serial.printf("key_id = %d  pressed_type = %d\r\n",id,type);
}


void loop() {
  Key_Driver(callback);
}
```



