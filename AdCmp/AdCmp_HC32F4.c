/******************************************************************************

                   AD(硬件)比较模块-在HC32F4中的实现
//仅支持序列A连续扫描模式,最多只支持最多16通道的比较
******************************************************************************/
#include "AdCmp.h"
#include "IoCtrl.h"
#include "misc.h"

/***************************************************************************
                           硬件相关配置说明及变量
****************************************************************************/

//定义AD比较控制器(AC = AdCompare)
#ifndef AC
  #define AC      M4_ADC2
#endif

//配置Io,同时需含时钟使能,物理与实际通道的映射关系
//#define AdCmp_cbCfgIo() do{}while(0)
//得到得到CR1配置: 含平均次数(b8:10),AD位数(b4:5)对齐方式(右对齐b7=0)
//#define AdCmp_cbGetCR0() do{}while(0)
//配置中断
//#define AdCmp_cbCfgInt() do{}while(0)
//指定中断处理函数宏
//#define IrqAdCmp_Handler  IRQ008_Handler

static unsigned char _ChMax = 0; //最大通道
/*********************************************************************
                        相关函数实现
*********************************************************************/

//-----------------------------初始化-----------------------------
void AdCmp_Init(void)
{
  //配置Io,同时需含时钟使能,物理与实际通道的映射关系
  AdCmp_cbCfgIo();
  AC->STR = 0;  //先停止
  AC->CR0 = AdCmp_cbGetCR0() | 1; //配置为序列A连续扫描模式
  AC->CHSELRA0 = 0;
  AC->AVCHSELR0 = 0;
  AC->AWDCHSR0 = 0;
  AdCmp_cbCfgInt(); //配置中断
}

//--------------------------设置比较条件--------------------------
void AdCmp_Condition(unsigned short lAD,   //低端比较AD值
                     unsigned short hAD,  //高端比较AD值
                     signed char Condition)//条件
{
  AC->AWDDR0 = lAD;    
  AC->AWDDR1 = hAD;
  //b8=1使能中断，b7:6=1序列A输出B不出,
  //B4=1：AWDDR0≦转换结果≦AWDDR1时，比较条件满足
  if(Condition)
    AC->AWDCR = ((unsigned short)1 << 8) | (1 << 6) | (1 << 4);
  else 
    AC->AWDCR = ((unsigned short)1 << 8) | (1 << 6);
}

//-----------------------开启需比较的AD通道--------------------
//需在停止采集时开启
void AdCmp_OpenCh(unsigned char Ch)
{
  if(Ch > _ChMax) _ChMax = Ch;  
  //这里只支持前16个通道  
  unsigned short Mask = (unsigned short)1 << Ch;
  AC->CHSELRA0 |= Mask;
  AC->AVCHSELR0 |= Mask; 
  AC->AWDCHSR0 |= Mask; 
}

//-----------------------关闭需比较的AD通道--------------------
//需在停止采集时关闭
void AdCmp_CloseCh(unsigned char Ch)
{
  unsigned short Mask = (unsigned short)1 << Ch;
  AC->CHSELRA0 &= ~Mask;
  AC->AVCHSELR0 &= ~Mask;  
  AC->AWDCHSR0 &= ~Mask; 
}

//--------------------------启动比较----------------------------
//需在通道开启后调用
void AdCmp_Start(void)
{
  AC->AWDCR_f.AWDEN = 1;//看门狗比较功能使能
  AC->STR = 1;  //开启
}

//-------------------------停止比较-----------------------------
void AdCmp_Stop(void)
{
  AC->STR = 0;  //关闭  
  AC->AWDCR_f.AWDEN = 0;//禁止
}

/*********************************************************************
                        中断处理函数实现
*********************************************************************/
void IrqAdCmp_Handler(void)
{
  //读取中断状态
  unsigned short Mask = AC->AWDSR0;
  //当寄存器置位，需要清除寄存器时，请读“1”后写“0”。
  AC->AWDSR0 = 0;
  AdCmp_IRQ(Mask);
  
  //检查是那个AD通道中断了成立时，回调
  //for(unsigned char Ch = 0; Ch < _ChMax; Ch++){
  //  if(Mask & ((unsigned short)1 << Ch))
  //     AdCmp_Int(Ch);
  //}
}



