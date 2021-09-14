/***************************************************************************

                     单例化 数字电位器(POT)标准化接口
此实现与硬件无关
****************************************************************************/
#include "Pot.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include <string.h>

struct _Pot Pot;//直接单例化

/***************************************************************************
                          相关函数实现
***************************************************************************/

//-------------------------重新启动检测-----------------------------------
static void _Restart(void)
{
  Pot.Flag = 1 | (Pot.Flag & POT_EX_MASK);//复位开始通讯
  Pot_Task();//开始同步
}

//-------------------------------初始化函数---------------------------------
void Pot_Init(unsigned char Id,  //分配的通道标识
              signed char IsInited)//初始化标志

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
  Pot_cbPos2HwInit();  //硬件初始化
  _Restart();        //开机更新位置
}

//-----------------------------任务函数---------------------------------
//<8ms调用一次
void Pot_Task(void)
{
  signed char Resume;
  
  //硬件通讯阻塞时查询
  if(Pot.Flag & POT_HW_COMM_BLOCK){
    Resume = Pot_cbIsHwCommDoing();
    if(Resume > 0) return; //通讯未完(未检查超时)
    //通讯完成了
    Pot.Flag &= ~POT_HW_COMM_BLOCK;
    Pot_Pos2HwEnd(Resume); //结束
    return;
  }
  
  //未同步完成或未通讯上时，重新启动同步
  if(Pot.Flag & POT_HW_COMM_COUNT_MASK){
    Pot_cbPos2HwStart();
    Resume = Pot_cbIsHwCommDoing();
    if(Resume > 0) Pot.Flag |= POT_HW_COMM_BLOCK;//硬件通讯阻塞了
    else Pot_Pos2HwEnd(Resume); //直接结束 
    return;
  }
  
  #ifdef SUPPORT_POT_DELAY_HW_CHECK   //支持硬件实时延时检测时
    if(Pot.Delay)  Pot.Delay--;
    else _Restart(pPot);//重新检测
  #endif
}

//------------------------结束电位器位置到硬件POT-----------------------------
//POT通讯完成后调用
void Pot_Pos2HwEnd(signed char Resume)//0:正常结束，负：未通讯上结束
{
  if(Resume == 0){//同步完成
    Pot.Flag &= ~POT_HW_COMM_COUNT_MASK;
    #ifdef SUPPORT_POT_DELAY_HW_CHECK   //支持硬件实时延时检测时复位
      Pot.Delay = SUPPORT_POT_DELAY_HW_CHECK;
    #endif    
  }
  else{//未通讯上,通讯故障计数
    if((Pot.Flag & POT_HW_COMM_COUNT_MASK) != POT_HW_COMM_COUNT_MASK)
      Pot.Flag++;
  }
}

//------------------------更新电位器位置---------------------------------
void Pot_UpdatePos(PotLen_t NewPos)
{
  if(Pot.Flag & POT_HW_COMM_BLOCK) return;//还在通讯中呢
  
  Pot.CurPos = NewPos;
  _Restart();        //立即更新位置
}

//---------------------------保存电位器当前位置-----------------------------
void Pot_SavePos(void)
{
  Pot.Info.Pos = Pot.CurPos;  
  Eeprom_Wr(Pot_GetInfoBase(), &Pot.Info, sizeof(struct _PotInfo));
}



