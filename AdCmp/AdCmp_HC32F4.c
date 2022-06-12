/******************************************************************************

                   AD(Ӳ��)�Ƚ�ģ��-��HC32F4�е�ʵ��
//��֧������A����ɨ��ģʽ,���ֻ֧�����16ͨ���ıȽ�
******************************************************************************/
#include "AdCmp.h"
#include "IoCtrl.h"
#include "misc.h"

/***************************************************************************
                           Ӳ���������˵��������
****************************************************************************/

//����AD�ȽϿ�����(AC = AdCompare)
#ifndef AC
  #define AC      M4_ADC2
#endif

//����Io,ͬʱ�躬ʱ��ʹ��,������ʵ��ͨ����ӳ���ϵ
//#define AdCmp_cbCfgIo() do{}while(0)
//�õ��õ�CR1����: ��ƽ������(b8:10),ADλ��(b4:5)���뷽ʽ(�Ҷ���b7=0)
//#define AdCmp_cbGetCR0() do{}while(0)
//�����ж�
//#define AdCmp_cbCfgInt() do{}while(0)
//ָ���жϴ�������
//#define IrqAdCmp_Handler  IRQ008_Handler

static unsigned char _ChMax = 0; //���ͨ��
/*********************************************************************
                        ��غ���ʵ��
*********************************************************************/

//-----------------------------��ʼ��-----------------------------
void AdCmp_Init(void)
{
  //����Io,ͬʱ�躬ʱ��ʹ��,������ʵ��ͨ����ӳ���ϵ
  AdCmp_cbCfgIo();
  AC->STR = 0;  //��ֹͣ
  AC->CR0 = AdCmp_cbGetCR0() | 1; //����Ϊ����A����ɨ��ģʽ
  AC->CHSELRA0 = 0;
  AC->AVCHSELR0 = 0;
  AC->AWDCHSR0 = 0;
  AdCmp_cbCfgInt(); //�����ж�
}

//--------------------------���ñȽ�����--------------------------
void AdCmp_Condition(unsigned short lAD,   //�Ͷ˱Ƚ�ADֵ
                     unsigned short hAD,  //�߶˱Ƚ�ADֵ
                     signed char Condition)//����
{
  AC->AWDDR0 = lAD;    
  AC->AWDDR1 = hAD;
  //b8=1ʹ���жϣ�b7:6=1����A���B����,
  //B4=1��AWDDR0�Qת������QAWDDR1ʱ���Ƚ���������
  if(Condition)
    AC->AWDCR = ((unsigned short)1 << 8) | (1 << 6) | (1 << 4);
  else 
    AC->AWDCR = ((unsigned short)1 << 8) | (1 << 6);
}

//-----------------------������Ƚϵ�ADͨ��--------------------
//����ֹͣ�ɼ�ʱ����
void AdCmp_OpenCh(unsigned char Ch)
{
  if(Ch > _ChMax) _ChMax = Ch;  
  //����ֻ֧��ǰ16��ͨ��  
  unsigned short Mask = (unsigned short)1 << Ch;
  AC->CHSELRA0 |= Mask;
  AC->AVCHSELR0 |= Mask; 
  AC->AWDCHSR0 |= Mask; 
}

//-----------------------�ر���Ƚϵ�ADͨ��--------------------
//����ֹͣ�ɼ�ʱ�ر�
void AdCmp_CloseCh(unsigned char Ch)
{
  unsigned short Mask = (unsigned short)1 << Ch;
  AC->CHSELRA0 &= ~Mask;
  AC->AVCHSELR0 &= ~Mask;  
  AC->AWDCHSR0 &= ~Mask; 
}

//--------------------------�����Ƚ�----------------------------
//����ͨ�����������
void AdCmp_Start(void)
{
  AC->AWDCR_f.AWDEN = 1;//���Ź��ȽϹ���ʹ��
  AC->STR = 1;  //����
}

//-------------------------ֹͣ�Ƚ�-----------------------------
void AdCmp_Stop(void)
{
  AC->STR = 0;  //�ر�  
  AC->AWDCR_f.AWDEN = 0;//��ֹ
}

/*********************************************************************
                        �жϴ�����ʵ��
*********************************************************************/
void IrqAdCmp_Handler(void)
{
  //��ȡ�ж�״̬
  unsigned short Mask = AC->AWDSR0;
  //���Ĵ�����λ����Ҫ����Ĵ���ʱ�������1����д��0����
  AC->AWDSR0 = 0;
  AdCmp_IRQ(Mask);
  
  //������Ǹ�ADͨ���ж��˳���ʱ���ص�
  //for(unsigned char Ch = 0; Ch < _ChMax; Ch++){
  //  if(Mask & ((unsigned short)1 << Ch))
  //     AdCmp_Int(Ch);
  //}
}



