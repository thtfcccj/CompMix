/******************************************************************************

                     多通道电压比较器模块-在HC32(默认M0P)中的实现

*******************************************************************************/
#include "MVC.h"

#include "HC32.h"
#include <String.h>

struct _MVC MVC;
//定义由寄存器通道到采集通道间的查找表
static const unsigned char _VcLut[VC_CHANNEL_GRP_COUNT][VC_CHANNEL_PER_COUNT] = 
  VC_CHANNEL_LUT;

//定义起始通道
static const unsigned char _ChBaseLut[VC_CHANNEL_GRP_COUNT] = VC_CH_BASE_LUT;

#ifndef PVC
  #define PVC    M0P_VC  //别名
#endif

#ifndef MVC_cbCfgVCclk
  #define MVC_cbCfgVCclk()   do{  M0P_SYSCTRL->PERI_CLKEN0_f.VC = 1;}while(0) 
#endif

/***************************************************************************
                             函数实现
****************************************************************************/


//-----------------------------------初始化-----------------------------
void MVC_Init(void)
{  
  memset(&MVC, 0, sizeof(struct _MVC));
  memset(MVC.CurCh,0xff,VC_CHANNEL_GRP_COUNT);

  //1. MVCC 、BGR模块时钟使能
  MVC_cbCfgVCclk();
  
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
    if(Ch >= VC_CHANNEL_PER_COUNT){//首次启动采集
      Ch = 0; 
    }
    else{//已有结果了
      //读取稳定后的结果
      unsigned char IsHi;
      if(Vc == 0) IsHi = PVC->IFR & (1 << 2);
      else if(Vc == 1) IsHi = PVC->IFR & (1 << 3);
      else IsHi = PVC->IFR & (1 << 5);
      if(_VcLut[Vc][Ch] & 0x80) IsHi = !IsHi;//负端引入时反向
      
      //更新当前状态
      MvcChMask_t ChMask = 1 << (_ChBaseLut[Vc] + Ch);
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
      //最后更新至下个通道以启动
      Ch++;
      if((Ch >= VC_CHANNEL_PER_COUNT) || (_VcLut[Vc][Ch] == 0xff))
        Ch = 0;
    }
  
    //最后启动下一通道
    MVC.CurCh[Vc] = Ch;
    
    unsigned char ChInfo = _VcLut[Vc][Ch];
    unsigned long Cr;
    if(ChInfo & 0x80)//负端引入
      Cr = ((unsigned long)ChInfo  << 4) | GetVcCrCfgN(Vc);
    else //正端引入
      Cr = ChInfo | GetVcCrCfgP(Vc);
    if(Vc == 0) PVC->VC0_CR = Cr;
    else if(Vc == 1) PVC->VC1_CR = Cr;
    else PVC->VC2_CR = Cr; 
    
  } //end for
}


