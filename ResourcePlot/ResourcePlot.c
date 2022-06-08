/******************************************************************************

                      ��Դ��Ϣ��ͼģ����غ���ʵ��


******************************************************************************/
#include "ResourcePlot.h"
#include "Plot.h"
#include "ResourceIndex.h"
#include "ePic.h"
#include "Fonts.h"
#include <string.h>

//--------------------------�õ��������-------------------------------
static unsigned short _GetX(const struct _RpParaDesc *pDesc)
{
  unsigned short x = pDesc->xl; //��8λ
  if(pDesc->Info & RP_ICON_X9) x += 256;  //��1λ
  return x * RP_X_MUTI; //���λ�ñ���
}

//--------------------------�õ��������-------------------------------
static unsigned short _GetY(const struct _RpParaDesc *pDesc)
{
  unsigned short y = pDesc->yl; //��8λ
  if(pDesc->Info & RP_ICON_Y9) y += 256;  //��1λ
  return y * RP_Y_MUTI; //���λ�ñ���
}

//-----------------------ָ��λ��ͼ���ͼ-----------------------------
//����ĳ������ͼ��
void RP_PaintIcon(unsigned long Handle,const struct _RpIconDesc *pDesc,
                  unsigned short x, unsigned short y)//���x,y
{
  if(pDesc == NULL) return; //�쳣
  unsigned char IconId = pDesc->Info & RP_ICON_ID_MASK;  
  const unsigned char *ePic = RP_cbGetIcon(Handle, IconId);  
  if((ePic == NULL) || (ePic == RI_NULL)) return;//�쳣δ�ҵ�
  //�ҵ�x,y����ɫ����Ϣ
  //���õ�ɫ��,����ǰԤ�ñ���ɫ
  
  Plot_SetPenColor(RP_cbGetIconFg(Handle,IconId));
  ePic_ePicBuf(ePic, RP_cbGetIconSize(Handle,IconId));//��ʼ��ePic
  ePic_Plot(x + _GetX((const struct _RpParaDesc *)pDesc), 
            y + _GetY((const struct _RpParaDesc *)pDesc));//��ͼ
}

//----------------------ָ��λ�ò�����ͼ-----------------------------
//����ĳ���������
void RP_PaintPara(unsigned long Handle,const struct _RpParaDesc *pDesc,
                  unsigned short x, unsigned short y)//���x,y
{
  if(pDesc == NULL) return; //�쳣
  x += _GetX(pDesc);  //x����λ��
  y += _GetY(pDesc);  //y����λ�� 
  unsigned char ParaId = pDesc->Info & RP_PARA_ID_MASK;
  unsigned char Para = pDesc->Para;
  unsigned char mType = Para & RP_PARA_TYPE_MASK; 
  //===========================�ַ����ͻ���=============================  
  if((mType == RP_PARA_TYPE_STRING) || (mType == RP_PARA_TYPE_STRING1)){
    const char *pSring = RP_cbGetString(Handle,ParaId);  
    if(pSring == NULL) return;//�쳣δ�ҵ�
    //���õ�ɫ��,����ǰԤ�ñ���ɫ
    Plot_SetPenColor(RP_cbGetStringFg(Handle,ParaId));
    //��̬����ɫʱ�õ�ǰʹ�õı���ɫ
    Color_t prvBg;
    if(mType == RP_PARA_TYPE_STRING1){
      prvBg = Plot_GetBrushColor();
      Plot_SetBrushColor(RP_cbGetStringBg(Handle,ParaId));
    }
    //ת��FontInfo
    unsigned char FontInfo = ((pDesc->Para & 0x0C) << 2);//���뷽ʽ
    if(pDesc->Para & RP_PARA_FONT_MUTI2) FontInfo += 2;//�����Ŵ�
    else FontInfo += 1;//1���Ŵ�
    unsigned char MaxLen = RP_cbGetStrMaxLen(Handle,ParaId);
    
    //��֧�ְ������ѡ��
    unsigned char fFontId = pDesc->Para & RP_PARA_FONT_MASK;
    if(fFontId) fFontId++;//1��ȫ�����壬Ҫ�ܿ���    
    const struct _FontsDesc *pFontBase = (const struct _FontsDesc*)FONTS_DESC_BASE;
    Font_PlotLine(pFontBase + fFontId,//���ʱʹ�õ�����
                  pFontBase + 1, //ȫ��ʱʹ�õ�����,�̶�Ϊ1
                  x,y,pSring, MaxLen, FontInfo);
    if(mType == RP_PARA_TYPE_STRING1) //��̬����ɫʱ�ָ�ԭ�еı���ɫ
      Plot_SetBrushColor(prvBg);
    return;
  }
  if(mType != RP_PARA_TYPE_TOOLS) return; //�����ݲ�֧��
  
  //============================������������===========================
  unsigned char ToolsType = Para & RP_PARA_TYPE_PARA;
  if(ToolsType >= RP_TOOLS_COUNT)  return;//��֧�ֵĹ�����
  //��ǰ��ȡ
  const union _RP_NsDesc *pNsDesc = RP_pcbGetNsDesc(Handle, Para, ParaId);
  if(pNsDesc == NULL) return;//�쳣
  const union _RP_NsPara *pNsPara = RP_pcbGetNsPara(Handle, Para, ParaId); 
  if(pNsPara == NULL) return;//�쳣
  
  if(Handle & RP_HANDLE_REAL_REFRESH) ToolsType |= 0x80;//�Ƿ�Ϊʵʱˢ��
  switch(ToolsType){
    case RP_TOOLS_PROGRESS: //��������ͼ
      RpTools_PlotProgress(&pNsDesc->Tools, &pNsPara->Tools, ToolsType, x, y); 
      break;
    default: break;
  }
}








