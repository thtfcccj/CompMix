/*****************************************************************************

		                 在EEPROM内实现的小时累加模块实现

*****************************************************************************/

#include "HourIncEeprom.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include <string.h>

struct _HourIncEeprom HourIncEeprom;          //直接实例化

static const struct _HourIncEepromInfo _InfoDefault = {
  0,//小时计数值
  HOUR_INC_EEPROM_TO_HOUR_DEFAULT,    //到达1小时的Task计数值,用于校准小时精度
};

/*******************************************************************************
		                      相关函数实现
*******************************************************************************/

//---------------------------------初始化函数-----------------------------------
void HourIncEeprom_Init(unsigned char IsInited)
{
  memset(&HourIncEeprom, 0, sizeof(struct _HourIncEeprom));
  //先预读
  Eeprom_Rd(HourIncEeprom_GetInfoBase(),
                &HourIncEeprom,
                sizeof(struct _HourIncEepromInfo));
  HourIncEeprom.HourCalibrationOV = 0xffff;//校准用
  if(!IsInited || (HourIncEeprom.Info.Hour >= 0xff000000)){//装载默认
    memcpy(&HourIncEeprom, &_InfoDefault, sizeof(struct _HourIncEepromInfo));
    Eeprom_Wr(HourIncEeprom_GetInfoBase(),
              &HourIncEeprom,
              sizeof(struct _HourIncEepromInfo));
  }
}

//---------------------------Tick任务函数-------------------------------
static  unsigned char _TickTimer;            //Tick计数
//每Tick(>= 0.5ms)调用一次，建议放在时基中断里
void HourIncEeprom_TickTask(void)
{
  _TickTimer++;
  if(_TickTimer) return;
  //_TickTimer计数到0， 一个周期到了
  HourIncEeprom.ToHourTimer++;
  if(HourIncEeprom.HourCalibrationOV) HourIncEeprom.HourCalibrationOV--;
}

//----------------------------128Tick任务函数----------------------------
//放入128Tick ,用于保存Flash，返回0：小时未到，1：小时到了
signed char HourIncEeprom_128TickTask(void)
{
  if(HourIncEeprom.ToHourTimer < HourIncEeprom.Info.ToHourCount) return 0;
  HourIncEeprom.ToHourTimer = 0;
  //1小时到了，保存处理
  HourIncEeprom.Info.Hour++;
  Eeprom_Wr(HourIncEeprom_GetInfoBase(), &HourIncEeprom.Info.Hour, 4); //存储前4个数
  return 1;
}

//---------------------------------小时校准----------------------------------
//自开机或上次校准到1小时时，调用此函数校准小时
void HourIncEeprom_HourCalibration(void)
{
  unsigned short ToHourTimer  = 0xffff - HourIncEeprom.HourCalibrationOV;
  //值太小，时间不够  
  if(ToHourTimer < (HOUR_INC_EEPROM_TO_HOUR_DEFAULT / 60)) return;
  HourIncEeprom.Info.ToHourCount = ToHourTimer;
  Eeprom_Wr(HourIncEeprom_GetInfoBase(),
           &HourIncEeprom,
           sizeof(struct _HourIncEepromInfo));
  HourIncEeprom.HourCalibrationOV = 0xffff;//可重新执行校准
  HourIncEeprom.ToHourTimer = ToHourTimer;//任务时执行加1小时
}

//---------------------------------模块恢复至0时间---------------------------
void HourIncEeprom_ResetToHour0(void)
{
  HourIncEeprom.Info.Hour = 0;
  Eeprom_Wr(HourIncEeprom_GetInfoBase(), &HourIncEeprom.Info.Hour, 4); //存储前4个数
}

//-----------------------------得到累加小时数------------------------------
//AbsHour为保存的绝对时间，为0时可获得此模块当前小时点
//负值表示保存的绝对时间比当前小时点要早
signed long HourIncEeprom_GetAddHour(unsigned long AbsHour)
{
  return HourIncEeprom.Info.Hour - AbsHour;
}

//----------------------------------得到小时中的秒数--------------------------
unsigned short HourIncEeprom_GetSecInHour(void)
{
  return ((unsigned long)HourIncEeprom.ToHourTimer * 3600) / 
          HourIncEeprom.Info.ToHourCount;
}


