/*******************************************************************************

                    温湿度(单例化)模块
此模块专为温湿度计准备，计算精度:  温度0.1或1度(实现决定), 湿度1%RH
*******************************************************************************/
#ifndef __HUMITURE_H
#define __HUMITURE_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/*****************************************************************************
                             相关配置
*****************************************************************************/

//平均滤波算法滤波个数
#ifndef HUMITURE_FILTER_BUF_COUNT
  #define HUMITURE_FILTER_BUF_COUNT   16  
#endif

//温度分配的种类ID号
#ifndef HUMITURE_TEMP_KIND_ID
  #define HUMITURE_TEMP_KIND_ID   0  
#endif
//湿度分配的种类ID号
#ifndef HUMITURE_RH_KIND_ID
  #define HUMITURE_RH_KIND_ID   1  
#endif

//温度分配的单位ID号
#ifndef HUMITURE_TEMP_UNIT_ID
  #define HUMITURE_TEMP_UNIT_ID   0  
#endif
//湿度分配的单位ID号
#ifndef HUMITURE_RH_UNIT_ID
  #define HUMITURE_RH_UNIT_ID   1  
#endif

/*****************************************************************************
                             相关结构
*****************************************************************************/
#include "Sense.h"

//主结构
struct _Humiture{
  struct _Sense Sense[2]; //传感器信号
  signed short FilterBuf[2][HUMITURE_FILTER_BUF_COUNT];//滤波算法缓冲区
};

extern struct _Humiture Humiture;

/*******************************************************************************
                           相关函数
*******************************************************************************/

//------------------------------初始化函数-------------------------------------
void Humiture_Init(unsigned char IsInited); //是否已初始化

//---------------------------------更新温度值-----------------------------------
#define Humiture_UpdateTemp(temp) do{Sense_Update(&Humiture.Sense[0], temp);}while(0)

//---------------------------------更新湿度值-----------------------------------
#define Humiture_UpdateRh(rh) do{Sense_Update(&Humiture.Sense[1], rh);}while(0)

//----------------------------温度校准处理--------------------------------------
//0标定成功，否则失败!
#define Humiture_CalibrationTemp(taget, isGain) \
  (Sense_Update(&Humiture.Sense[0], taget, isGain))

//----------------------------湿度校准处理--------------------------------------
//0标定成功，否则失败!
#define Humiture_CalibrationRh(taget, isGain) \
  (Sense_Update(&Humiture.Sense[1], taget, isGain))

//------------------------------------得到当前温度------------------------------
#define Humiture_GetTemp() (Sense_GetVolInScope(&Humiture.Sense[0]))

//------------------------------------得到当前湿度------------------------------
#define Humiture_GetRh() (Sense_GetVolInScope(&Humiture.Sense[1]))

/*******************************************************************************
                           回调函数
*******************************************************************************/




#endif






