/*****************************************************************************

		                    在EEPROM内实现的小时累加模块
此模块主要用于在EEPROM(非FLASH模拟)中存储小时时间
EEPROM的寿命为10万次~100万次，以0万次计算，第1小时保存一次，则可确保最少
//100000 / 365 / 24 = 11.4年数据的正确性！
*****************************************************************************/
#ifndef _HOUR_INC_EEPROM_H
#define	_HOUR_INC_EEPROM_H

/********************************************************************************
		                      相关配置
********************************************************************************/

//定义1h计数默认值(256Tick为单位 )，以提高小时精度
#ifndef HOUR_INC_EEPROM_TO_HOUR_DEFAULT  
  #define HOUR_INC_EEPROM_TO_HOUR_DEFAULT   14062  //无误差时为14062    
#endif

/********************************************************************************
		                      相关结构
********************************************************************************/

//信息结构
struct _HourIncEepromInfo{
  unsigned short ToHourCount;    //到达1小时的Task计数值,用于校准小时精度
  unsigned long Hour;            //小时计数值
};

//主结构
struct _HourIncEeprom{
  struct _HourIncEepromInfo Info;
  
  //小时内RAM计数:
  unsigned short ToHourTimer; //到1小时时计数值
  unsigned short HourCalibrationOV;//1小时校准专用(因ToHourTimer会提前结束故不能用)
};

extern struct _HourIncEeprom HourIncEeprom;          //直接实例化

/*******************************************************************************
		                      相关行为函数
*******************************************************************************/

//------------------------------初始化函数---------------------------------
void HourIncEeprom_Init(unsigned char IsInited);

//------------------------------Tick任务函数-------------------------------
//每Tick(>= 0.5ms)调用一次，建议放在时基中断里
void HourIncEeprom_TickTask(void);

//----------------------------128Tick任务函数------------------------------
//放入128Tick ,用于保存Flash，返回0：小时未到，1：小时到了
signed char HourIncEeprom_128TickTask(void);

//---------------------------------小时校准----------------------------------
//自开机或上次校准到1小时，调用此函数校准小时
void HourIncEeprom_HourCalibration(void);

//---------------------------------模块恢复至0时间---------------------------
void HourIncEeprom_ResetToHour0(void);

/*******************************************************************************
		                      相关属性函数
*******************************************************************************/

//--------------------------------得到累加小时数-------------------------------
//AbsHour为保存的绝对时间，为0时可获得此模块当前小时点
//负值表示保存的绝对时间比当前小时点要早
signed long HourIncEeprom_GetAddHour(unsigned long AbsHour);

//----------------------------得到累加小时中天数------------------------------
#define HourIncEeprom_GetDay(absHour)  (HourIncEeprom_GetAddHour(absHour) / 24)

//----------------------------得到累加小时中的小时数---------------------------
#define HourIncEeprom_GetHour(absHour)  (HourIncEeprom_GetAddHour(absHour) % 24)

//----------------------------------得到小时中的秒数--------------------------
unsigned short HourIncEeprom_GetSecInHour(void);

//---------------------------得到小时中的分钟数-----------------------------
#define HourIncEeprom_GetMinue()  (HourIncEeprom_GetSecInHour() / 60)

//----------------------------------得到分中的秒数--------------------------
#define HourIncEeprom_GetSec()  (HourIncEeprom_GetSecInHour() % 60)

//----------------------------------得到Tick计数----------------------------
#define HourIncEeprom_GetTick()   (HourIncEeprom.TickTimer)

#endif
