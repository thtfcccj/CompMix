/******************************************************************************

                      ��Դ��Ϣ��ͼģ����غ���ʵ��


******************************************************************************/
#include "ResourcePlot.h"
#include "Plot.h"
#include "ResourceIndex.h"
#include "ePic.h"
#include "Fonts.h"
#include <string.h>

//-----------------------ָ��λ��ͼ���ͼ-----------------------------
//����ĳ������ͼ��
void RP_PaintIcon(unsigned long Handle,const struct _RpIconDesc *pDesc,
                  unsigned short x, unsigned short y)//���x,y
{
  if(pDesc == NULL) return; //�쳣
  const unsigned char *ePic = RP_cbGetIcon(Handle,
                                            pDesc->Info & RP_ICON_ID_MASK);  
  if(ePic == RI_NULL) return;//�쳣δ�ҵ�
  //�ҵ�x,y����ɫ����Ϣ
  //���õ�ɫ��,����ǰԤ�ñ���ɫ
  unsigned char IconId = pDesc->Info & RP_ICON_ID_MASK;
  Plot_SetPenColor(RP_cbGetIconFg(Handle,IconId));
  ePic_ePicBuf(ePic, RP_cbGetIconSize(Handle,IconId));//��ʼ��ePic
  x += pDesc->xl;
  if(pDesc->Info & RP_ICON_X9) x+= 256;
  y += pDesc->yl;
  if(pDesc->Info & RP_ICON_Y9) x+= 256;  
  ePic_Plot(x,y);//��ͼ
}

//----------------------ָ��λ�ò�����ͼ-----------------------------
//����ĳ���������
void RP_PaintPara(unsigned long Handle,const struct _RpParaDesc *pDesc,
                  unsigned short x, unsigned short y)//���x,y
{
  if(pDesc == NULL) return; //�쳣
  unsigned char ParaId = pDesc->Info & RP_PARA_ID_MASK;  
  const char *pSring = RP_cbGetString(Handle,ParaId);  
  if(pSring == NULL) return;//�쳣δ�ҵ�
  //�ҵ�x,y����ɫ����Ϣ
  //���õ�ɫ��,����ǰԤ�ñ���ɫ
  Plot_SetPenColor(RP_cbGetParaFg(Handle,ParaId));
  x += pDesc->xl;
  if(pDesc->Info & RP_PARA_X9) x+= 256;
  y += pDesc->yl;
  if(pDesc->Info & RP_PARA_Y9) x+= 256;  

  //��֧�ְ������ѡ��
  unsigned char fFontId = pDesc->Para & RP_PARA_FONT_MASK;
  if(fFontId) fFontId++;//1��ȫ�����壬Ҫ�ܿ���
  
  const struct _FontsDesc *pFontBase = (const struct _FontsDesc*)FONTS_DESC_BASE;
  Font_PlotLine(pFontBase + fFontId,//���ʱʹ�õ�����
                pFontBase + 1, //ȫ��ʱʹ�õ�����,�̶�Ϊ1
                x,y,pSring);
}




