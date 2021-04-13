/***************************************************************************

                       数字电位器(POT)标准化接口
此实现与硬件无关
****************************************************************************/


#include "Pot.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include <string.h>

/***************************************************************************
                          相关函数实现
***************************************************************************/

//-------------------------------初始化函数---------------------------------
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
  Pot_cbPos2HwInit(pPot);  //硬件初始化
  Pot_Task(pPot);//开始同步
}

//-----------------------------任务函数---------------------------------
//<8ms调用一次
void Pot_Task(struct _Pot *pPot)
{
  //同步完成或同步等待中时，不处理
  if(pPot->Flag & (POT_HW_SYNC_FINAL | POT_HW_COMM_WAITTING)){
    #ifdef SUPPORT_POT_DELAY_HW_CHECK   //支持硬件实时延时检测时
      pPot->Delay--;
      if(!pPot->Delay){
        pPot->Flag &= ~POT_HW_SYNC_FINAL;//取消同步标志以重新同步
      }
    #endif
    return;
  }
  //开始同步
  signed char Resume = Pot_cbPos2HwStart(pPot);
  if(Resume > 0) pPot->Flag |= POT_HW_COMM_WAITTING;//等待
  else Pot_Pos2HwEnd(pPot, Resume); //直接结束
}

//------------------------结束电位器位置到硬件POT-----------------------------
//POT通讯完成后调用
void Pot_Pos2HwEnd(struct _Pot *pPot, 
                   signed char Resume)//负错误，否则正确
{
  pPot->Flag &= ~POT_HW_COMM_WAITTING;//结束
  if(Resume < 0){
    if((pPot->Flag & POT_HW_COMM_ERR_MASK) != POT_HW_COMM_ERR_MASK)
      pPot->Flag++;
    else pPot->Flag &= ~POT_HW_SYNC_FINAL;//取消同步完成
  }
  else{//同步完成
    pPot->Flag &= ~POT_HW_COMM_ERR_MASK;//去故障
    pPot->Flag |= POT_HW_SYNC_FINAL;//置同步标志
    #ifdef SUPPORT_POT_DELAY_HW_CHECK   //支持硬件实时延时检测时
      pPot->Delay = SUPPORT_POT_DELAY_HW_CHECK;
    #endif
  }
}

//------------------------更新电位器位置---------------------------------
void Pot_UpdatePos(struct _Pot *pPot, unsigned char NewPos)
{
  pPot->CurPos = NewPos;
  pPot->Flag = 0;//复位  
  Pot_Task(pPot);//开始同步
}

//---------------------------保存电位器当前位置-----------------------------
void Pot_SavePos(struct _Pot *pPot)
{
  pPot->Info.Pos = pPot->CurPos;  
  Eeprom_Wr(Pot_GetInfoBase(pPot->Id), &pPot->Info, sizeof(struct _PotInfo));
}



