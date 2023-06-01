#ifndef __INPUT_H
#define __INPUT_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TRIGGER_LEVEL_HIGH      1       //按下为高电平
#define TRIGGER_LEVEL_LOW       0       //按下为低电平

typedef struct __KEY {

    uint32_t TimeThr[16];			    //按键长按时间阈值
    uint8_t  TrigLevel[16];			    //按键触发电平（高电平/低电平）
    uint8_t  Number;					//按键数量
	uint32_t TimeSpan[16];			    //按键长按时间间隔
    uint32_t ConstTimeThr[16];		    //按键长按阈值（值不变，用于恢复InputTimeThr的值）
    uint16_t Value;                     //按键值

} Key_Structure;


void Set_Key_Number(uint8_t number);
void Set_Key_Trigger_Level(uint8_t keyId, uint8_t keyLevel);
void Set_Key_LongPress_Threshold(uint8_t keyId, uint16_t thresholdValue);
void Set_Key_Repetition_Threshold(uint8_t keyId, uint16_t thresholdValue);


void Key_Driver(void (*pValue)(uint8_t, uint8_t));
void Key_Scan(uint16_t* keyStatus);

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif