/******************************************************************
                  ϵͳUs��Tickģ��-��CMSIS�е�ʵ��
ʹ��systimerӲ������CMSIS��׼��ʵ��
********************************************************************/
#include "SysTickUs.h"
#include "CMSIS.h"

static volatile unsigned short _IntCount;//��ʱ���жϼ���

extern void SysTick_cbHandlerNotify(void); //��ʱ��ͨ��,�ⲿʵ��

//---------------------ʱ����ʱ��0��ʼ������------------------------
void SysTickUs_Init()
{
  SysTick_Config(SystemCoreClock / 1000); //����Ϊ1msһ���ж�,�β�Ϊ���¼�ʱloadֵ 
}

//---------------------------cm3�ж�ʵ��--------------------------
void SysTick_Handler(void)
{
  _IntCount++;
  SysTick_cbHandlerNotify();
}

//------------------�õ���ʱ����ֵ--------------------------------
TickType_t SysTickUs_Get(void)
{
  //�ٽ�����������С�����ȿ���
  //SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Usk; //Enter Critcal
  TickType_t Us =  SysTick->VAL;
  //SysTick->CTRL |= SysTick_CTRL_TICKINT_Usk; //Exit Critcal
  
  //��������ϴζ�ȡ���Ĵ����� SysTick �Ѿ��Ƶ��� 0�����λΪ 1,�������һ��CTRL
  volatile uint32_t Dump = SysTick->CTRL;
  
  return ((TickType_t)_IntCount * 1000) + (Us / SYS_MHZ); 
}


//---------------------ʱ����ʱ����ѯ��ʽ------------------------
//�ſ��ٽ����У�����ʱ�����з�ֹ���ʱ��
//�ڴ˺�����,Ӳ��ʱ����ʱ
void SysTickUs_FastTask(void)
{
  if(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) return;
  _IntCount++;
  //�˳��ٽ���,��
}

