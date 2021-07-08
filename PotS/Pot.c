/***************************************************************************

                     ������ ���ֵ�λ��(POT)��׼���ӿ�
��ʵ����Ӳ���޹�
****************************************************************************/
#include "Pot.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include <string.h>

struct _Pot Pot;//ֱ�ӵ�����

/***************************************************************************
                          ��غ���ʵ��
***************************************************************************/

//-------------------------�����������-----------------------------------
static void _Restart(void)
{
  Pot.Flag = 1 | (Pot.Flag & POT_EX_MASK);//��λ��ʼͨѶ
  Pot_Task();//��ʼͬ��
}

//-------------------------------��ʼ������---------------------------------
void Pot_Init(unsigned char Id,  //�����ͨ����ʶ
              signed char IsInited)//��ʼ����־

{
  memset(&Pot, 0, sizeof(struct _Pot));
  Pot.Id = Id;
  if(!IsInited){
    Pot.Info.Pos = Pot_cbGetDefaultPos();
    Eeprom_Wr(Pot_GetInfoBase(), &Pot.Info, sizeof(struct _PotInfo));
  }
  else{
    Eeprom_Rd(Pot_GetInfoBase(), &Pot.Info, sizeof(struct _PotInfo));
  }
  Pot.CurPos = Pot.Info.Pos;
  Pot_cbPos2HwInit();  //Ӳ����ʼ��
  _Restart();        //��������λ��
}

//-----------------------------������---------------------------------
//<8ms����һ��
void Pot_Task(void)
{
  signed char Resume;
  
  //Ӳ��ͨѶ����ʱ��ѯ
  if(Pot.Flag & POT_HW_COMM_BLOCK){
    Resume = Pot_cbIsHwCommDoing();
    if(Resume > 0) return; //ͨѶδ��(δ��鳬ʱ)
    //ͨѶ�����
    Pot.Flag &= ~POT_HW_COMM_BLOCK;
    Pot_Pos2HwEnd(Resume); //����
    return;
  }
  
  //δͬ����ɻ�δͨѶ��ʱ����������ͬ��
  if(Pot.Flag & POT_HW_COMM_COUNT_MASK){
    Pot_cbPos2HwStart();
    Resume = Pot_cbIsHwCommDoing();
    if(Resume > 0) Pot.Flag |= POT_HW_COMM_BLOCK;//Ӳ��ͨѶ������
    else Pot_Pos2HwEnd(Resume); //ֱ�ӽ��� 
    return;
  }
  
  #ifdef SUPPORT_POT_DELAY_HW_CHECK   //֧��Ӳ��ʵʱ��ʱ���ʱ
    if(Pot.Delay)  Pot.Delay--;
    else _Restart(pPot);//���¼��
  #endif
}

//------------------------������λ��λ�õ�Ӳ��POT-----------------------------
//POTͨѶ��ɺ����
void Pot_Pos2HwEnd(signed char Resume)//0:��������������δͨѶ�Ͻ���
{
  if(Resume == 0){//ͬ�����
    Pot.Flag &= ~POT_HW_COMM_COUNT_MASK;
    #ifdef SUPPORT_POT_DELAY_HW_CHECK   //֧��Ӳ��ʵʱ��ʱ���ʱ��λ
      Pot.Delay = SUPPORT_POT_DELAY_HW_CHECK;
    #endif    
  }
  else{//δͨѶ��,ͨѶ���ϼ���
    if((Pot.Flag & POT_HW_COMM_COUNT_MASK) != POT_HW_COMM_COUNT_MASK)
      Pot.Flag++;
  }
}

//------------------------���µ�λ��λ��---------------------------------
void Pot_UpdatePos(PotLen_t NewPos)
{
  if(Pot.Flag & POT_HW_COMM_BLOCK) return;//����ͨѶ����
  
  Pot.CurPos = NewPos;
  _Restart();        //��������λ��
}

//---------------------------�����λ����ǰλ��-----------------------------
void Pot_SavePos(void)
{
  Pot.Info.Pos = Pot.CurPos;  
  Eeprom_Wr(Pot_GetInfoBase(), &Pot.Info, sizeof(struct _PotInfo));
}



