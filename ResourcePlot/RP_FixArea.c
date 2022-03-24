/******************************************************************************

                         固定区域绘图实现

******************************************************************************/
#include "RP_FixArea.h"
#include "Plot.h"
#include "ResourceIndex.h"
#include "ePic.h"
#include "Fonts.h"
#include <string.h>

//----------------------------固定区域绘图------------------------------
//绘制整个区域或局部
void RP_FixArea(unsigned long Handle,
                 unsigned char PlotMask,//0b所有，1b图标,2b参数
                 const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  Plot_SetBrushColor(pDesc->cBg);   
  if(PlotMask & 0x01){//绘制背景
    Plot_FullRect(x, y,pDesc->Rect.w,pDesc->Rect.h);
    PlotMask = 0xff;//需所有
  }
  if(PlotMask & 0x02){//绘制所有图标
    unsigned char Base = pDesc->IconBase;
    unsigned char Count = pDesc->IconCount + Base;
    for(; Base < Count; Base++){
      RP_PaintIcon(Handle, RP_FixArea_cbGetIconDesc(Handle, Base), x, y);
    }
  }
  if(PlotMask & 0x04){//绘制所有参数
    unsigned char Base = pDesc->ParaBase;
    unsigned char Count = pDesc->ParaCount + Base;
    for(; Base < Count; Base++){
       RP_PaintPara(Handle, RP_cbGetParaDesc(Handle, Base), x, y);
    }
  }
}

//-----------------------固定区域更新某个指定图标------------------------------
//用于主动更新
void RP_FixAreaIcon(unsigned long Handle,
                     unsigned char IconId,//图标ID号
                     const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  //查找图标Desc
  unsigned char Base = pDesc->IconBase;
  unsigned char Count = pDesc->IconCount + Base;
  for(; Base < Count; Base++){
    const struct _RpIconDesc *pDesc = RP_FixArea_cbGetIconDesc(Handle, Base);
    if((pDesc != NULL) && ((pDesc->Info & RP_ICON_ID_MASK) == IconId)){
      RP_PaintIcon(Handle, pDesc, x, y); //匹配了，绘图
      return;
    }
  }
}

//-----------------------固定区域更新某个指定参数------------------------------
//用于主动更新
void RP_FixAreaPara(unsigned long Handle,
                     unsigned char ParaId,//图标ID号
                     const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  //查找图标Desc
  unsigned char Base = pDesc->ParaBase;
  unsigned char Count = pDesc->ParaCount + Base;
  for(; Base < Count; Base++){
    const struct _RpParaDesc *pDesc = RP_cbGetParaDesc(Handle, Base);
    if((pDesc != NULL) && ((pDesc->Info & RP_ICON_ID_MASK) == ParaId)){
      RP_PaintPara(Handle, pDesc, x, y); //匹配了，绘图
      return;
    }
  }
}




