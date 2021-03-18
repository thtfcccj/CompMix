/*******************************************************************************

                         �¶��豸ģ��ʵ��

*******************************************************************************/
#include "TempDev.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include "MemMng.h"
#include <string.h>

/*******************************************************************************
                           �����Ϊ����ʵ��
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void TempDev_Init(struct _TempDev *pDev,
                  unsigned char IsInited, //�Ƿ��ѳ�ʼ��
                  unsigned char Ch)      //�����ͨ��
{
  memset(pDev, 0, sizeof(struct _TempDev));
  pDev->Ch = Ch;
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(Ch);
  if(!IsInited){//װ��Ĭ��
    pDesc->InfoToDefault(pDev);
    Eeprom_Wr(TempDev_GetInfoBase(Ch),
              &pDev->Info,
              sizeof(struct _TempDevInfo));
  }
  else{
    Eeprom_Rd(TempDev_GetInfoBase(Ch),
                &pDev->Info,
                sizeof(struct _TempDevInfo));
  }
  
  #ifdef SUPPORT_TEMP_DEV_FILETER //֧���˲�ʱ,�����ڴ�
  unsigned char FilterType = pDesc->FilterCfg >> TEMP_DEV_FILETER_TYPE_SHIFT;
  if(FilterType){
    unsigned short BufCapability = pDesc->LutAryCapability * 2;
    #ifdef SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID //����������1������
      if(FilterType == TEMP_DEV_FILETER_AVERVGE_MID) BufCapability *= 2;
    #endif
    pDev->pFilterBuf = MemMng_pvMalloc(BufCapability);
    memset(pDev->pFilterBuf, 0, BufCapability);//��ʼ��Ϊ��Чֵ
  }
  #endif //SUPPORT_TEMP_DEV_FILETER
  
  //������0��ʼ��
  //(��)
}

//---------------------------------�����¶�ֵ-----------------------------------
//ͬ�ڵ����Ը����¶�,�����ڷ��ж������ڵ���
void TempDev_Update(struct _TempDev *pDev,
                    unsigned short OrgSignal) //ԭʼ�ź�(Ϊ��ʱ���ϸ��̶�ֵΪ��)
{
  //========================��ԭʼ�źŵõ��¶�ֵ============================
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  unsigned short Temp;
  if(OrgSignal >= pDev->Info.Zero){//ֻ���������¶�
    //ԭʼ�ź�->�����
    unsigned long Data = OrgSignal - pDev->Info.Zero;
    //->X����
    Data *= pDev->Info.Gain;
    Temp = Data >> pDesc->GainQ;
    //->(��ѡ)����У��
    if(pDesc->pLutAry != NULL){
      Temp = NolinearConvert(Temp, //����ԴΪ��ǰ�¶�
                             pDesc->pLutAry,  //������������ұ��������
                             0);//�Զ���С
    }
    //->(��ѡ)����У��
    if(pDesc->ExVolPro != NULL){
      Temp = pDesc->ExVolPro(pDev, Temp);
      if(Temp == 0) return;//��Чֵ����
    }
  }
  else Temp = 1;//��ֵΪ�����¶ȣ�������,(0���ڱ�ʾδ��ʼ����)
  //==========================�˲�Ԥ����==============================
  #ifdef SUPPORT_TEMP_DEV_FILETER //֧���˲�ʱ
    unsigned short *pFilterBuf = pDev->pFilterBuf;
    //ֱ�ӽ��
    if(pFilterBuf == NULL){
      pDev->CurTemp = Temp;
      return;
    }
    //(��������Ч)�״�ʹ��ʱ,�����׸���Чֵ���
    if(*pFilterBuf == 0){
      unsigned short *pEndFilterBuf = pFilterBuf + pDesc->LutAryCapability;
      for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
        *pFilterBuf = Temp;
      return;
    }
    //���˲�����ʱ����׷�ӵ����
   if(pDev->FilterPara >= pDesc->LutAryCapability)
      pDev->FilterPara = 0;
    else pDev->FilterPara++;
    *(pFilterBuf + pDev->FilterPara) = Temp;
    pDev->FilterPara = 0; //TEMP_DEV_FILETER_AVERVGE_MIDʹ��
    pDev->Flag |= TEMP_DEV_UPDATE_LATER;//�Ժ���
  #else
    pDev->CurTemp = Temp; //ֱ�ӽ��
  #endif
}

//---------------------------�Ժ��������-------------------------------
//����TempDev_Update()��һ���Ľ����е���(����32ms����)
#ifdef SUPPORT_TEMP_DEV_FILETER //֧���˲�ʱ
void TempDev_UpdateLater(struct _TempDev *pDev)
{
  if(pDev->Flag & TEMP_DEV_UPDATE_LATER) return;//������
  
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  unsigned char Capability = pDesc->LutAryCapability;
  unsigned char FilterType = pDesc->FilterCfg >> TEMP_DEV_FILETER_TYPE_SHIFT;
  unsigned short *pFilterBuf = pDev->pFilterBuf;  
  
  #ifdef SUPPORT_TEMP_DEV_FILETER_AVERVGE //��ƽ��ʱ: �ۼӳ����
  if(FilterType == TEMP_DEV_FILETER_AVERVGE){
    unsigned short *pEndFilterBuf = pFilterBuf + Capability;
    unsigned long AddTemp = 0;
    for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
      AddTemp += *pFilterBuf;
    pDev->CurTemp = AddTemp / Capability;
    pDev->Flag &= ~TEMP_DEV_UPDATE_LATER;//һ�δ������
    return;
  }
  #endif //SUPPORT_TEMP_DEV_FILETER_AVERVGE
  
  #ifdef SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID  //֧��ƽ����ȥ�������Сֵʱ
  if(FilterType == TEMP_DEV_FILETER_AVERVGE_MID){
    if(pDev->FilterPara == 0){//�״γ�ʼ��
      memcpy(pFilterBuf, pFilterBuf + Capability, Capability * 2);
      pDev->FilterPara = Capability;
    }
    //��С����ð������,�����Lost��������,����������� 
    pFilterBuf += Capability;//ָ�����򻺳�    
    unsigned char End = pDev->FilterPara;  //��ִ�����ڷֳɶ�ݴ���
    unsigned char Lost = pDesc->FilterCfg & TEMP_DEV_FILETER_PARA_MASK;//ǰ������
    if(End <= Lost){//�ѽ�����,��������ǰ�¶�
      unsigned short *pEndFilterBuf = pFilterBuf + Capability - Lost;      
      pFilterBuf += Lost;
      unsigned long AddTemp = 0;
      for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
        AddTemp += *pFilterBuf;
      pDev->CurTemp = AddTemp / (Capability - Lost * 2);
      pDev->Flag &= ~TEMP_DEV_UPDATE_LATER;//һ�δ������
      return;
    }
    unsigned char Start = End - ((Capability - Lost * 2) / 4); //4һ��������������
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
    pDev->FilterPara = End; //�´ν���λ��
  }
  #endif //SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID
}
#endif //SUPPORT_TEMP_DEV_FILETER //֧���˲�ʱ

//-------------------------------�õ�ԭʼ�¶�ֵ---------------------------------
//����Ŀ���¶ȵõ�����ǰδ������¶�ֵ,Ϊ�����¶ȵķ����㣬��Ҫ���ڱ궨
static unsigned short _GetOrgTemp(struct _TempDev *pDev,
                                  unsigned short TargetTemp) //Ŀ���¶�
{
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  //->(��ѡ)���ӷ�У��
  if(pDesc->AntiExVolPro != NULL){
    TargetTemp = pDesc->AntiExVolPro(pDev, TargetTemp);
  }
  //->(��ѡ)������У��
  if(pDesc->pLutAry != NULL){
    struct _NolinearConvertTbl *pDeLut = NolinearConvert_pGetBuf();
    NolinearConvert_AntiCopy(pDeLut, //���򵽻�����
                             pDesc->pLutAry, 
                             0);
    TargetTemp = NolinearConvert(TargetTemp, //����Դ
                                 pDeLut,  //������������ұ��������
                                 0); 
  }
  return TargetTemp;
}

//------------------------��ԭʼ�¶�ֵ�õ�ԭʼ�ź�ֵ----------------------------
//Ϊ�����¶�ʱ�����������ķ����㣬��Ҫ���ڱ궨
static unsigned short  _GetOrgSignal(struct _TempDev *pDev,
                                     unsigned short OrgTemp) //ԭʼ�¶�
{
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  //->��X����
  unsigned long Data = (unsigned long )OrgTemp << pDesc->GainQ;
  Data /= pDev->Info.Gain;
  //ԭʼ�ź�->���������ź�
  return Data + pDev->Info.Zero;
}

//----------------------------�¶�У׼����--------------------------------------
void TempDev_Calibration(struct _TempDev *pDev,
                         unsigned short TargetTemp) //Ŀ��Ũ��ֵ
{
  unsigned short OrgTemp = _GetOrgTemp(pDev, TargetTemp);//��ǰ��ʵ�¶ȶ�Ӧԭ�¶�
  unsigned short CurSignal = _GetOrgSignal(pDev, ///��ǰ��ʵ�¶ȶ�Ӧ�����ź�
                                           _GetOrgTemp(pDev, pDev->CurTemp));
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  //����궨ģʽʱ����������
  if(pDev->Flag &TEMP_DEV_ADJ_MODE){
    if(!(pDev->Flag &TEMP_DEV_ADJ_1ST_FINAL)){//�����¼ֻ��һ��
      pDev->PrvAdjTemp = OrgTemp;
      pDev->PrvAdjSignal = CurSignal;
      pDev->Flag |= TEMP_DEV_ADJ_1ST_FINAL;
      return;
    }
    //�������ˣ���������ֵ;
    unsigned short TempDiff;
    unsigned short PrvAdjSignal = pDev->PrvAdjSignal;
    if(pDev->PrvAdjTemp > OrgTemp){//�ȸ��º����
      TempDiff = pDev->PrvAdjTemp - OrgTemp;
      if(PrvAdjSignal < CurSignal) return; //�û�������������
      PrvAdjSignal = PrvAdjSignal - CurSignal;
    }
    else{//�ȵ��º����
      TempDiff =  OrgTemp - pDev->PrvAdjTemp;
      if(CurSignal < PrvAdjSignal) return; //�û�������������
      PrvAdjSignal = CurSignal - PrvAdjSignal;
    }
    if(TempDiff < pDesc->CalibrationTempDiffMin)  return; //�²�̫С,���ܱ�У
    if(PrvAdjSignal < pDesc->CalibrationOrgSignalDiffMin) return; //ԭʼ�ź����̫С,���ܱ�У
    //�õ�����  
    unsigned long Data = (unsigned long)TempDiff << pDesc->GainQ;
    Data /= PrvAdjSignal;
    if(Data > 0xffff) pDev->Info.Gain = 0xffff; //���Ŵ�����
    else if(Data < 0x3f) pDev->Info.Gain = 0x3f; //��С�Ŵ�����
    else pDev->Info.Gain = Data;
  }
  //����궨�򵥵�궨���������ֵ
  unsigned long Data = (unsigned long)OrgTemp << pDesc->GainQ;
  Data /= pDev->Info.Gain;//��ΪĿ���¶���Ҫ�����
  pDev->Info.Zero = (signed short)CurSignal - (signed short)Data;
  //�滻�ϴ�Ϊ��ǰ�Ա��´�ʹ��
  pDev->PrvAdjTemp = OrgTemp;
  pDev->PrvAdjSignal = CurSignal;
  //��󱣴�
  Eeprom_Wr(TempDev_GetInfoBase(pDev->Ch),
              &pDev->Info,
              sizeof(struct _TempDevInfo));
}



