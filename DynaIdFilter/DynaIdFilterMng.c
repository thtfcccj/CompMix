/************************************************************************

                         �鶯̬Idɸѡ������ʵ��

************************************************************************/
#include "DynaIdFilterMng.h"

/************************************************************************
                           ��غ���ʵ��
************************************************************************/

//---------------------------�ض�λ����------------------------------
//���Ͳ���ͬʱ����
void DynaIdFilterMng_ReLocal(struct _DynaIdFilterMng *pMng,
                              unsigned char NewLutType,
                              struct _DynaIdFilter *pNewFilter)
{
  if(!DynaIdFilter_IsValid(pNewFilter)) return; //��Чʱ�Ͳ��û�����                         
  //����
  pMng->LutType =  NewLutType;
  //������ֱ�ӱ���copy
  pMng->Filter.UserPos = pNewFilter->UserPos;
  pMng->Filter.LocalPos = pNewFilter->LocalPos;  
}