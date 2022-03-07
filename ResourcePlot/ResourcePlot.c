/******************************************************************************

                      资源信息绘图模块相关函数实现


******************************************************************************/
#include "ResourcePlot.h"
#include "Plot.h"
#include "ResourceIndex.h"
#include "ePic.h"
#include <string.h>

/***********************************************************************
                        固定区域绘图相关-含接口函数与回调函数
***********************************************************************/

//-------------------------固定区域图标绘图-----------------------------
//更新某个具体图标
void RP_PaintFixAreaIcon(unsigned char Handle,
                         unsigned char IconAryId)
{
  const struct _RpIconDesc *pDesc = RP_cbGetIconDesc(Handle, IconAryId);
  if(pDesc == NULL) return; //未找到或出错
  unsigned char IconId = pDesc->Info & RP_PARA_ID_MASK;
  const unsigned char *ePic = RP_cbGetIcon(Handle,IconId);  
  if(ePic == RI_NULL) return;//异常未找到
  //找到x,y及调色板信息
  //设置调色板,已提前预置背景色
  Plot_SetPenColor(RP_cbGetIconFg(Handle,IconId));
  ePic_ePicBuf(ePic);//初始化ePic
  unsigned short x = pDesc->xl;
  if(pDesc->Info & RP_PARA_X9) x+= 256;
  unsigned short y = pDesc->yl;
  if(pDesc->Info & RP_PARA_Y9) x+= 256;  
  ePic_Plot(x,y);//绘图
}

//----------------------------固定区域绘图------------------------------
//绘制整个区域或局部
void RP_FixArea(unsigned char Handle,
                     unsigned char PlotMask,//绘制区域7b所有,0-6b变量0x7f所有
                     const struct _RpFixAreaDesc *pDesc)
{
  Plot_SetBrushColor(pDesc->cBg);  
  if(PlotMask & 0x01){//绘制背景
    Plot_FullRect(pDesc->Rect.x, pDesc->Rect.y,
                  pDesc->Rect.w,pDesc->Rect.h);
  }
  if(PlotMask & 0x02){//绘制所有图标
    unsigned char Base = pDesc->IconBase;
    unsigned char Count = pDesc->IconCount + Base;
    for(; Base < Count; Base++){
      RP_PaintFixAreaIcon(Handle, Base);
    }
  }
  if(PlotMask & 0x04){//绘制所有参数

  }  
  
}




