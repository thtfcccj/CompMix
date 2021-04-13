/***************************************************************************

                       ���ֵ�λ��(POT)��׼���ӿ�
��ʵ����Ӳ���޹�
****************************************************************************/


#include "Pot.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include <string.h>

/***************************************************************************
                          ��غ���ʵ��
***************************************************************************/

//-------------------------------��ʼ������---------------------------------
void Pot_Init(signed char IsInited,
              struct _Pot *pPot, unsigned char Id)
{
  memset(pPot, 0, sizeof(pPot));
  pPot->Id = Id;
  if(!IsInited){
    pPot->Info.Pos = Pot_cbGetDefaultPos(pPot);
    Eeprom_Wr(Pot_GetInfoBase(Id), &pPot->Info, sizeof(struct _PotInfo));
  }
  else{
    Eeprom_Rd(Pot_GetInfoBase(Id), &pPot->Info, sizeof(struct _PotInfo));
  }
  pPot->CurPos = pPot->Info.Pos;
  Pot_cbPos2HwInit(pPot);  //Ӳ����ʼ��
  Pot_Task(pPot);//��ʼͬ��
}

//-----------------------------������---------------------------------
//<8ms����һ��
void Pot_Task(struct _Pot *pPot)
{
  //ͬ����ɻ�ͬ���ȴ���ʱ��������
  if(pPot->Flag & (POT_HW_SYNC_FINAL | POT_HW_COMM_WAITTING)){
    #ifdef SUPPORT_POT_DELAY_HW_CHECK   //֧��Ӳ��ʵʱ��ʱ���ʱ
      pPot->Delay--;
      if(!pPot->Delay){
        pPot->Flag &= ~POT_HW_SYNC_FINAL;//ȡ��ͬ����־������ͬ��
      }
    #endif
    return;
  }
  //��ʼͬ��
  signed char Resume = Pot_cbPos2HwStart(pPot);
  if(Resume > 0) pPot->Flag |= POT_HW_COMM_WAITTING;//�ȴ�
  else Pot_Pos2HwEnd(pPot, Resume); //ֱ�ӽ���
}

//------------------------������λ��λ�õ�Ӳ��POT-----------------------------
//POTͨѶ��ɺ����
void Pot_Pos2HwEnd(struct _Pot *pPot, 
                   signed char Resume)//�����󣬷�����ȷ
{
  pPot->Flag &= ~POT_HW_COMM_WAITTING;//����
  if(Resume < 0){
    if((pPot->Flag & POT_HW_COMM_ERR_MASK) != POT_HW_COMM_ERR_MASK)
      pPot->Flag++;
    else pPot->Flag &= ~POT_HW_SYNC_FINAL;//ȡ��ͬ�����
  }
  else{//ͬ�����
    pPot->Flag &= ~POT_HW_COMM_ERR_MASK;//ȥ����
    pPot->Flag |= POT_HW_SYNC_FINAL;//��ͬ����־
    #ifdef SUPPORT_POT_DELAY_HW_CHECK   //֧��Ӳ��ʵʱ��ʱ���ʱ
      pPot->Delay = SUPPORT_POT_DELAY_HW_CHECK;
    #endif
  }
}

//------------------------���µ�λ��λ��---------------------------------
void Pot_UpdatePos(struct _Pot *pPot, unsigned char NewPos)
{
  pPot->CurPos = NewPos;
  pPot->Flag = 0;//��λ  
  Pot_Task(pPot);//��ʼͬ��
}

//---------------------------�����λ����ǰλ��-----------------------------
void Pot_SavePos(struct _Pot *pPot)
{
  pPot->Info.Pos = pPot->CurPos;  
  Eeprom_Wr(Pot_GetInfoBase(pPot->Id), &pPot->Info, sizeof(struct _PotInfo));
}



