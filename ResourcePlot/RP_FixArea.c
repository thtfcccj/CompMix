/******************************************************************************

                         �̶������ͼʵ��

******************************************************************************/
#include "RP_FixArea.h"
#include "Plot.h"
#include "ResourceIndex.h"
#include "ePic.h"
#include "Fonts.h"
#include <string.h>

//----------------------------�̶������ͼ------------------------------
//�������������ֲ�
//PlotMask: 0b���У�1b����ͼ, 2bͼ��, 3b����, 4b����ͼ+1
void RP_FixArea(unsigned long Handle,
                 unsigned char PlotMask,
                 const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  Plot_SetBrushColor(pDesc->cBg);   
  if(PlotMask & 0x01){//���Ʊ���
    Plot_FullRect(x, y,pDesc->Rect.w,pDesc->Rect.h);
    PlotMask |= 0x0f;//������
  }
  if((PlotMask & 0x02) && (pDesc->IconBgId)){//���Ʊ���ͼ
    unsigned char IconBgId = pDesc->IconBgId;
    if(PlotMask & 0x10) IconBgId++; //�ڶ�����
    const unsigned char *ePic = RP_cbGetIcon(Handle,IconBgId);  
    if(ePic != RI_NULL){//�ҵ���
      Plot_SetPenColor(RP_cbGetIconFg(Handle,IconBgId));
      ePic_ePicBuf(ePic, RP_cbGetIconSize(Handle,IconBgId));//��ʼ��ePic
      ePic_Plot(x,y);//��ͼ
      PlotMask = 0x0f;//��������
    }
  }
  if(PlotMask & 0x04){//��������ͼ��
    unsigned char Base = pDesc->IconBase;
    unsigned char Count = pDesc->IconCount + Base;
    for(; Base < Count; Base++){
      RP_PaintIcon(Handle, RP_FixArea_cbGetIconDesc(Handle, Base), x, y);
    }
  }
  if(PlotMask & 0x08){//�������в���
    unsigned char Base = pDesc->ParaBase;
    unsigned char Count = pDesc->ParaCount + Base;
    for(; Base < Count; Base++){
       RP_PaintPara(Handle, RP_cbGetParaDesc(Handle, Base), x, y);
    }
  }
}

//-----------------------�̶��������ĳ��ָ��ͼ��------------------------------
//������������
void RP_FixAreaIcon(unsigned long Handle,
                     unsigned char IconId,//ͼ��ID��
                     const struct _RpFixAreaDesc *pDesc)
{
  unsigned short x = pDesc->Rect.x;
  unsigned short y = pDesc->Rect.y;  
  
  //����ͼ��Desc
  unsigned char Base = pDesc->IconBase;
  unsigned char Count = pDesc->IconCount + Base;
  for(; Base < Count; Base++){
    const struct _RpIconDesc *pDesc = RP_FixArea_cbGetIconDesc(Handle, Base);
    if((pDesc != NULL) && ((pDesc->Info & RP_ICON_ID_MASK) == IconId)){
      RP_PaintIcon(Handle, pDesc, x, y); //ƥ���ˣ���ͼ
      return;
    }
  }
}

//-----------------------�̶��������ĳ��ָ������------------------------------
//������������
void RP_FixAreaPara(unsigned long Handle,
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




