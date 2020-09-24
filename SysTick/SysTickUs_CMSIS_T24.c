/******************************************************************
                  ϵͳUs��Tickģ��-��CMSIS��ֱ����24λSysTickTimerʵ��
ʹ��systimerӲ������CMSIS��׼��ʵ��
********************************************************************/
#include "SysTickUs.h"
#include "CMSIS.h"
//ע��SYS_MHZΪȫ�ֶ���
//    ��SysTimer���ʱ���趨��SYSTICK_T24ֵ

//---------------------ʱ����ʱ��0��ʼ������------------------------
void SysTickUs_Init()
{
  SysTick_Config(0x1000000); //24bit��
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;//���ж� 
}

//---------------------����������------------------------
//�ɷſ��ٽ����У�����ʱ�����з�ֹ���ʱ��
void SysTickUs_FastTask(void)
{

}

//------------------�õ���ʱ����ֵ--------------------------------
TickType_t SysTickUs_Get(void)
{
  return (0x1000000 - SysTick->VAL) / SYS_MHZ; 
}
