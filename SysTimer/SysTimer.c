/* ----------------------------------------------------------------------------
 *              FreeRTOSϵͳ��ʱ����������
 * --------------------------------------------------------------------------*/
#include "SysTimer.h"

#ifdef NEEDTIME_VALID
  portTickType NeedTime;  //��ʱͳ�ƶγ����ִ��ʱ��
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
  else if(((portTickType)0xFFFFFF - TimerStart + Timer) < TimerCount){
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
  return (portTickType)0xFFFFFF - TimerStart + Timer;
}

