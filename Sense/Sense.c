/*******************************************************************************

                         ��֪���źŴ���ģ��ʵ��

*******************************************************************************/
#include "Sense.h"
#include <string.h>

#include "Eeprom.h"
#include "math_3.h"

struct _Sense Sense;

/*******************************************************************************
                           ��غ���ʵ��
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void Sense_Init(struct _Sense *pSense,
                const struct _SenseDesc *pDesc,//��̬�����ṹ
                unsigned char IsInited) //�Ƿ��ѳ�ʼ��
{
  memset(pSense, 0, sizeof(struct _Sense));
  pSense->pDesc = pDesc;
  if(!IsInited){//װ��Ĭ��
    pSense->Info.Zero = pDesc->DefaultZero;
    pSense->Info.Gain = pDesc->DefaultGain; 
    Eeprom_Wr(pDesc->InfoBase,
              &pSense->Info,
              sizeof(struct _SenseInfo));
  }
  else{
    Eeprom_Rd(pDesc->InfoBase,
                &pSense->Info,
                sizeof(struct _SenseInfo));
  }
}

//------------------------------�����˲��㷨--------------------------------
//���ڳ�ʼ�������
void Sense_UpdateFilter(struct _Sense *pSense,
                        unsigned char FilterType, //�˲��㷨����,������
                        signed short *pFilterBuf, //�˲�������
                        unsigned short FilterSize,//�˲��������
                        unsigned short FilterPara)//��ͬ�˲��㷨ʱ������Ĳ���
{
  if(FilterType >= SENSE_FILTER_TYPE_COUNT) FilterType = 0;
  pSense->FilterType = FilterType;
  pSense->pFilterBuf = pFilterBuf;
  pSense->FilterSize = FilterSize;
  pSense->FilterPara = FilterPara; 
  pSense->FilterNewPos = 0;    //��0��ʼ��
  pSense->Flag &= ~SENSE_VOL_VALID;//������Ч
}

//-----------------------------����Ŀ��ֵ-----------------------------------
//���ڵ����Ը����¶�,
//��������ȥ��ֵ����Ҫ������˲��㷨ʱ��Ӧ�ڷ��ж��е���
void Sense_Update(struct _Sense *pSense,
                  signed short OrgSignal) //ԭʼ�ź�(������)
{
  const struct _SenseDesc *pDesc = pSense->pDesc;
  //��Ũ��ֵ��ת��Ϊ�����������
  signed short CurVol = Line_GetYInZeroMutiSS(pSense->Info.Zero + 32767, //ֱ�ߴ���y���xֵ
                                     pSense->Info.Gain,//б��
                                     pDesc->GainQ,//б�ʵ�Qֵ
                                     OrgSignal + 32767);  //δ֪y���x����
  if(pDesc->NlLut != NULL) //����ת��
    CurVol = pDesc->NlLut(CurVol);
  
  //���˲�, ֱ�ӵý��
  if(pSense->FilterType == SENSE_FILTER_TYPE_NONE){
    pSense->Vol = CurVol;
    return;
  }
  signed short *pFilterBuf = pSense->pFilterBuf;  
  unsigned short FilterSize = pSense->FilterSize;
  //������Чʱ�������ʼ��
  if(!(pSense->Flag & SENSE_VOL_VALID)){
    memset2((unsigned short*)pFilterBuf, 
            (unsigned short)CurVol, 
            FilterSize);
    pSense->Flag |= SENSE_VOL_VALID;
    pSense->Vol = CurVol;
    return;
  }
  //���˲�����ʱ��������Ч��׷�ӵ����
  pSense->FilterNewPos++;
  if(pSense->FilterNewPos >= FilterSize)
    pSense->FilterNewPos = 0;
  *(pFilterBuf + pSense->FilterNewPos) = CurVol;
  //�˲�����->ƽ��
  //if(pSense->FilterType == SENSE_FILTER_TYPE_AVERVGE){
    pSense->Vol = AverageS2(pFilterBuf, FilterSize);
    return;
  //}
}

//----------------------------У׼����--------------------------------------
//0�궨�ɹ�������ʧ��!
signed char Sense_Calibration(struct _Sense *pSense,
                              signed short TargetVol, //Ŀ��Ũ��ֵ
                              unsigned char IsGain)//������(����ǰ�����)����������
{
  //��ǰ��ֵ����ʱ
  if(pSense->Flag & (SENSE_LOW_OV_ERR | SENSE_HI_OV_ERR)) return -1;  
  //����궨ʱ��ֻ��һ��
  if(IsGain && !(pSense->Flag & SENSE_ADJ_1ST_FINAL)) return -1; 
 
  const struct _SenseDesc *pDesc = pSense->pDesc;  
  //��Ŀ��Ũ��ֵ���������Դ�ź�ֵ(ֱ�����ź�Դ������ֵ��)
  signed short OrgData;
  if(pDesc->AntiNlLut != NULL) //������ת��
    OrgData = pDesc->AntiNlLut(pSense->Vol);
  else OrgData = pSense->Vol;
  
  OrgData = Line_GetXInZeroMutiU(pSense->Info.Zero, //ֱ�ߴ���y���xֵ
                                 pSense->Info.Gain,//б��
                                 pDesc->GainQ,//б�ʵ�Qֵ
                                 OrgData);  //δ֪x���y����
  
  //����궨ģʽʱ���������������ֵ
  if(IsGain){
    unsigned short Diff;//���Ŀ�������Ӱ�쾫�ȣ���ֹ�궨��
    if(pSense->PrvVol >= TargetVol) Diff = pSense->PrvVol - TargetVol;
    else Diff =  TargetVol - pSense->PrvVol;
    if(Diff < pDesc->DiffAdjVol) return -1;
    
    unsigned short Gain = Line_GetMuti(pSense->PrvSignal + 32767,  //��0��x����
                            pSense->PrvVol + 32767,  //��0��y����
                            OrgData +  32767,  //��1��x����
                            TargetVol + 32767,  //��1��y����                      
                            pDesc->GainQ); //б�ʵ�Qֵ
    if(Gain <= 0) return -1;//�쳣
    pSense->Info.Gain = Gain;
  }
  //����궨�򵥵�궨���������ֵ
  pSense->Info.Zero = Line_GetXzInZeroMutiU(OrgData,  //xֵ
                                            pSense->Info.Gain,//б��
                                            pDesc->GainQ,//б�ʵ�Qֵ
                                            TargetVol);  //x��Ӧ�ĵ�y����
  
  //�滻�ϴ�Ϊ��ǰ�Ա��´�ʹ��
  pSense->PrvSignal = OrgData;
  pSense->PrvVol = TargetVol;
  pSense->Flag |= SENSE_ADJ_1ST_FINAL; //ֻҪ�ɹ�������һ������  
  //��󱣴�
  Eeprom_Wr(pDesc->InfoBase,
            &pSense->Info,
            sizeof(struct _SenseInfo));
  //�궨�ɹ��󣬽�Ŀ��Ũ��ֵ���뵱ǰ��������ʾ���½��
  pSense->Vol = TargetVol;
  memset2((unsigned short*)pSense->pFilterBuf, 
          (unsigned short)TargetVol, 
          pSense->FilterSize);
  return 0;
}

//--------------------------------�õ��ڷ�ΧĿ��ֵ------------------------------
signed short Sense_GetVolInScope(const struct _Sense *pSense)
{
  const struct _SenseDesc *pDesc = pSense->pDesc;
  signed short Vol = pSense->Vol;
  if(Vol < pDesc->Min) return pDesc->Min;
  if(Vol > pDesc->Max) return pDesc->Max;
  return Vol;
}




