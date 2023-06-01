#include "INPUT.h"

Key_Structure Key;

uint8_t  keySta[16]      = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t  keyBackup[16]   = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint32_t keyDownTime[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t  keyBuffer[16]   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*************************************************************************
*	函 数 名: Set_Key_Number
*	功能说明: 设置按键数量
*	形    参: number
*	返 回 值: 无
**************************************************************************/
void Set_Key_Number(uint8_t number)
{
    Key.Number = number;
}

/*************************************************************************
*	函 数 名: Set_Key_Trigger_Level
*	功能说明: 设置按键触发方式 0(按下为低电平) 1(按下为高电平)
*	形    参: keyId: 按键id keyLevel: 触发方式
*	返 回 值: 无
**************************************************************************/
void Set_Key_Trigger_Level(uint8_t keyId, uint8_t keyLevel)
{
    Key.TrigLevel[keyId] = keyLevel;
}

/*************************************************************************
*	函 数 名: Set_Key_LongPress_Threshold
*	功能说明: 设置按键长按时间阈值
*	形    参: keyId: 按键id thresholdValue: 时间阈值(ms)
*	返 回 值: 无
**************************************************************************/
void Set_Key_LongPress_Threshold(uint8_t keyId, uint16_t thresholdValue)
{
    Key.ConstTimeThr[keyId] = thresholdValue / 2;   
    Key.TimeThr[keyId]      = thresholdValue / 2;
}

/*************************************************************************
*	函 数 名: Set_KeySet_Key_Repetition_Threshold_LongPress_Threshold
*	功能说明: 设置按键长按时间间隔
*	形    参: keyId: 按键id thresholdValue: 时间间隔(ms)
*	返 回 值: 无
**************************************************************************/
void Set_Key_Repetition_Threshold(uint8_t keyId, uint16_t thresholdValue)
{
    Key.TimeSpan[keyId] = thresholdValue / 2;
}

/*************************************************************************
*	函 数 名: Key_Driver
*	功能说明: 按键函数，需要在while(1)中调用
*	形    参: (*pValue)(uint8_t, uint8_t) : 用户自定义功能的回调函数，
*										   返回按键id和按键类型（短按/长按）
*	返 回 值: 无
**************************************************************************/
void Key_Driver(void (*pValue)(uint8_t, uint8_t))
{
    for(uint8_t i = 0; i < Key.Number; i++)         //循环检测Number(Number <= 16)个按键
    {
        if(keyBackup[i] != keySta[i])               //检测按键动作
        {
            if(keyBackup[i] != 0)     		        //按键按下时执行动作
            {
                pValue(i,0);                        //短按
            }
            keyBackup[i] = keySta[i];               //刷新前一次的备份值
        }
        if(keyDownTime[i] > 0)
        {
            if(keyDownTime[i] >= Key.TimeThr[i])
            {
                pValue(i,1);			            //长按
                Key.TimeThr[i] += Key.TimeSpan[i];  //多次执行按键长按动作函数	
            }
        }
        else{
            Key.TimeThr[i] = Key.ConstTimeThr[i];
        }
    }
}

/*************************************************************************
*	函 数 名: Key_Scan
*	功能说明: 输入扫描函数，需要2ms定时器中断中调用
*	形    参: keyStatus 按键状态
*	返 回 值: 无
**************************************************************************/
void Key_Scan(uint16_t* keyStatus)
{
    uint16_t keyindex = 0x0001;
    Key.Value = 0x0000;

    for(int i = 0; i < Key.Number; i++){
        if(keyStatus[i] == Key.TrigLevel[i]){
            Key.Value |= keyindex << i;
        }
        else{
            Key.Value &= (keyindex << i) ^ 0xFFFF;
        }        
    }

    for(int i = 0; i < Key.Number; i++) {
        keyBuffer[i] = (keyBuffer[i] << 1) | (((Key.Value & (keyindex << i)) >> i) & 0x01);
    }

    /* 消抖后更新按键状态 */
    for(int j = 0; j < Key.Number; j++)
    {
        if(keyBuffer[j] == 0xFF)     //(按下状态)
        {   
            keySta[j] = 0;
            keyDownTime[j] ++;
        }
        else if(keyBuffer[j] == 0x00) //(松开状态)
        {   
            keySta[j] = 1;
            keyDownTime[j] = 0;
        }
    }
}