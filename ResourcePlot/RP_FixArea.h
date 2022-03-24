/******************************************************************************

                         �̶������ͼģ��

******************************************************************************/
#ifndef __RES_FIX_H 
#define __RES_FIX_H 
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif
#include "ResourcePlot.h"


/*****************************************************************************
                               ��ͼ����
*****************************************************************************/

//----------------------------�̶������ͼ------------------------------
//�������������ֲ�
void RP_FixArea(unsigned long Handle,
                unsigned char PlotMask,//0b���У�1bͼ��,2b����
                const struct _RpFixAreaDesc *pDesc);

//-----------------------�̶��������ĳ��ָ��ͼ��------------------------------
//������������
void RP_FixAreaIcon(unsigned long Handle,
                     unsigned char IconId,//ͼ��ID��
                     const struct _RpFixAreaDesc *pDesc);

//-----------------------�̶��������ĳ��ָ������------------------------------
//������������
void RP_FixAreaPara(unsigned long Handle,
                     unsigned char ParaId,//ͼ��ID��
                     const struct _RpFixAreaDesc *pDesc);

/********************************************************************************
                           �ص�����
********************************************************************************/

//----------------------------��ͼ��AryID��ͼ��Ϣ------------------------------
const struct _RpIconDesc *RP_FixArea_cbGetIconDesc(unsigned long Handle,
                                                    unsigned char IconAryId);

#endif  //#ifndef __RESOURCE_PLOT_H 



