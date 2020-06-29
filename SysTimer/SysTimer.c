/* ----------------------------------------------------------------------------
 *              FreeRTOSϵͳ��ʱ����������
 * --------------------------------------------------------------------------*/
#include "SysTimer.h"

#ifdef NEEDTIME_VALID
  portTickType NeedTime;  //��ʱͳ�ƶγ����ִ��ʱ��
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

//--------------------���һ����ʱ���Ƿ񵽺���---------------------
//#����0 : һ����ʱ��δ��
//#���ط�0 : һ����ʱ�䵽
signed char LoopTimerProIsOv(portTickType *pTimerStart,//�ϴ�ʱ��
                     portTickType TimerCount)  //ʱ������
{
  portTickType Timer = xTaskGetTickCount();  
  portTickType TimerStart = *pTimerStart;
  //���ʱ���Ƿ���
  if(Timer >= TimerStart){
    if((Timer - TimerStart) < TimerCount)
      return 0; //һ����û��
  }
  //�ػ�����
  else if(((portTickType)SYS_TIMER_OV - TimerStart + Timer) < TimerCount){
      return 0; //һ����û��
  }
  *pTimerStart = Timer;   //���µ�ǰʱ��
  return -1;
}

//--------------------ͳ�ƴ���������ʱ�����ں���---------------------
portTickType GetPeriodTick(portTickType *pTimerStart)//�ϴ�ʱ��
{
  portTickType Timer = xTaskGetTickCount();//�õ���ǰֵ
  portTickType TimerStart = *pTimerStart;  //�ϴ�ֵ
  *pTimerStart = Timer;//���浱ǰֵ
  if(Timer > TimerStart)
     return Timer - TimerStart;
  //�ػ���ʱ�� 
  return (portTickType)SYS_TIMER_OV - TimerStart + Timer;
}

