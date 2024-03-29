/*******************************************************************************

                         温度(单例化)模块
此模块固定温度单位为1℃，并上浮64℃以避免产生负温度值(上限为192℃)
//处理流程为：原始信号->减零点->X增益->(可选)温补曲线校正
//支持单点校准(校零点)与两点校准(校零点与增益)
*******************************************************************************/
#ifndef __TEMP_H
#define __TEMP_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/*****************************************************************************
                             相关配置
*****************************************************************************/

//是否支持此模块，外部可选时定义
//#define SUPPORT_TEMP  

//支持滤波定义(本局里配置)
//#define SUPPORT_TEMP_DEV_FILETER   
//#define SUPPORT_TEMP_DEV_FILETER_AVERVGE   //支持简单平均时
//#define SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID  //支持平均但去除最大最小值时

//定义是否支持温补曲线(本局里配置)
//#define SUPPORT_TEMP_LUT     

//温度零点固定为64度
#define TEMP_BASE    64   

/*****************************************************************************
                             相关结构
*****************************************************************************/
//---------------------------硬件支持-------------------------------------
#if defined(SUPPORT_HC32)
 #include "Temp_HC32.h"
#elif defined(SUPPORT_NTC10K_3380K)
 #include "Temp_NTC10K_3380K.h"
#endif

//-------------------------------------Info结构---------------------------------
//需放入EEPROM存储
struct _TempInfo{
  signed short Zero;    //零点,原始信号为单位
  unsigned short Gain;  //增益,转换后即为温度
};

//主结构,需放入EEPROM存储
struct _Temp{
  struct _TempInfo Info;   //信息
  unsigned short  PrvAdjSignal;  //上次标校时的原始信号值 
  unsigned char  PrvAdjTemp;    //上次标校时的目标温度值  
  
  #ifdef SUPPORT_TEMP_FILETER//支持滤波时
    #ifdef SUPPORT_TEMP_FILETER_AVERVGE_MID
      unsigned char FilterBuf[SUPPORT_TEMP_FILETER * 2];//缓存的最近温度值加位值
    #else
      unsigned char FilterBuf[SUPPORT_TEMP_FILETER ];//缓存的最近温度值
    #endif
    unsigned char FilterPos;  //滤波存储位置  
  #endif
    
  unsigned char CurTemp;    //当前标准温度值
  unsigned char Flag;         //相关标志,见定义
};

//相关标志定义为：
#define TEMP_ADJ_1ST_FINAL   0x20  //增益标校模式时，温度零点记录完成
#define TEMP_UPDATE_LATER    0x10  //更新延后标志
#define TEMP_LOW_OV_ERR      0x20  //负超限标志(有可能是未标定)
#define TEMP_HI_OV_ERR       0x40  //正超限标志(有可能是未标定)

extern struct _Temp Temp;
/*******************************************************************************
                           相关行为函数
*******************************************************************************/

//------------------------------初始化函数-------------------------------------
void Temp_Init(unsigned char IsInited); //是否已初始化

//---------------------------------更新温度值-----------------------------------
//周期调用以更新温度,建议在非中断中周期调用
void Temp_Update(void); //原始信号(为负时需上浮固定值为正)

//---------------------------稍后更新任务-------------------------------
//放入Temp_Update()快两倍的进程中调用(建议32ms进程)
#ifdef SUPPORT_TEMP_FILETER //支持滤波时
  void Temp_UpdateLater(void);
#else
  #define Temp_UpdateLater() do{}while(0)
#endif

//----------------------------温度校准处理--------------------------------------
//0标定成功，否则失败!
signed char Temp_Calibration(unsigned char TargetTemp, //目标浓度值
                              unsigned char IsGain);//标增益(需提前标零点)，否则标零点

/*******************************************************************************
                           相关属性函数
*******************************************************************************/

//--------------------------------当前温度是否异常------------------------------
#define Temp_IsInvalid() (Temp.Flag & (TEMP_LOW_OV_ERR | TEMP_HI_OV_ERR))

//------------------------------------得到当前温度------------------------------
#define Temp_GetTemp() (Temp.CurTemp)

//--------------------------两点标定时，上次点相关------------------------------
//清除上次点
#define Temp_ClrPrvPoint() \
  do{Temp.Flag &= ~TEMP_ADJ_1ST_FINAL;}while(0)
//是否上次有点
#define Temp_IsPrvPoint() (Temp.Flag & TEMP_ADJ_1ST_FINAL)
    
/*******************************************************************************
                           回调函数
*******************************************************************************/

//---------------------------得到当前温度值对应信号-----------------------------
unsigned short Temp_cbGetCurTempSignal(void);

//------------------------------------温度校正--------------------------------
//如：线性度校正，其它附加处理等,输入为Q8温度，输出为结果温度
#ifdef SUPPORT_TEMP_LUT
  unsigned char Temp_cbAppendPro(unsigned short OrgTempQ8);
#else//不支持时,直接转换
  #define Temp_cbAppendPro(tempQ8) ((tempQ8) >> 8)
#endif

//----------------------------------温度反校正---------------------------------
//与Temp_cbAppendPro()成对使用,输入为结果温度，输出为原始温度(非Q8)
#ifdef SUPPORT_TEMP_LUT
  unsigned char Temp_cbAntiAppendPro(unsigned char CurTemp);
#else//不支持时,直接转换
  #define Temp_cbAntiAppendPro(temp) (temp)
#endif

#endif






