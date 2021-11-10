/*******************************************************************************

                         �¶��豸ģ��-ʵ��
��ʵ�ֶ�����Ӳ��
*******************************************************************************/
#include "Temp.h"
#include <string.h>

#include "Eeprom.h"
#include "InfoBase.h"

struct _Temp Temp;

/*******************************************************************************
                           �����Ϊ����ʵ��
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void Temp_Init(unsigned char IsInited) //�Ƿ��ѳ�ʼ��
{
  memset(&Temp, 0, sizeof(struct _Temp));
  if(!IsInited){//װ��Ĭ��
    Temp.Info.Zero = TEMP_DEFAULT_ZERO;
    Temp.Info.Gain = TEMP_DEFAULT_GAIN; 
    Eeprom_Wr(Temp_GetInfoBase(),
              &Temp.Info,
              sizeof(struct _TempInfo));
  }
  else{
    Eeprom_Rd(Temp_GetInfoBase(),
                &Temp.Info,
                sizeof(struct _TempInfo));
  }
}

//---------------------------------�����¶�ֵ-----------------------------------
//ͬ�ڵ����Ը����¶�,�����ڷ��ж������ڵ���
void Temp_Update(void) //ԭʼ�ź�(Ϊ��ʱ���ϸ��̶�ֵΪ��)
{
  unsigned short OrgSignal = Temp_cbGetCurTempSignal();
  //========================��ԭʼ�źŵõ��¶�ֵ============================
  unsigned char CurTemp;
  if(OrgSignal >= Temp.Info.Zero){//ֻ���������¶�
    //ԭʼ�ź�->�����
    unsigned long Data = OrgSignal - Temp.Info.Zero;
    //->X����
    Data *= Temp.Info.Gain;
    Data >>= (TEMP_GAIN_Q - 8); //Q8�����ڸ���У��ʱ���У׼����
    if(Data > 0xffff){//������ʱ
      CurTemp = 255;
      Temp.Flag |= TEMP_HI_OV_ERR;
    }
    else{
      CurTemp = Temp_cbAppendPro(Data); //����У��
      Temp.Flag &= ~(TEMP_HI_OV_ERR | TEMP_LOW_OV_ERR);
    }
  }
  else{//��������
    CurTemp = 0;
    Temp.Flag |= TEMP_LOW_OV_ERR;
  }
  //==========================�˲�Ԥ����==============================
  #ifdef SUPPORT_TEMP_FILETER //֧���˲�ʱ
    unsigned char *pFilterBuf = Temp.FilterBuf;
    //(��������Ч)�״�ʹ��ʱ,�����׸���Чֵ���
    if(*pFilterBuf == 0){
      unsigned char *pEndFilterBuf = pFilterBuf + SUPPORT_TEMP_FILETER;
      for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
        *pFilterBuf = CurTemp;
      return;
    }
    //���˲�����ʱ����׷�ӵ����
   if(Temp.FilterPos >= SUPPORT_TEMP_FILETER)
      Temp.FilterPos = 0;
    else Temp.FilterPos++;
    *(pFilterBuf + Temp.FilterPos) = CurTemp;
    Temp.FilterPos = 0; //TEMP_FILETER_AVERVGE_MIDʹ��
    Temp.Flag |= TEMP_UPDATE_LATER;//�Ժ���
  #else
    Temp.CurTemp = CurTemp; //ֱ�ӽ��
  #endif
}

//---------------------------�Ժ��������-------------------------------
//����Temp_Update()��һ���Ľ����е���(����32ms����)
#ifdef SUPPORT_TEMP_FILETER //֧���˲�ʱ
void Temp_UpdateLater(void)
{
  if(!(Temp.Flag & TEMP_UPDATE_LATER)) return;//������
  
  unsigned char *pFilterBuf = Temp.FilterBuf;  
  
  #ifdef SUPPORT_TEMP_FILETER_AVERVGE //��ƽ��ʱ: �ۼӳ����
    unsigned char *pEndFilterBuf = pFilterBuf + SUPPORT_TEMP_FILETER;
    unsigned short AddTemp = 0;
    for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
      AddTemp += *pFilterBuf;
    Temp.CurTemp = AddTemp / SUPPORT_TEMP_FILETER;
    Temp.Flag &= ~TEMP_UPDATE_LATER;//һ�δ������
    return;
  #endif //SUPPORT_TEMP_FILETER_AVERVGE
  
  #ifdef SUPPORT_TEMP_FILETER_AVERVGE_MID  //֧��ƽ����ȥ�������Сֵʱ
    if(Temp.FilterPos == 0){//�״γ�ʼ��
      memcpy(pFilterBuf, pFilterBuf + SUPPORT_TEMP_FILETER, SUPPORT_TEMP_FILETER * 2);
      Temp.FilterPos = SUPPORT_TEMP_FILETER;
    }
    //��С����ð������,�����Lost��������,����������� 
    pFilterBuf += SUPPORT_TEMP_FILETER;//ָ�����򻺳�    
    unsigned char End = Temp.FilterPos;  //��ִ�����ڷֳɶ�ݴ���
    unsigned char Lost = SUPPORT_TEMP_FILETER_AVERVGE_MID;//ǰ������
    if(End <= Lost){//�ѽ�����,��������ǰ�¶�
      unsigned char *pEndFilterBuf = pFilterBuf + SUPPORT_TEMP_FILETER - Lost;      
      pFilterBuf += Lost;
      unsigned short AddTemp = 0;
      for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
        AddTemp += *pFilterBuf;
      Temp.CurTemp = AddTemp / (SUPPORT_TEMP_FILETER - Lost * 2);
      Temp.Flag &= ~TEMP_UPDATE_LATER;//һ�δ������
      return;
    }
    unsigned char Start = End - ((SUPPORT_TEMP_FILETER - Lost * 2) / 4); //4һ��������������
    if(Start < Lost) Start = Lost;  
    //�ɺ���ǰ���ҵ���ǰ���ֵ���Ƶ������
    for(; End >= Start; End--){
      unsigned char MaxPos = 0;       //��ס�������ֵ����λ��
      unsigned char Max = *pFilterBuf;    
      for(unsigned char i = 1; i < End; i++){
        unsigned char Cur = *(pFilterBuf + i); 
        if(Cur > Max){//�����ֵ����,���¼�ס���ֵ����λ��
           Max = Cur;
           MaxPos = i;
        }
      }
      //�����һ��ֵ�����ֵλ��������
      *(pFilterBuf + MaxPos) = *(pFilterBuf + End - 1);
      *(pFilterBuf + End - 1) = Max;
    }
    Temp.FilterPos = End; //�´ν���λ��
  #endif //SUPPORT_TEMP_FILETER_AVERVGE_MID
}
#endif //SUPPORT_TEMP_FILETER //֧���˲�ʱ

//------------------------��ԭʼ�¶�ֵ�õ�ԭʼ�ź�ֵ----------------------------
//Ϊ�����¶�ʱ�����������ķ����㣬��Ҫ���ڱ궨
static unsigned char  _GetOrgSignal(unsigned char OrgTemp) //ԭʼ�¶�
{
  //->��X����
  unsigned long Data = (unsigned long )OrgTemp << TEMP_GAIN_Q;
  Data /= Temp.Info.Gain;
  //ԭʼ�ź�->���������ź�
  return Data + Temp.Info.Zero;
}

//----------------------------�¶�У׼����--------------------------------------
//0�궨�ɹ�������ʧ��!
signed char Temp_Calibration(unsigned char TargetTemp, //Ŀ��Ũ��ֵ
                              unsigned char IsGain)//������(����ǰ�����)����������
{
  //����궨ʱ��ֻ��һ��
  if(IsGain && !(Temp.Flag &TEMP_ADJ_1ST_FINAL)) return -1; 
  unsigned char OrgTemp = Temp_cbAntiAppendPro(TargetTemp);//��ǰ��ʵ�¶ȶ�Ӧԭ�¶�
  unsigned short CurSignal;
  if(Temp.Flag & (TEMP_LOW_OV_ERR | TEMP_HI_OV_ERR))//���ȶ����¶ȷ�ADΪ׼
    CurSignal = _GetOrgSignal(Temp_cbAntiAppendPro(Temp.CurTemp));
  else CurSignal = Temp_cbGetCurTempSignal();//�Ե�ǰADΪ׼(AD����ʱ������Ҫ������)

  if(IsGain){ //����궨ģʽʱ���������������ֵ
    unsigned short TempDiff;
    unsigned short PrvAdjSignal = Temp.PrvAdjSignal;
    if(Temp.PrvAdjTemp > OrgTemp){//�ȸ��º����
      TempDiff = Temp.PrvAdjTemp - OrgTemp;
      if(PrvAdjSignal < CurSignal) return -2; //�û�������������
      PrvAdjSignal = PrvAdjSignal - CurSignal;
    }
    else{//�ȵ��º����
      TempDiff =  OrgTemp - Temp.PrvAdjTemp;
      if(CurSignal < PrvAdjSignal) return -3; //�û�������������
      PrvAdjSignal = CurSignal - PrvAdjSignal;
    }
    if(TempDiff < TEMP_DIFF_TEPM_MIN)  return - 4; //�²�̫С,���ܱ�У
    if(PrvAdjSignal < TEMP_DIFF_SIGAL_MIN) return -5; //ԭʼ�ź����̫С,���ܱ�У
    
    //�õ�����  
    unsigned long Data = (unsigned long)TempDiff << TEMP_GAIN_Q;
    Data /= PrvAdjSignal;
    if(Data > 0xffff) Temp.Info.Gain = 0xffff; //���Ŵ�����
    else if(Data < 0x3f) Temp.Info.Gain = 0x3f; //��С�Ŵ�����
    else Temp.Info.Gain = Data;
  }
  //����궨�򵥵�궨���������ֵ
  unsigned long Data = (unsigned long)OrgTemp << TEMP_GAIN_Q;
  Data /= Temp.Info.Gain;//��ΪĿ���¶���Ҫ�����
  Temp.Info.Zero = (signed short)CurSignal - (signed short)Data;
  //�滻�ϴ�Ϊ��ǰ�Ա��´�ʹ��
  Temp.PrvAdjTemp = OrgTemp;
  Temp.PrvAdjSignal = CurSignal;
  Temp.Flag |= TEMP_ADJ_1ST_FINAL; //ֻҪ�ɹ�������һ������  
  //��󱣴�
  Eeprom_Wr(Temp_GetInfoBase(),
              &Temp.Info,
              sizeof(struct _TempInfo));
  return 0;
}





