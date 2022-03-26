/******************************************************************************

                      资源信息绘图模块相关函数实现


******************************************************************************/
#include "ResourcePlot.h"
#include "Plot.h"
#include "ResourceIndex.h"
#include "ePic.h"
#include "Fonts.h"
#include <string.h>

//--------------------------得到相对坐标-------------------------------
static unsigned short _GetX(const struct _RpParaDesc *pDesc)
{
  unsigned short x = pDesc->xl; //低8位
  if(pDesc->Info & RP_ICON_X9) x += 256;  //高1位
  return x * RP_X_MUTI; //相对位置倍率
}

//--------------------------得到相对坐标-------------------------------
static unsigned short _GetY(const struct _RpParaDesc *pDesc)
{
  unsigned short y = pDesc->yl; //低8位
  if(pDesc->Info & RP_ICON_Y9) y += 256;  //高1位
  return y * RP_Y_MUTI; //相对位置倍率
}

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
  ePic_Plot(x + _GetX((const struct _RpParaDesc *)pDesc), 
            y + _GetY((const struct _RpParaDesc *)pDesc));//绘图
}

//----------------------指定位置参数绘图-----------------------------
//更新某个具体参数
void RP_PaintPara(unsigned long Handle,const struct _RpParaDesc *pDesc,
                  unsigned short x, unsigned short y)//相对x,y
{
  if(pDesc == NULL) return; //异常
  unsigned char ParaId = pDesc->Info & RP_PARA_ID_MASK;
  unsigned char Para = pDesc->Para;
  unsigned char mType = Para & RP_PARA_TYPE_MASK; 
  //===========================字符串型绘制=============================  
  if(mType == RP_PARA_TYPE_STRING){
    const char *pSring = RP_cbGetString(Handle,ParaId);  
    if(pSring == NULL) return;//异常未找到
    x += _GetX(pDesc);  //x绝对位置
    y += _GetY(pDesc);  //y绝对位置 
    //设置调色板,已提前预置背景色
    Plot_SetPenColor(RP_cbGetStringFg(Handle,ParaId));
    //仅支持半角字体选择
    unsigned char fFontId = pDesc->Para & RP_PARA_FONT_MASK;
    if(fFontId) fFontId++;//1是全角字体，要避开。
    
    const struct _FontsDesc *pFontBase = (const struct _FontsDesc*)FONTS_DESC_BASE;
    Font_PlotLine(pFontBase + fFontId,//半角时使用的字体
                  pFontBase + 1, //全角时使用的字体,固定为1
                  x,y,pSring);
    return;
  }
  if(mType != RP_PARA_TYPE_TOOLS) return; //其它暂不支持
  //工具类具体绘制：
  unsigned char ToolsType = Para & RP_PARA_TYPE_PARA;
  if(ToolsType >= RP_TOOLS_COUNT)  return;//不支持的工具类
  //提前读取
  const union _RP_NsDesc *pNsDesc = RP_pcbGetNsDesc(Handle, Para, ParaId);
  if(pNsDesc == NULL) return;//异常
  const union _RP_NsPara *pNsPara = RP_pcbGetNsPara(Handle, Para, ParaId); 
  if(pNsPara == NULL) return;//异常
  
  if(Handle & RP_HANDLE_REAL_REFRESH) ToolsType |= 0x80;//是否为实时刷新
  switch(ToolsType){
    case RP_TOOLS_PROGRESS: //进度条绘图
      RpTools_PlotProgress(&pNsDesc->Tools, &pNsPara->Tools, ToolsType, x, y); 
      break;
    default: break;
  }
}








