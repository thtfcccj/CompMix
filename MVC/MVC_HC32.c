/******************************************************************************

                     ��ͨ����ѹ�Ƚ���ģ��-��HC32�е�ʵ��

*******************************************************************************/
#include "MVC.h"

#include "HC32.h"
#include <String.h>

struct _MVC MVC;
//�����ɼĴ���ͨ�����ɼ�ͨ����Ĳ��ұ�
static const unsigned char _VcLut[VC_CHANNEL_GRP_COUNT][VC_CHANNEL_PER_COUNT + 1] = 
  VC_CHANNEL_LUT;

/***************************************************************************
                             ����ʵ��
****************************************************************************/


//-----------------------------------��ʼ��-----------------------------
void MVC_Init(void)
{  
  memset(&MVC,0xff,sizeof(struct _MVC));
  memset(MVC.CurCh,0xff,VC_CHANNEL_GRP_COUNT);
  MVC.PrvState = 0;
  MVC.CurState = 0;  

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
    if(Ch>= VC_CHANNEL_PER_COUNT){//�״������ɼ�
      Ch = 0; 
    }
    else{//���н����
      //��ȡ�ȶ���Ľ��
      unsigned char IsHi;
      if(Vc == 0) IsHi = M0P_VC->IFR & (1 << 2);
      else if(Vc == 1) IsHi = M0P_VC->IFR & (1 << 3);
      else IsHi = M0P_VC->IFR & (1 << 5);
      //���µ�ǰ״̬
      MvcChMask_t ChMask = 1 << (_VcLut[Vc][0] + Ch);
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
      //���������¸�ͨ��
      Ch++;
      if(_VcLut[Vc][1 + Ch] >= VC_CHANNEL_PER_COUNT) Ch = 0;
    }
  
    //�����������һͨ��
    MVC.CurCh[Ch] = Ch;
    unsigned long Cr = GetVcCrCfg() | _VcLut[Vc][1 + Ch];
    if(Vc == 0) M0P_VC->VC0_CR = Cr;
    else if(Vc == 1) M0P_VC->VC1_CR = Cr;
    else M0P_VC->VC2_CR = Cr; 
    
  } //end for
}


