/* ----------------------------------------------------------------------------
 *              定时器操作函数
 * --------------------------------------------------------------------------*/

#ifndef _SYS_TIMER_H
#define _SYS_TIMER_H

//定时器支持
#include "SysTickUs.h"
#define xTaskGetTickCount() SysTickUs_Get()
#define portTickType TickType_t

//#define NEEDTIME_VALID  //定时统计时间有效

#ifdef NEEDTIME_VALID
  extern   portTickType NeedTime;  //临时统计段程序的执行时间
  #define StartNeedTime() do{NeedTime = xTaskGetTickCount();}while(0)
  #define EndNeedTime() do{NeedTime = xTaskGetTickCount() - NeedTime;}while(0)    
#else
  #define StartNeedTime() do{}while(0)
  #define EndNeedTime() do{}while(0)
#endif

//--------------------检查一周期时间是否到函数---------------------
signed char  LoopTimerProIsOv(portTickType *pTimerStart, //上次时间
                              portTickType TimerCount);  //时间总数

//--------------------统计此周期所需时间周期函数---------------------
portTickType GetPeriodTick(portTickType *pTimerStart);   //上次时间

//--------------------定时器手工复位函数---------------------
#define LoopTimerReset(pTimerStart) \
  do{*pTimerStart = xTaskGetTickCount();}while(0)

//--------------------定时器手工复位在ISR中函数---------------------
#define LoopTimerResetFromISR(pTimerStart) (*pTimerStart = xTaskGetTickCountFromISR())


#endif

