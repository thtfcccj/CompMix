/******************************************************************************

                      ��Դ��Ϣ��ͼģ����غ���ʵ��


******************************************************************************/
#include "ResourcePlot.h"
#include "Plot.h"
#include "ResourceIndex.h"
#include "ePic.h"
#include <string.h>

/***********************************************************************
                        �̶������ͼ���-���ӿں�����ص�����
***********************************************************************/

//-------------------------�̶�����ͼ���ͼ-----------------------------
//����ĳ������ͼ��
void RP_PaintFixAreaIcon(unsigned char Handle,
                         unsigned char IconAryId)
{
  const struct _RpIconDesc *pDesc = RP_cbGetIconDesc(Handle, IconAryId);
  if(pDesc == NULL) return; //δ�ҵ������
  unsigned char IconId = pDesc->Info & RP_PARA_ID_MASK;
  const unsigned char *ePic = RP_cbGetIcon(Handle,IconId);  
  if(ePic == RI_NULL) return;//�쳣δ�ҵ�
  //�ҵ�x,y����ɫ����Ϣ
  //���õ�ɫ��,����ǰԤ�ñ���ɫ
  Plot_SetPenColor(RP_cbGetIconFg(Handle,IconId));
  ePic_ePicBuf(ePic);//��ʼ��ePic
  unsigned short x = pDesc->xl;
  if(pDesc->Info & RP_PARA_X9) x+= 256;
  unsigned short y = pDesc->yl;
  if(pDesc->Info & RP_PARA_Y9) x+= 256;  
  ePic_Plot(x,y);//��ͼ
}

//----------------------------�̶������ͼ------------------------------
//�������������ֲ�
void RP_FixArea(unsigned char Handle,
                     unsigned char PlotMask,//��������7b����,0-6b����0x7f����
                     const struct _RpFixAreaDesc *pDesc)
{
  Plot_SetBrushColor(pDesc->cBg);  
  if(PlotMask & 0x01){//���Ʊ���
    Plot_FullRect(pDesc->Rect.x, pDesc->Rect.y,
                  pDesc->Rect.w,pDesc->Rect.h);
  }
  if(PlotMask & 0x02){//��������ͼ��
    unsigned char Base = pDesc->IconBase;
    unsigned char Count = pDesc->IconCount + Base;
    for(; Base < Count; Base++){
      RP_PaintFixAreaIcon(Handle, Base);
    }
  }
  if(PlotMask & 0x04){//�������в���

  }  
  
}




