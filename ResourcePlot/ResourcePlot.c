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
void RP_PaintIcon(unsigned long Handle,const struct _RpIconDesc *pDesc,
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
void RP_PaintPara(unsigned long Handle,const struct _RpParaDesc *pDesc,
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




