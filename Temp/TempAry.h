/*******************************************************************************

                         温度设备-多例化实现
多例化时可使用此子模块实例化
*******************************************************************************/
#ifndef __TEMP_ARY_H
#define __TEMP_ARY_H

/*****************************************************************************
                                相关说明与定义
*****************************************************************************/

//实例化数量
#ifndef TEMP_ARY_COUNT
  #define TEMP_ARY_COUNT  2
#endif

/*****************************************************************************
                                相关结构
*****************************************************************************/
#include "TempDev.h" 

extern struct _TempDev TempDevAry[TEMP_ARY_COUNT];

/*******************************************************************************
                           相关函数
*******************************************************************************/

//------------------------------初始化函数-------------------------------------
void TempAry_Init(unsigned char IsInited); //是否已初始化

//---------------------------------更新温度值-----------------------------------
//周期调用以更新温度,建议在非中断中周期调用
void TempAry_Update(void);

//---------------------------稍后更新任务-------------------------------
//放入TempAry_Update()快两倍的进程中调用(建议32ms进程)
#ifdef SUPPORT_TEMP_DEV_FILETER //支持滤波时
  void TempAry_UpdateLater(void);
#else
  #define TempAry_UpdateLater() do{}while(0)
#endif
  
//---------------------------由通道得到设备-------------------------------------
//未检查超限情况
#define TempAry_pGetDev(ch)  (&(TempDevAry[ch]))

/*******************************************************************************
                         相关回调函数
*******************************************************************************/
  
//-------------------------由定义的通道得到原始信号值---------------------------
unsigned short TempAry_cbGetOrgSignal(unsigned char Ch);

#endif






