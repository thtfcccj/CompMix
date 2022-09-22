/*******************************************************************************

					            APP总体配置模块实现

********************************************************************************/
#include  "gCfg.h"
#include  "Eeprom.h"
#include  "InfoBase.h"
#include  <String.h>


struct _gCfg gCfg;

#ifdef G_CFG_EX_DEFAULT //外部没有定义时
  extern const struct _gCfg gCfg_Default;
#else
  const struct _gCfg gCfg_Default = {
    GCFG_BIT_DEFAULT,
    GCFG_BYTE_DEFAULT,
  };
#endif

/****************************************************************************
                             相关函数实现
****************************************************************************/
//--------------------------------初始化函数------------------------------
void gCfg_Init(unsigned char Inited,  signed char IsHandInit)
{
  memset(&gCfg, 0 , sizeof(struct _gCfg));
  
  //内部变量初始化 ,仅允许首次初始化
  if(!Inited && !IsHandInit){//装载默认
    memcpy(&gCfg, &gCfg_Default, sizeof(struct _gCfg));
    Eeprom_Wr(gCfg_GetInfoBase(),
              &gCfg,
              sizeof(struct _gCfg));
  }
  else{
    Eeprom_Rd(gCfg_GetInfoBase(),
                &gCfg,
                sizeof(struct _gCfg));
  }
}

//-----------------------------设置并保存位配置--------------------------
void gCfg_CfgBit(unsigned char Pos,    //位置下标
                     unsigned char BitCfg,    //位配置
                     unsigned char ValidMask) //影响位
{
  gCfg.Bit[Pos] &= ~ValidMask;
  gCfg.Bit[Pos] |= (BitCfg & ValidMask);
  //保存
  Eeprom_Wr(gCfg_GetInfoBase(),
            &gCfg,
            sizeof(struct _gCfg));
}

//-------------------------------设置通用字节数据并保存----------------------
void gCfg_CfgData(unsigned char Pos,
                  unsigned char Byte)
{
  gCfg.Byte[Pos] = Byte;
  //保存
  Eeprom_Wr(gCfg_GetInfoBase(),
            &gCfg,
            sizeof(struct _gCfg));  
}


