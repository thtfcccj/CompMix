/******************************************************************************

                         固定区域绘图模块

******************************************************************************/
#ifndef __RES_FIX_H 
#define __RES_FIX_H 
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif
#include "ResourcePlot.h"


/*****************************************************************************
                               绘图函数
*****************************************************************************/

//----------------------------固定区域绘图------------------------------
//绘制整个区域或局部
void RP_FixArea(unsigned long Handle,
                unsigned char PlotMask,//0b所有，1b图标,2b参数
                const struct _RpFixAreaDesc *pDesc);

//-----------------------固定区域更新某个指定图标------------------------------
//用于主动更新
void RP_FixAreaIcon(unsigned long Handle,
                     unsigned char IconId,//图标ID号
                     const struct _RpFixAreaDesc *pDesc);

//-----------------------固定区域更新某个指定参数------------------------------
//用于主动更新
void RP_FixAreaPara(unsigned long Handle,
                     unsigned char ParaId,//图标ID号
                     const struct _RpFixAreaDesc *pDesc);

/********************************************************************************
                           回调函数
********************************************************************************/

//----------------------------由图标AryID找图信息------------------------------
const struct _RpIconDesc *RP_FixArea_cbGetIconDesc(unsigned long Handle,
                                                    unsigned char IconAryId);

#endif  //#ifndef __RESOURCE_PLOT_H 



