/******************************************************************************

                     ��ͨ����ѹ�Ƚ���ģ��-��HC32�е�ʵ��

*******************************************************************************/
#include "MVC.h"

#include "HC32.h"
#include <String.h>

struct _MVC MVC;
//�����ɼĴ���ͨ�����ɼ�ͨ����Ĳ��ұ�
static const unsigned char _VcLut[VC_CHANNEL_GRP_COUNT][VC_CHANNEL_PER_COUNT] = 
  VC_CHANNEL_LUT;

//������ʼͨ��
static const unsigned char _ChBaseLut[AD_CHANNEL_COUNT] = VC_CH_BASE_LUT;

/***************************************************************************
                             ����ʵ��
****************************************************************************/


//-----------------------------------��ʼ��-----------------------------
void MVC_Init(void)
{  
  memset(&MVC, 0, sizeof(struct _MVC));
  memset(MVC.CurCh,0xff,VC_CHANNEL_GRP_COUNT);

  //1. MVCC ��BGRģ��ʱ��ʹ��
  M0P_SYSCTRL->PERI_CLKEN0_f.VC = 1;
  
  //2. ����IO��Ĵ���
  CfgVcIO();
  
  //������ʼ�ɼ�
  MVC_Task();
}

//----------------------------������------------------------------
//����ʵʱ��Ҫ�󣬷���<=0.5ms������ɨ�裬����ʱ�����ж���ɨ��
void MVC_Task(void)
{
  for(unsigned char Vc = 0; Vc < VC_CHANNEL_GRP_COUNT; Vc++){
    unsigned char Ch = MVC.CurCh[Vc];
    if(Ch >= VC_CHANNEL_PER_COUNT){//�״������ɼ�
      Ch = 0; 
    }
    else{//���н����
      //��ȡ�ȶ���Ľ��
      unsigned char IsHi;
      if(Vc == 0) IsHi = M0P_VC->IFR & (1 << 2);
      else if(Vc == 1) IsHi = M0P_VC->IFR & (1 << 3);
      else IsHi = M0P_VC->IFR & (1 << 5);
      if(_VcLut[Vc][Ch] & 0x80) IsHi = !IsHi;//��������ʱ����
      
      //���µ�ǰ״̬
      MvcChMask_t ChMask = 1 << (_ChBaseLut[Vc] + Ch);
      if(IsHi) MVC.CurState |= ChMask;
      else MVC.CurState &= ~ChMask;
      //ֻ���ϴ��뱾��״̬��ͬ�ˣ��Ÿ�����״̬
      if((MVC.CurState & ChMask) == (MVC.PrvState & ChMask)){
        if(IsHi) MVC.State |= ChMask;
        else MVC.State &= ~ChMask;
      }
      else{//����ͬ�������ϴ�״̬
        if(IsHi) MVC.PrvState |= ChMask;
        else MVC.PrvState &= ~ChMask;
      }
      //���������¸�ͨ��������
      Ch++;
      if((Ch >= VC_CHANNEL_PER_COUNT) || (_VcLut[Vc][Ch] == 0xff))
        Ch = 0;
    }
  
    //���������һͨ��
    MVC.CurCh[Vc] = Ch;
    
    unsigned char ChInfo = _VcLut[Vc][Ch];
    unsigned long Cr;
    if(ChInfo & 0x80)//��������
      Cr = ((unsigned long)(ChInfo & 0x7f) << 4) | GetVcCrCfgN();
    else //��������
      Cr = ChInfo | GetVcCrCfgP();
    if(Vc == 0) M0P_VC->VC0_CR = Cr;
    else if(Vc == 1) M0P_VC->VC1_CR = Cr;
    else M0P_VC->VC2_CR = Cr; 
    
  } //end for
}


