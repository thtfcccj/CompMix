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
void RP_FixArea(unsigned long Handle,
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
      RP_PaintIcon(Handle, RP_FixArea_cbGetIconDesc(Handle, Base), x, y);
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




