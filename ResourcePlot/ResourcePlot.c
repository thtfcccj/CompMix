/******************************************************************************

                      资源信息绘图模块相关函数实现


******************************************************************************/
#include "ResourcePlot.h"
#include "Plot.h"
#include "ResourceIndex.h"
#include "ePic.h"
#include "Fonts.h"
#include <string.h>

//-----------------------指定位置图标绘图-----------------------------
//更新某个具体图标
void RP_PaintIcon(unsigned char Handle,const struct _RpIconDesc *pDesc,
                  unsigned short x, unsigned short y)//相对x,y
{
  if(pDesc == NULL) return; //异常
  const unsigned char *ePic = RP_cbGetIcon(Handle,
                                            pDesc->Info & RP_ICON_ID_MASK);  
  if(ePic == RI_NULL) return;//异常未找到
  //找到x,y及调色板信息
  //设置调色板,已提前预置背景色
  unsigned char IconId = pDesc->Info & RP_ICON_ID_MASK;
  Plot_SetPenColor(RP_cbGetIconFg(Handle,IconId));
  ePic_ePicBuf(ePic, RP_cbGetIconSize(Handle,IconId));//初始化ePic
  x += pDesc->xl;
  if(pDesc->Info & RP_ICON_X9) x+= 256;
  y += pDesc->yl;
  if(pDesc->Info & RP_ICON_Y9) x+= 256;  
  ePic_Plot(x,y);//绘图
}

//----------------------指定位置参数绘图-----------------------------
//更新某个具体参数
void RP_PaintPara(unsigned char Handle,const struct _RpParaDesc *pDesc,
                  unsigned short x, unsigned short y)//相对x,y
{
  if(pDesc == NULL) return; //异常
  unsigned char ParaId = pDesc->Info & RP_PARA_ID_MASK;  
  const char *pSring = RP_cbGetString(Handle,ParaId);  
  if(pSring == NULL) return;//异常未找到
  //找到x,y及调色板信息
  //设置调色板,已提前预置背景色
  Plot_SetPenColor(RP_cbGetParaFg(Handle,ParaId));
  x += pDesc->xl;
  if(pDesc->Info & RP_PARA_X9) x+= 256;
  y += pDesc->yl;
  if(pDesc->Info & RP_PARA_Y9) x+= 256;  

  //仅支持半角字体选择
  unsigned char fFontId = pDesc->Para & RP_PARA_FONT_MASK;
  if(fFontId) fFontId++;//1是全角字体，要避开。
  
  const struct _FontsDesc *pFontBase = (const struct _FontsDesc*)FONTS_DESC_BASE;
  Font_PlotLine(pFontBase + fFontId,//半角时使用的字体
                pFontBase + 1, //全角时使用的字体,固定为1
                x,y,pSring);
}

//----------------------------固定区域绘图------------------------------
//绘制整个区域或局部
void RP_FixArea(unsigned char Handle,
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
      RP_PaintIcon(Handle, RP_cbGetIconDesc(Handle, Base), x, y);
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
void RP_FixAreaIcon(unsigned char Handle,
                     unsigned char IconId,//图标ID号
                     const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  //查找图标Desc
  unsigned char Base = pDesc->IconBase;
  unsigned char Count = pDesc->IconCount + Base;
  for(; Base < Count; Base++){
    const struct _RpIconDesc *pDesc = RP_cbGetIconDesc(Handle, Base);
    if((pDesc != NULL) && ((pDesc->Info & RP_ICON_ID_MASK) == IconId)){
      RP_PaintIcon(Handle, pDesc, x, y); //匹配了，绘图
      return;
    }
  }
}

//-----------------------固定区域更新某个指定参数------------------------------
//用于主动更新
void RP_FixAreaPara(unsigned char Handle,
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




