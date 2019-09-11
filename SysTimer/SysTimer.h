/* ----------------------------------------------------------------------------
 *              ��ʱ����������
 * --------------------------------------------------------------------------*/

#ifndef _SYS_TIMER_H
#define _SYS_TIMER_H

//��ʱ��֧��
#include "SysTickUs.h"
#define xTaskGetTickCount() SysTickUs_Get()
#define portTickType TickType_t

//#define NEEDTIME_VALID  //��ʱͳ��ʱ����Ч

#ifdef NEEDTIME_VALID
  extern   portTickType NeedTime;  //��ʱͳ�ƶγ����ִ��ʱ��
  #define StartNeedTime() do{NeedTime = xTaskGetTickCount();}while(0)
  #define EndNeedTime() do{NeedTime = xTaskGetTickCount() - NeedTime;}while(0)    
#else
  #define StartNeedTime() do{}while(0)
  #define EndNeedTime() do{}while(0)
#endif

//--------------------���һ����ʱ���Ƿ񵽺���---------------------
signed char  LoopTimerProIsOv(portTickType *pTimerStart, //�ϴ�ʱ��
                              portTickType TimerCount);  //ʱ������

//--------------------ͳ�ƴ���������ʱ�����ں���---------------------
portTickType GetPeriodTick(portTickType *pTimerStart);   //�ϴ�ʱ��

//--------------------��ʱ���ֹ���λ����---------------------
#define LoopTimerReset(pTimerStart) \
  do{*pTimerStart = xTaskGetTickCount();}while(0)

//--------------------��ʱ���ֹ���λ��ISR�к���---------------------
#define LoopTimerResetFromISR(pTimerStart) (*pTimerStart = xTaskGetTickCountFromISR())


#endif

