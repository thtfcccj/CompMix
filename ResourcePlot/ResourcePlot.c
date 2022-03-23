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
void RP_PaintIcon(unsigned char Handle,const struct _RpIconDesc *pDesc,
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
void RP_PaintPara(unsigned char Handle,const struct _RpParaDesc *pDesc,
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

//----------------------------�̶������ͼ------------------------------
//�������������ֲ�
void RP_FixArea(unsigned char Handle,
                 unsigned char PlotMask,//0b���У�1bͼ��,2b����
                 const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  Plot_SetBrushColor(pDesc->cBg);   
  if(PlotMask & 0x01){//���Ʊ���
    Plot_FullRect(x, y,pDesc->Rect.w,pDesc->Rect.h);
    PlotMask = 0xff;//������
  }
  if(PlotMask & 0x02){//��������ͼ��
    unsigned char Base = pDesc->IconBase;
    unsigned char Count = pDesc->IconCount + Base;
    for(; Base < Count; Base++){
      RP_PaintIcon(Handle, RP_cbGetIconDesc(Handle, Base), x, y);
    }
  }
  if(PlotMask & 0x04){//�������в���
    unsigned char Base = pDesc->ParaBase;
    unsigned char Count = pDesc->ParaCount + Base;
    for(; Base < Count; Base++){
       RP_PaintPara(Handle, RP_cbGetParaDesc(Handle, Base), x, y);
    }
  }
}

//-----------------------�̶��������ĳ��ָ��ͼ��------------------------------
//������������
void RP_FixAreaIcon(unsigned char Handle,
                     unsigned char IconId,//ͼ��ID��
                     const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  //����ͼ��Desc
  unsigned char Base = pDesc->IconBase;
  unsigned char Count = pDesc->IconCount + Base;
  for(; Base < Count; Base++){
    const struct _RpIconDesc *pDesc = RP_cbGetIconDesc(Handle, Base);
    if((pDesc != NULL) && ((pDesc->Info & RP_ICON_ID_MASK) == IconId)){
      RP_PaintIcon(Handle, pDesc, x, y); //ƥ���ˣ���ͼ
      return;
    }
  }
}

//-----------------------�̶��������ĳ��ָ������------------------------------
//������������
void RP_FixAreaPara(unsigned char Handle,
                     unsigned char ParaId,//ͼ��ID��
                     const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  //����ͼ��Desc
  unsigned char Base = pDesc->ParaBase;
  unsigned char Count = pDesc->ParaCount + Base;
  for(; Base < Count; Base++){
    const struct _RpParaDesc *pDesc = RP_cbGetParaDesc(Handle, Base);
    if((pDesc != NULL) && ((pDesc->Info & RP_ICON_ID_MASK) == ParaId)){
      RP_PaintPara(Handle, pDesc, x, y); //ƥ���ˣ���ͼ
      return;
    }
  }
}




