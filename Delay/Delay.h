/***********************************************************

                ��׼��ʱģ��
��ģ����Ӧ���޹�
************************************************************/
#ifndef  _DELAY_H
#define  _DELAY_H  


//--------------------Us��ʱ����----------------------------
void Delay_Us(unsigned short DelayUs);
#define vDelayUs(delayus) Delay_Us(delayus) //Ϊ���ּ�����
#define DelayUs(delayus) Delay_Us(delayus) //Ϊ���ּ�����

//--------------------Ms��ʱ����----------------------------
void Delay_Ms(unsigned short DelayMs);
#define vDelayMs(delayms) Delay_Ms(delayms) //Ϊ���ּ�����
#define DelayMs(delayms) Delay_Ms(delayms) //Ϊ���ּ�����

#endif
