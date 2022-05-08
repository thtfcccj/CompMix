/******************************************************************************

                         ���������ͼ�ӿ�ʵ��

******************************************************************************/
#include "RpTools.h"
#include "RpHandle.h"
#include "Plot.h"
#include <string.h>


//-------------------------------���ƽ�����-------------------------------
void RpTools_PlotProgress(const union _RpToolsDesc *pNsDesc,
                          const union _RpToolsPara *pNsPara,
                          unsigned char Info, //b7:ʵʱˢ��,b4:����
                          unsigned short x, unsigned short y)//����x,y
{
  unsigned short w = pNsDesc->Progress.w & 0x0fff;
  unsigned short h = pNsDesc->Progress.h & 0x0fff; 
  unsigned short cw = (pNsDesc->Progress.w >> 12) + 1;//�߿���
  unsigned short ch = (pNsDesc->Progress.h >> 12) + 1;//�߿�߶�
  
   //����ʵʱˢ��ʱ�����߿�  
  Color_t PrvBg = Plot_GetBrushColor(); //�ú���ָ�
  if(!(Info & 0x80) && (pNsDesc->Progress.cBg != pNsDesc->Progress.cRect)){
    Plot_SetBrushColor(pNsDesc->Progress.cRect);
    Plot_FullRect(x,y, cw, h);// ��|
    Plot_FullRect(x,y, w, ch);// ��-
    Plot_FullRect(x+w-cw, y, cw, h);// ��|
    Plot_FullRect(x,y+h-ch, w, ch); // ��-
  }
  //��������
  x += cw;
  w -= cw * 2;
  y += ch;
  h -= ch * 2;
  Plot_SetBrushColor(pNsDesc->Progress.cFg[pNsPara->Progress.FgId & 0x07]);//ǰ��
  //�õ��ٷֱ�ֵ
  cw = pNsPara->Progress.Percent;
  if(cw > 100) cw = 100;//������  
  if(Info & 0x10){//���������
    ch = ((unsigned short)cw * h) / 100;
    if(ch > 100) ch = 100;//������
    Plot_FullRect(x, y + h - ch, w, ch); //����
    Plot_SetBrushColor(pNsDesc->Progress.cBg);//��������ı���
    Plot_FullRect(x, y, w, h - ch); //����
  }
  else{//���������  
    cw = ((unsigned short)cw * w) / 100;
    Plot_FullRect(x, y, cw, h); //����
    Plot_SetBrushColor(pNsDesc->Progress.cBg);//��������ı���
    Plot_FullRect(x + cw, y, w - cw, h); //����
  }
  
   Plot_SetBrushColor(PrvBg);//�ָ�ԭ�б���  
}








