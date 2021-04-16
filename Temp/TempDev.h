/*******************************************************************************

                         温度设备模块
此模块支持同一软件项目内，将多种途径产生的原始温度信号，转换为温度值
此模块固定温度单位为0.01℃，并上浮273.5℃以避免产生负温度值(上限为3276.1℃)
*******************************************************************************/
#ifndef __TEMP_DEV_H
#define __TEMP_DEV_H

/*****************************************************************************
                                相关说明与定义
*****************************************************************************/

//处理流程为：原始信号->减零点->X增益->(可选)曲线校正->(可选)附加校正
//支持单点校准(校零点)与两点校准(校零点与增益)

//支持多种不同的滤波方式:
//#define SUPPORT_TEMP_DEV_FILETER   //支持滤波时
//#define SUPPORT_TEMP_DEV_FILETER_AVERVGE   //支持简单平均时
//#define SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID  //支持平均但去除最大最小值时

//支持字节温度时同时定义起始温度，精度为℃，起点为此值,范围：(-此值) ~ (255-此值)
//#define TEMP_DEV_BYTE_BASE           70

//温度零点固定为2740度，用0度方便无小数温度计算
#define TEMP_DEV_BASE           2740

//0为无效值
#define TEMP_DEV_TEMP_INVALID   0       

/*****************************************************************************
                                相关结构
*****************************************************************************/
#include "NolinearConvert.h"   //曲线校正

//------------------------------------Desc结构----------------------------------
//根据同一软件项目不同途径(如一路18B20,一路模拟传感器,一路自MCU内部AD)具体情况，配置参数
//常量结构直接固化在Flash中
struct _TempDev;
struct _TempDevDesc{
  unsigned char GainQ;                   //增益使用的Q值
  unsigned char LutAryCapability;        //曲线补偿表容量
  unsigned char FilterBufCount;          //温度不稳定时滤波缓冲个数,0时不滤波
  unsigned char FilterCfg;              //滤波类型与参数，见定义
  void (*InfoToDefault)(struct _TempDev *pDev);//将设备Info恢复出厂设置
  const struct _NolinearConvertTbl * pLutAry;//曲线补偿表阵列, NULL无此功能
  unsigned short (*ExVolPro)(struct _TempDev *pDev, unsigned short Temp);//附加校正,
  unsigned short (*AntiExVolPro)(struct _TempDev *pDev, unsigned short Temp);//反附加校正，标定用
  unsigned short CalibrationTempDiffMin; //标定时，允许的最小温差值
  unsigned short CalibrationOrgSignalDiffMin; //标定时，允许的最小信号量
};

//FilterCfg定义为:
#define TEMP_DEV_FILETER_TYPE_SHIFT   5 //滤波方式,位
#define TEMP_DEV_FILETER_PARA_MASK    0x1f //不同滤波方式对应的参数
//滤波方式,定义为
#define TEMP_DEV_FILETER_NONE         0  //不滤波
#define TEMP_DEV_FILETER_AVERVGE      1  //简单平均，参数为当前位置
#define TEMP_DEV_FILETER_AVERVGE_MID  2  //平均但去除最大最小值，参数为两侧丢弃数量

//-------------------------------------Info结构---------------------------------
//需放入EEPROM存储
struct _TempDevInfo{
  signed short Zero;    //零点,原始信号为单位
  unsigned short Gain;  //增益,转换后即为温度
};

//主结构,需放入EEPROM存储
struct _TempDev{
  struct _TempDevInfo Info;   //信息
  #ifdef SUPPORT_TEMP_DEV_FILETER//支持滤波时
    unsigned short *pFilterBuf;//缓存的最近温度值，长度为pDesc->FilterBufCount
    unsigned short FilterPara;  //不同滤波方式对应的相关参数  
  #endif
  
  unsigned short CurTemp;    //当前标准温度值
  //两点校准相关:
  unsigned short  PrvAdjTemp;    //上次标校时的目标温度值
  unsigned short  PrvAdjSignal;  //上次标校时的原始信号值
  //其它
  unsigned char Ch;           //内部定义的通道ID
  unsigned char Flag;         //相关标志,见定义
};

//相关标志定义为：
#define TEMP_DEV_ADJ_MODE        0x80  //温度增益标校模式,否则为零点标定模式
#define TEMP_DEV_ADJ_1ST_FINAL   0x20  //增益标校模式时，温度零点记录完成
#define TEMP_DEV_UPDATE_LATER    0x10  //更新延后标志

/*******************************************************************************
                           相关行为函数
*******************************************************************************/

//------------------------------初始化函数-------------------------------------
void TempDev_Init(struct _TempDev *pDev,
                  unsigned char IsInited, //是否已初始化
                  unsigned char Ch);      //分配的通道

//---------------------------------更新温度值-----------------------------------
//周期调用以更新温度,建议在非中断中周期调用
void TempDev_Update(struct _TempDev *pDev,
                    unsigned short OrgSignal); //原始信号(为负时需上浮固定值为正)

//---------------------------稍后更新任务-------------------------------
//放入TempDev_Update()快两倍的进程中调用(建议32ms进程)
#ifdef SUPPORT_TEMP_DEV_FILETER //支持滤波时
  void TempDev_UpdateLater(struct _TempDev *pDev);
#else
  #define TempDev_UpdateLater(dev) do{}while(0)
#endif

//----------------------------温度校准处理--------------------------------------
//形参为当前时刻的目标温度值
//需标校两次,以最后两次调用此函数的结果为准
void TempDev_Calibration(struct _TempDev *pDev,
                         unsigned short Temp); //目标浓度值

//--------------------------支持字节温度表示时互转------------------------------
//可用于存储时节省空间
#ifdef TEMP_DEV_BYTE_BASE
  unsigned char TempDev_ToByte(unsigned short Temp);
  unsigned short TempDev_FromByte(unsigned char ByteTemp);
#endif

/*******************************************************************************
                           相关属性函数
*******************************************************************************/
//------------------------------------得到当前温度------------------------------
#define TempDev_GetTemp(dev) ((dev)->CurTemp)

//------------------------------标校模式相关------------------------------------
//是否标识在两点标定模式
#define TempDev_IsAdj2Mode(dev) ((dev)->Flag &  TEMP_DEV_ADJ_MODE) 
//是否标识在1点标定模式
#define TempDev_IsAdj1Mode(dev) (!((dev)->Flag & TEMP_DEV_ADJ_MODE))
//切换到1点标定模式
#define TempDev_SetAdj1Mode(dev) do{(dev)->Flag &= ~TEMP_DEV_ADJ_MODE;}while(0)

//切换到两点标定模式
#define TempDev_SetAdj2Mode(dev) do{(dev)->Flag |= TEMP_DEV_ADJ_MODE;}while(0)
//两点标定模式时，清除上次点
#define TempDev_ClrAdj2PrvPoint(dev) do{(dev)->Flag &= ~TEMP_DEV_ADJ_1ST_FINAL;}while(0)
//切换到两点标定模式同时清除上次点
#define TempDev_SetAdj2Mode_ClrPrv(dev) do{TempDev_SetAdj2Mode(dev); \
  TempDev_ClrAdj2PrvPoint(dev);}while(0)

/*******************************************************************************
                           回调函数
*******************************************************************************/

//------------------------由定义的通道ID得到Desc结构----------------------------
//必须实现且不能为NULL
const struct _TempDevDesc *TempDev_cbpGetDesc(unsigned char Ch);

#endif






