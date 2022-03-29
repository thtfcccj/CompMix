/************************************************************************

                         ��̬Idɸѡ������
//����������ͨѶ����ƥ���Ƿ��Ӧ
************************************************************************/
#ifndef __DYNA_ID_FILTER_MNG_H
#define __DYNA_ID_FILTER_MNG_H


/************************************************************************
                           ��ؽṹ
************************************************************************/

#include "DynaIdFilter.h"  //ɸѡ��

struct _DynaIdFilterMng{
  unsigned char LutType;            //���ұ�����
  struct _DynaIdFilter Filter;       //����ʹ��ɸѡ��
  struct _DynaIdFilter FilterTemp;   //��ʱɸѡ��
};

/************************************************************************
                           ��غ���
************************************************************************/

//-----------------------------��ʼ������------------------------------
#define DynaIdFilterMng_Init(pmng) do{(pmng)->LutType = 0xff;}while(0)

//----------------------------�����Ƿ���ͬ------------------------------
#define DynaIdFilterMng_TypeIsSame(pmng, lutType) ((pmng)->LutType == lutType)

//---------------------------�ض�λ����------------------------------
//���Ͳ���ͬʱ����
void DynaIdFilterMng_ReLocal(struct _DynaIdFilterMng *pMng,
                              unsigned char NewLutType,
                              struct _DynaIdFilter *pNewFilter);

//----------------------------�õ�ɸѡ��------------------------------
#define DynaIdFilterMng_Get(pmng) (&(pmng)->Filter)

//----------------------------�õ���ʱɸѡ��------------------------------
#define DynaIdFilterMng_GetTemp(pmng) (&(pmng)->FilterTemp)

#endif //__DYNA_ID_FILTER_Mng_H


