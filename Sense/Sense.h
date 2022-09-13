/*******************************************************************************

                     感知端信号处理
//此模块 将物理信号实时转换为实际信号
//此模块专为下述参数固定的简单传感器设计：
//  固定位置：即传感器ID号固定  
//  固定测量物质： 即传感器类型固定
//  固定固定量程:  即传感器测量范围固定
//  固定单位: 即只能用一种单位(如摄氏度与华氏度须固定)
//  最大表示范围+=-32768, 允许中间点小数点
//  原始信号一直为上升沿(若为下降，则调用前反向即可转换为上升)
//  传感器在整个范围表现均衡，正存在在某点表现特殊的情况。
//支持不同滤波算法，以及非线性校正
//支持单点标定(标零点即x轴偏移)与两点标定(标增益即斜率)
//此模块主要为物联网设计，仅负责数值相关，不负责其工作状态，故障，报警等功能。
*******************************************************************************/
#ifndef __SENSE_H
#define __SENSE_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/*****************************************************************************
                             相关配置
*****************************************************************************/

//是否支持此模块，外部可选时定义
//#define SUPPORT_SENSE  

/*****************************************************************************
                             相关结构
*****************************************************************************/

//静态描述结构
struct _SenseDesc{
  //相关信息
  unsigned long InfoBase;  //struct _SenseInfo在EEPROM中的起始位置
  unsigned char Id;       //为此结构分配的ID标识
  unsigned char GainQ;    //增益使用的Q值 
  unsigned char Dot;      //测量的小数点位置
  unsigned char UnitId;   //测量物质的单位ID 
  unsigned short KindId;  //测量物质的类型ID
  
  signed short Min;   //量程范围最小值,超过此值将判断故障
  signed short Max;   //量程范围最大值,超过此值将判断故障
  signed short DefaultZero;    //默认零点原始值,用于初始化
  signed short DefaultGain;    //默认增益值,用于初始化
  signed short DefaultZeroVol; //默认零点(第一点)浓度值,用于标定
  signed short DefaultAdjVol;  //两点标定时，默认第二点浓度值
  unsigned short DiffAdjVol;   //两点标定时，第2点与第1点结果差最小值，小于此值禁止标定  
  
  //支持非线性运算时,由计算出的值到得到转换后的值
  signed short (*NlLut)(signed short OrgVol);
  //支持非线性运算时,由转换后的值,得转换前的值，主要用于校准。
  signed short (*AntiNlLut)(signed short OrgVol);
};

//Info结构, 需放入EEPROM存储
struct _SenseInfo{
  signed short   Zero;  //零点,原始信号为单位
  unsigned short Gain;  //增益,转换后即为直实温度
};

//主结构
struct _Sense{
  struct _SenseInfo Info;         //信息
  const struct _SenseDesc *pDesc;//静态描述结构
  signed short PrvSignal;     //上次标校时的原始信号值 
  signed short PrvVol;        //上次标校时的目标目标值  

  //滤波缓冲及算法
  signed short *pFilterBuf;    //支持滤波时,滤波缓冲区
  unsigned short FilterSize;     //支持滤波时,滤波个数
  unsigned short FilterPara;     //不同滤波算法时，带入的参数
  unsigned short FilterNewPos;   //最新数据在缓冲中的位置
  unsigned char FilterType;      //滤波算法类型,见定义
  
  unsigned char Flag;            //相关标志,见定义
  signed short  Vol;              //计算出的结果
};

//滤波算法定义为：
#define SENSE_FILTER_TYPE_NONE     0  //无滤波
#define SENSE_FILTER_TYPE_AVERVGE  1  //最近FilterSize次平均滤波
#define SENSE_FILTER_TYPE_AVG_MID  2  //去除前后FilterPara个数据的平均滤波(暂未支持)
#define SENSE_FILTER_TYPE_AVG_PEAK 3  //去除缓冲中的最大最小值后的平均滤波(暂未支持)
#define SENSE_FILTER_TYPE_COUNT    4  //支持4种

//相关标志定义为：
#define SENSE_ADJ_1ST_FINAL   0x10  //增益标校时，零点完成标志
#define SENSE_VOL_VALID       0x20  //结果标志
#define SENSE_LOW_OV_ERR      0x40  //负超限标志,计算结果<-32768了
#define SENSE_HI_OV_ERR       0x80  //正超限标志,计算结果>+32767了

/*******************************************************************************
                           相关函数
*******************************************************************************/

//------------------------------初始化-------------------------------------
//初始化为无滤波算法
void Sense_Init(struct _Sense *pSense,
                const struct _SenseDesc *pDesc,//静态描述结构
                unsigned char IsInited); //是否已初始化
                
//------------------------------更新滤波算法--------------------------------
//可在初始化后调用
void Sense_UpdateFilter(struct _Sense *pSense,
                        unsigned char FilterType, //滤波算法类型,见定义
                        signed short *pFilterBuf, //滤波缓冲区
                        unsigned short FilterSize,//滤波缓冲个数
                        unsigned short FilterPara);//不同滤波算法时，带入的参数
          
//-----------------------------更新目标值-----------------------------------
//周期调用以更新温度,
//若采用了去极值等需要排序的滤波算法时，应在非中断中调用
void Sense_Update(struct _Sense *pSense,
                  signed short OrgSignal); //原始信号(上升沿)

//----------------------------校准处理--------------------------------------
//0标定成功，否则失败!
signed char Sense_Calibration(struct _Sense *pSense,
                              signed short TargetVol, //目标浓度值
                              unsigned char IsGain);//标增益(需提前标零点)，否则标零点

//--------------------------------当前值是否异常------------------------------
#define Sense_IsInvalid(sense) ((sense)->Flag & (SENSE_LOW_OV_ERR | SENSE_HI_OV_ERR))

//--------------------------------得到在范围目标值------------------------------
//建议先Sense_IsInvalid()后调用此函数                        
signed short Sense_GetVolInScope(const struct _Sense *pSense);             
                              
//------------------------------------得到当前值------------------------------
#define Sense_GetSense(sense) ((sense)->Vol)

//--------------------------两点标定时，上次点相关----------------------------
//清除上次点
#define Sense_ClrPrvPoint(sense) do{(sense)->Flag &= ~SENSE_ADJ_1ST_FINAL;}while(0)
//是否上次有点
#define Sense_IsPrvPoint(sense) ((sense)->Flag & SENSE_ADJ_1ST_FINAL)

#endif






