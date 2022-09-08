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

//温湿度Q值
#define HUMITURE_TEMP_GAIN_Q  10
#define HUMITURE_RH_GAIN_Q    10


/*****************************************************************************
                             相关结构
*****************************************************************************/
#include "Sense.h"

//主结构
struct _HumitureInfo{
  signed short   HiTemp;       //高报温度点
  signed short   LowTemp;      //低报温度点
  signed char    HiRH;         //高报湿度点
  signed char    LowRH;        //低报湿度点  
};

//主结构
struct _Humiture{
  struct _HumitureInfo Info;
  //信号(含种类,单位与深度)
  struct _Sense Sense[2]; //传感器信号
  signed short FilterBuf[2][HUMITURE_FILTER_BUF_COUNT];//滤波算法缓冲区
  //注：故障与报警交给用户以回调方式实现。
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

//---------------------------------得到故障状态--------------------------------
unsigned char Humiture_GetErr(signed char IsRh);

/*******************************************************************************
                           回调函数
*******************************************************************************/

//--------------------------------默认值----------------------------------
extern const struct _HumitureInfo HumitureInfo_cbDefault;

//-----------------------------得到温度报警状态---------------------------
//由用户实现,返回0正确，1低报，2高报
unsigned char Humiture_cbGetAlarm(signed char IsRh);

//-----------------------------是否为满量程测试状态-----------------------
//仅供外部使用,非0时，需置全部报警+满量程浓度。
unsigned char Humiture_cbIsFullState(void);


#endif






