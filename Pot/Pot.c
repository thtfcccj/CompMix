/***************************************************************************

                       ���ֵ�λ��(POT)��׼���ӿ�
��ʵ����Ӳ���޹�
****************************************************************************/
#include "Pot.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include <string.h>

struct _Pot _Pot;//����ռλ

/***************************************************************************
                          ��غ���ʵ��
***************************************************************************/

//-------------------------�����������-----------------------------------
static void _Restart(struct _Pot *pPot)
{
  pPot->Flag = 1 | (pPot->Flag & POT_EX_MASK);//��λ��ʼͨѶ
  Pot_Task(pPot);//��ʼͬ��
}

//-------------------------------��ʼ������---------------------------------
void Pot_Init(struct _Pot *pPot, 
              unsigned char Id,  //�����ͨ����ʶ
              signed char IsInited)//��ʼ����־

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
  _Restart(pPot);        //��������λ��
}

//-----------------------------������---------------------------------
//<8ms����һ��
void Pot_Task(struct _Pot *pPot)
{
  signed char Resume;
  
  //Ӳ��ͨѶ����ʱ��ѯ
  if(pPot->Flag & POT_HW_COMM_BLOCK){
    Resume = Pot_cbIsHwCommDoing(pPot);
    if(Resume > 0) return; //ͨѶδ��(δ��鳬ʱ)
    //ͨѶ�����
    pPot->Flag &= ~POT_HW_COMM_BLOCK;
    Pot_Pos2HwEnd(pPot, Resume); //����
    return;
  }
  
  //δͬ����ɻ�δͨѶ��ʱ����������ͬ��
  if(pPot->Flag & POT_HW_COMM_COUNT_MASK){
    Pot_cbPos2HwStart(pPot);
    Resume = Pot_cbIsHwCommDoing(pPot);
    if(Resume > 0) pPot->Flag |= POT_HW_COMM_BLOCK;//Ӳ��ͨѶ������
    else Pot_Pos2HwEnd(pPot, Resume); //ֱ�ӽ��� 
    return;
  }
  
  #ifdef SUPPORT_POT_DELAY_HW_CHECK   //֧��Ӳ��ʵʱ��ʱ���ʱ
    if(pPot->Delay)  pPot->Delay--;
    else _Restart(pPot);//���¼��
  #endif
}

//------------------------������λ��λ�õ�Ӳ��POT-----------------------------
//POTͨѶ��ɺ����
void Pot_Pos2HwEnd(struct _Pot *pPot, 
                   signed char Resume)//0:��������������δͨѶ�Ͻ���
{
  if(Resume == 0){//ͬ�����
    pPot->Flag &= ~POT_HW_COMM_COUNT_MASK;
    #ifdef SUPPORT_POT_DELAY_HW_CHECK   //֧��Ӳ��ʵʱ��ʱ���ʱ��λ
      pPot->Delay = SUPPORT_POT_DELAY_HW_CHECK;
    #endif    
  }
  else{//δͨѶ��,ͨѶ���ϼ���
    if((pPot->Flag & POT_HW_COMM_COUNT_MASK) != POT_HW_COMM_COUNT_MASK)
      pPot->Flag++;
  }
}

//------------------------���µ�λ��λ��---------------------------------
void Pot_UpdatePos(struct _Pot *pPot, PotLen_t NewPos)
{
  if(pPot->Flag & POT_HW_COMM_BLOCK) return;//����ͨѶ����
  
  pPot->CurPos = NewPos;
  _Restart(pPot);        //��������λ��
}

//---------------------------�����λ����ǰλ��-----------------------------
void Pot_SavePos(struct _Pot *pPot)
{
  pPot->Info.Pos = pPot->CurPos;  
  Eeprom_Wr(Pot_GetInfoBase(pPot->Id), &pPot->Info, sizeof(struct _PotInfo));
}



