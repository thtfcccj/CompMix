/*********************************************************************

             ��ʱ������׼�ӿ�-��cm4f�е�ʵ��

**********************************************************************/

#include "Delay.h"

//ע��SYS_MHZΪȫ�ֶ���

//-----------------------΢����ʱ����---------------------------------
void DelayUs(unsigned short Us)
{
  for(; Us > 0; Us--){
    for(volatile int i = 0; i < (SYS_MHZ / 20); i++);
  }
}

//-----------------------������ʱ����---------------------------------
void DelayMs(unsigned short Ms)
{
  for(; Ms > 0; Ms--) DelayUs(1000);
}