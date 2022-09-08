/*******************************************************************************

                         温度设备模块-在DH11上的实现
此实现硬件相关，温度精度1度
*******************************************************************************/
#include "Humiture.h"
#include <string.h>

#include "Eeprom.h"
#include "InfoBase.h"
#include "DH11.h"

struct _Humiture Humiture;

/*******************************************************************************
                           相关函数实现
*******************************************************************************/
//温度的静态描述结构
struct _SenseDesc TempDesc = {
  //相关信息
  Temp_GetInfoBase(),  //struct _SenseInfo在EEPROM中的起始位置
  0,       //为此结构分配的ID标识
  HUMITURE_TEMP_GAIN_Q,      //增益使用的Q值 
  1,      //测量的小数点位置
  HUMITURE_TEMP_KIND_ID,      //测量物质的单位ID 
  HUMITURE_TEMP_UNIT_ID,       //测量物质的类型ID
  
  0,       //量程范围最小值,超过此值将判断故障
  60,      //量程范围最大值,超过此值将判断故障
  0,       //默认零点原始值,用于初始化
  1024,    //默认增益值,用于初始化
  0,       //默认零点(第一点)浓度值,用于标定
  40,      //两点标定时，默认第二点浓度值 
  
  NULL,//支持非线性运算时,由计算出的值到得到转换后的值
  NULL,//支持非线性运算时,由转换后的值,得转换前的值，主要用于校准。
};

//湿度的静态描述结构
struct _SenseDesc RhDesc = {
  //相关信息
  Rh_GetInfoBase(),  //struct _SenseInfo在EEPROM中的起始位置
  1,     //为此结构分配的ID标识
  HUMITURE_RH_GAIN_Q,    //增益使用的Q值 
  0,     //测量的小数点位置
  HUMITURE_RH_KIND_ID,      //测量物质的单位ID 
  HUMITURE_RH_UNIT_ID,       //测量物质的类型ID
  
  0,      //量程范围最小值,超过此值将判断故障
  100,   //量程范围最大值,超过此值将判断故障
  50,    //默认零点原始值,用于初始化
  1024,  //默认增益值,用于初始化
  50,    //默认零点(第一点)浓度值,用于标定
  80,    //两点标定时，默认第二点浓度值 
  
  NULL,//支持非线性运算时,由计算出的值到得到转换后的值
  NULL,//支持非线性运算时,由转换后的值,得转换前的值，主要用于校准。
};


//------------------------------初始化函数-------------------------------------
void Humiture_Init(unsigned char IsInited) //是否已初始化
{
  //memset(&Humiture, 0, sizeof(struct _Humiture));
  //初始化自身
  if(!IsInited){//装载默认
    memcpy(&Humiture.Info, 
           &HumitureInfo_cbDefault, 
           sizeof(struct _HumitureInfo));
    Eeprom_Wr(Humiture_GetInfoBase(),
              &Humiture.Info,
              sizeof(struct _HumitureInfo));
  }
  else{
    Eeprom_Rd(Humiture_GetInfoBase(),
              &Humiture.Info,
              sizeof(struct _HumitureInfo));
  }
  
  //温度初始化
  Sense_Init(&Humiture.Sense[0], &TempDesc, IsInited);  
  Sense_UpdateFilter(&Humiture.Sense[0],
                     SENSE_FILTER_TYPE_AVERVGE,//算法
                     Humiture.FilterBuf[0],//滤波缓冲区
                     HUMITURE_FILTER_BUF_COUNT,//滤波缓冲个数
                     0);//不同滤波算法时，带入的参数
  //湿度初始化  
  Sense_Init(&Humiture.Sense[1], &RhDesc, IsInited);
  Sense_UpdateFilter(&Humiture.Sense[1],
                     SENSE_FILTER_TYPE_AVERVGE,//算法
                     Humiture.FilterBuf[1],//滤波缓冲区
                     HUMITURE_FILTER_BUF_COUNT,//滤波缓冲个数
                     0);//不同滤波算法时，带入的参数
}

//---------------------------------得到故障状态--------------------------------
unsigned char Humiture_GetErr(signed char IsRh)
{
  return Dh11_GetLastErr();
}




