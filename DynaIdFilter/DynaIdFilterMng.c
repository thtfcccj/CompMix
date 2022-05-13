/************************************************************************

                         查动态Id筛选器管理实现

************************************************************************/
#include "DynaIdFilterMng.h"

/************************************************************************
                           相关函数实现
************************************************************************/

//---------------------------重定位函数------------------------------
//类型不相同时调用
void DynaIdFilterMng_ReLocal(struct _DynaIdFilterMng *pMng,
                              unsigned char NewLutType,
                              struct _DynaIdFilter *pNewFilter)
{
  if(!DynaIdFilter_IsValid(pNewFilter)) return; //无效时就不用缓冲了                         
  //缓冲
  pMng->LutType =  NewLutType;
  //变量少直接变量copy
  pMng->Filter.UserPos = pNewFilter->UserPos;
  pMng->Filter.LocalPos = pNewFilter->LocalPos;  
}