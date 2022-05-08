/******************************************************************************

                         各工具类绘图接口实现

******************************************************************************/
#include "RpTools.h"
#include "RpHandle.h"
#include "Plot.h"
#include <string.h>


//-------------------------------绘制进度条-------------------------------
void RpTools_PlotProgress(const union _RpToolsDesc *pNsDesc,
                          const union _RpToolsPara *pNsPara,
                          unsigned char Info, //b7:实时刷新,b4:配置
                          unsigned short x, unsigned short y)//绝对x,y
{
  unsigned short w = pNsDesc->Progress.w & 0x0fff;
  unsigned short h = pNsDesc->Progress.h & 0x0fff; 
  unsigned short cw = (pNsDesc->Progress.w >> 12) + 1;//边框宽度
  unsigned short ch = (pNsDesc->Progress.h >> 12) + 1;//边框高度
  
   //不是实时刷新时，画边框  
  Color_t PrvBg = Plot_GetBrushColor(); //用后需恢复
  if(!(Info & 0x80) && (pNsDesc->Progress.cBg != pNsDesc->Progress.cRect)){
    Plot_SetBrushColor(pNsDesc->Progress.cRect);
    Plot_FullRect(x,y, cw, h);// 左|
    Plot_FullRect(x,y, w, ch);// 上-
    Plot_FullRect(x+w-cw, y, cw, h);// 右|
    Plot_FullRect(x,y+h-ch, w, ch); // 下-
  }
  //填充进度条
  x += cw;
  w -= cw * 2;
  y += ch;
  h -= ch * 2;
  Plot_SetBrushColor(pNsDesc->Progress.cFg[pNsPara->Progress.FgId & 0x07]);//前景
  //得到百分比值
  cw = pNsPara->Progress.Percent;
  if(cw > 100) cw = 100;//超限了  
  if(Info & 0x10){//纵向进度条
    ch = ((unsigned short)cw * h) / 100;
    if(ch > 100) ch = 100;//超限了
    Plot_FullRect(x, y + h - ch, w, ch); //进度
    Plot_SetBrushColor(pNsDesc->Progress.cBg);//进度条后的背景
    Plot_FullRect(x, y, w, h - ch); //进度
  }
  else{//横向进度条  
    cw = ((unsigned short)cw * w) / 100;
    Plot_FullRect(x, y, cw, h); //进度
    Plot_SetBrushColor(pNsDesc->Progress.cBg);//进度条后的背景
    Plot_FullRect(x + cw, y, w - cw, h); //进度
  }
  
   Plot_SetBrushColor(PrvBg);//恢复原有背景  
}








