/******************************************************************************

                         阵列区域绘图模块
此模块可用于菜单与项的应用,最大支持每页256项
******************************************************************************/
#ifndef __RP_ARY_AREA_H 
#define __RP_ARY_AREA_H 
  
/***********************************************************************
                             说明
***********************************************************************/
//按行列方式组织：每个块相当于一个固定区域,如：每屏2行3列：
//              0   ■ □    1
//              2   □ □    3
//              4   □ □    5

/***********************************************************************
                           相关配置
***********************************************************************/

#ifndef RP_ARY_AREA_KEY_KEEP_OV    
  #define RP_ARY_AREA_KEY_KEEP_OV   10//按键后停留时间
#endif

#ifndef RP_ARY_AREA_AUTO_SCREEN_OV    
  #define RP_ARY_AREA_AUTO_SCREEN_OV   10//自动滚屏时间
#endif

#ifndef RP_ARY_AREA_REFRESH_PERIOD    
  #define RP_ARY_AREA_REFRESH_PERIOD   10//非实时数据自动更新时间
#endif

/***********************************************************************
                             相关结构
***********************************************************************/
//依赖
#include "ResourcePlot.h"
#include "Color.h"

//---------------------------子类型信息------------------------------
struct _RP_AryArea_STypeInfo{
  unsigned long SType;  //子类型,即handle中的RP_HANDLE_STYPE_MASK位
  //描述主handle下，子类型图标与变量查找表信息：
  unsigned char IconBase;  //图标信息ID查找表起始
  unsigned char IconCount; //图标信息ID查找表个数
  unsigned char ParaBase;  //变量信息ID查找表起始
  unsigned char ParaCount; //变量信息ID查找表个数 
};

//---------------------------主结构----------------------------------
struct _RP_AryArea{
  
  const struct _RpMainAreaDesc *pMainDesc; //整体描述
  const struct _RpAryAreaDesc *pAryDesc;   //项描述 
  unsigned long Handle;                   //主句柄 
  struct _RP_AryArea_STypeInfo  STypeInfo; //读取到的当前子类型信息
  struct _RpFixAreaDesc FDesc;        //用于绘制项内数据的固定描述结构
  //内部管理
  unsigned short AryCount;       //阵列个数  
  unsigned short AryStart;       //本页的阵列起始
  unsigned char Focus;           //页内焦点,即选择的第几个
  unsigned char PrvFocus;        //上页焦点,与页内焦点相同时不更新上页
  
  unsigned char PlotMask;        //绘图类型掩码，b0项背景,b1项图,b2项数,b7所有
  
  unsigned char AutoPageTimer;   //自动翻页倒计定时器
  unsigned char RefreshTimer;    //普通数据更新定时器  
  //unsigned char DyncTimer;      //动画定时器,可用于翻页特效(暂不支持)
};

/************************************************************************
                           相关函数
*************************************************************************/

//-----------------------------初始化函数----------------------------
void RP_AryArea_Init(struct _RP_AryArea *pArea,    //主结构
                     const struct _RpMainAreaDesc *pMainDesc,//整体描述
                     const struct _RpAryAreaDesc *pAryDesc, //项描述                     
                     unsigned long Handle, //主句柄 
                     unsigned short AryCount);    //初始化时阵列大小

//----------------------------任务处理函数----------------------------
//1s左右调用一次
void RP_AryArea_Task(struct _RP_AryArea *pArea); 

//-------------------------------刷新所有函数----------------------------
//用于立即重建所有数据
#define RP_AryArea_RefreshAll(area) do{(area)->PlotMask = 0xff; }while(0)

//-------------------------------刷新普通数据函数----------------------------
//用于立即重建普通数据
#define RP_AryArea_RefreshNor(area) do{(area)->RefreshTimer = 0x06; }while(0)

//-----------------------------按键处理函数----------------------------
//GuideKey定义为: 确认0左1上2右3下4, 5上一页，6下一页,及数字键'1'-'9'
void RP_AryArea_Key(struct _RP_AryArea *pArea, unsigned char GuideKey); 

/******************************************************************************
                               回调函数
*******************************************************************************/

//-------------------由主Handle与阵列ID得到子类型相关信息----------------------
//返回0结束，后续无Item了，否则需填充pSTypeInfo
signed char RP_AryArea_cbGetSTypeInfo(unsigned long Handle, 
                                      unsigned short AryId,                        
                                      struct _RP_AryArea_STypeInfo *pSTypeInfo);

//------------------------------某项被选择------------------------------------
//在某项(焦点)上按“确认”键后，将调用此函数
void RP_AryArea_cbSelFinal(unsigned long Handle,
                           unsigned short AryId);//被选择项


#endif



