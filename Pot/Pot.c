/***************************************************************************

                       数字电位器(POT)标准化接口
此实现与硬件无关
****************************************************************************/
#include "Pot.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include <string.h>

struct _Pot _Pot;//测试占位

/***************************************************************************
                          相关函数实现
***************************************************************************/

//-------------------------重新启动检测-----------------------------------
static void _Restart(struct _Pot *pPot)
{
  pPot->Flag = 1 | (pPot->Flag & POT_EX_MASK);//复位开始通讯
  Pot_Task(pPot);//开始同步
}

//-------------------------------初始化函数---------------------------------
void Pot_Init(struct _Pot *pPot, 
              unsigned char Id,  //分配的通道标识
              signed char IsInited)//初始化标志

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
  _Restart(pPot);        //开机更新位置
}

//-----------------------------任务函数---------------------------------
//<8ms调用一次
void Pot_Task(struct _Pot *pPot)
{
  signed char Resume;
  
  //硬件通讯阻塞时查询
  if(pPot->Flag & POT_HW_COMM_BLOCK){
    Resume = Pot_cbIsHwCommDoing(pPot);
    if(Resume > 0) return; //通讯未完(未检查超时)
    //通讯完成了
    pPot->Flag &= ~POT_HW_COMM_BLOCK;
    Pot_Pos2HwEnd(pPot, Resume); //结束
    return;
  }
  
  //未同步完成或未通讯上时，重新启动同步
  if(pPot->Flag & POT_HW_COMM_COUNT_MASK){
    Pot_cbPos2HwStart(pPot);
    Resume = Pot_cbIsHwCommDoing(pPot);
    if(Resume > 0) pPot->Flag |= POT_HW_COMM_BLOCK;//硬件通讯阻塞了
    else Pot_Pos2HwEnd(pPot, Resume); //直接结束 
    return;
  }
  
  #ifdef SUPPORT_POT_DELAY_HW_CHECK   //支持硬件实时延时检测时
    if(pPot->Delay)  pPot->Delay--;
    else _Restart(pPot);//重新检测
  #endif
}

//------------------------结束电位器位置到硬件POT-----------------------------
//POT通讯完成后调用
void Pot_Pos2HwEnd(struct _Pot *pPot, 
                   signed char Resume)//0:正常结束，负：未通讯上结束
{
  if(Resume == 0){//同步完成
    pPot->Flag &= ~POT_HW_COMM_COUNT_MASK;
    #ifdef SUPPORT_POT_DELAY_HW_CHECK   //支持硬件实时延时检测时复位
      pPot->Delay = SUPPORT_POT_DELAY_HW_CHECK;
    #endif    
  }
  else{//未通讯上,通讯故障计数
    if((pPot->Flag & POT_HW_COMM_COUNT_MASK) != POT_HW_COMM_COUNT_MASK)
      pPot->Flag++;
  }
}

//------------------------更新电位器位置---------------------------------
void Pot_UpdatePos(struct _Pot *pPot, PotLen_t NewPos)
{
  if(pPot->Flag & POT_HW_COMM_BLOCK) return;//还在通讯中呢
  
  pPot->CurPos = NewPos;
  _Restart(pPot);        //立即更新位置
}

//---------------------------保存电位器当前位置-----------------------------
void Pot_SavePos(struct _Pot *pPot)
{
  pPot->Info.Pos = pPot->CurPos;  
  Eeprom_Wr(Pot_GetInfoBase(pPot->Id), &pPot->Info, sizeof(struct _PotInfo));
}



