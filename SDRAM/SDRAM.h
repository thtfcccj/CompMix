/************************************************************************

                          SDRAM驱动程序

************************************************************************/

/************************************************************************
                          配置及说明
************************************************************************/

//此模块初始化后即可，将映射到下述地址，或直接通过指定地址访问：
#ifndef SDRAM_BASE
  #define SDRAM_BASE       ((unsigned long)0xD0000000)
#endif
//若不能直接访问, 通过SDRAM_Rd()及SDRAM_Wr()访问(需实现)

/************************************************************************
                          函数接口
************************************************************************/

//--------------------------初始化函数------------------------------------
void SDRAM_Init(void);

//---------------------------读取数据实现---------------------------------
void SDRAM_Rd(unsigned long Adr,//SDRAM中的起始地址
              void *pVoid,
              unsigned short Len);

//---------------------------写入数据实现---------------------------------
void SDRAM_Wr(unsigned long Adr,//SDRAM中的起始地址
              const void *pVoid,
              unsigned short Len);

/************************************************************************
                          回调函数
************************************************************************/

//--------------------------IO初始化------------------------------------
//void SDRAM_cbIoInit(void);
#include "IoCtrl.h"
#define SDRAM_cbIoInit() CfgRamIo()