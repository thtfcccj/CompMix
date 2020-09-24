/* ----------------------------------------------------------------------------
 *              FreeRTOS系统定时器操作函数
 * --------------------------------------------------------------------------*/
#include "SysTimer.h"

#ifdef NEEDTIME_VALID
  portTickType NeedTime;  //临时统计段程序的执行时间
#endif

#if defined(SYSTICK_T24)
  #define SYS_TIMER_OV   SYSTICK_T24
#elif defined(SUPPORT_HC32)
  #define SYS_TIMER_OV   (0x10000 * 1000)  // base 1ms
#elif defined(SUPPORT_PIC)
  #define SYS_TIMER_OV   0xFFFFFF         //base 256us
#else
  #define SYS_TIMER_OV   0xFFFFFF         //base 256us
#endif

//--------------------检查一周期时间是否到函数---------------------
//#返回0 : 一周期时间未到
//#返回非0 : 一周期时间到
signed char LoopTimerProIsOv(portTickType *pTimerStart,//上次时间
                     portTickType TimerCount)  //时间总数
{
  portTickType Timer = xTaskGetTickCount();  
  portTickType TimerStart = *pTimerStart;
  //检查时间是否超限
  if(Timer >= TimerStart){
    if((Timer - TimerStart) < TimerCount)
      return 0; //一周期没到
  }
  //回环后检查
  else if(((portTickType)SYS_TIMER_OV - TimerStart + Timer) < TimerCount){
      return 0; //一周期没到
  }
  *pTimerStart = Timer;   //更新当前时间
  return -1;
}

//--------------------统计此周期所需时间周期函数---------------------
portTickType GetPeriodTick(portTickType *pTimerStart)//上次时间
{
  portTickType Timer = xTaskGetTickCount();//得到当前值
  portTickType TimerStart = *pTimerStart;  //上次值
  *pTimerStart = Timer;//保存当前值
  if(Timer > TimerStart)
     return Timer - TimerStart;
  //回环后时间 
  return (portTickType)SYS_TIMER_OV - TimerStart + Timer;
}

