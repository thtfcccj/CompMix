/******************************************************************************

                     多通道电压比较器模块-在HC32中的实现

*******************************************************************************/
#include "MVC.h"

#include "HC32.h"
#include <String.h>

struct _MVC MVC;
//定义由寄存器通道到采集通道间的查找表
static const unsigned char _VcLut[VC_CHANNEL_GRP_COUNT][VC_CHANNEL_PER_COUNT + 1] = 
  VC_CHANNEL_LUT;

/***************************************************************************
                             函数实现
****************************************************************************/


//-----------------------------------初始化-----------------------------
void MVC_Init(void)
{  
  memset(&MVC,0xff,sizeof(struct _MVC));
  memset(MVC.CurCh,0xff,VC_CHANNEL_GRP_COUNT);
  MVC.PrvState = 0;
  MVC.CurState = 0;  

  //1. MVCC 、BGR模块时钟使能
  M0P_SYSCTRL->PERI_CLKEN0_f.VC = 1;
  
  //2. 配置IO与寄存器
  CfgVcIO();
  
  //触发开始采集
  MVC_Task();
}

//----------------------------任务函数------------------------------
//根据实时性要求，放在<=0.5ms进程中扫描，更高时放在中断中扫许
void MVC_Task(void)
{
  for(unsigned char Vc = 0; Vc < VC_CHANNEL_GRP_COUNT; Vc++){
    unsigned char Ch = MVC.CurCh[Vc];
    if(Ch>= VC_CHANNEL_PER_COUNT){//首次启动采集
      Ch = 0; 
    }
    else{//已有结果了
      //读取稳定后的结果
      unsigned char IsHi;
      if(Vc == 0) IsHi = M0P_VC->IFR & (1 << 2);
      else if(Vc == 1) IsHi = M0P_VC->IFR & (1 << 3);
      else IsHi = M0P_VC->IFR & (1 << 5);
      //更新当前状态
      MvcChMask_t ChMask = 1 << (_VcLut[Vc][0] + Ch);
      if(IsHi) MVC.CurState |= ChMask;
      else MVC.CurState &= ~ChMask;
      //只有上次与本次状态相同了，才更新总状态
      if((MVC.CurState & ChMask) == (MVC.PrvState & ChMask)){
        if(IsHi) MVC.State |= ChMask;
        else MVC.State &= ~ChMask;
      }
      else{//不相同，更新上次状态
        if(IsHi) MVC.PrvState |= ChMask;
        else MVC.PrvState &= ~ChMask;
      }
      //最后更新至下个通道
      Ch++;
      if(_VcLut[Vc][1 + Ch] >= VC_CHANNEL_PER_COUNT) Ch = 0;
    }
  
    //最后启动到下一通道
    MVC.CurCh[Ch] = Ch;
    unsigned long Cr = GetVcCrCfg() | _VcLut[Vc][1 + Ch];
    if(Vc == 0) M0P_VC->VC0_CR = Cr;
    else if(Vc == 1) M0P_VC->VC1_CR = Cr;
    else M0P_VC->VC2_CR = Cr; 
    
  } //end for
}


