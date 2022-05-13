/************************************************************************

                         动态Id筛选器管理
//管理器用于通讯部分匹配是否对应
************************************************************************/
#ifndef __DYNA_ID_FILTER_MNG_H
#define __DYNA_ID_FILTER_MNG_H


/************************************************************************
                           相关结构
************************************************************************/

#include "DynaIdFilter.h"  //筛选器

struct _DynaIdFilterMng{
  unsigned char LutType;            //查找表类型
  struct _DynaIdFilter Filter;       //正在使用筛选器
  struct _DynaIdFilter FilterTemp;   //临时筛选器
};

/************************************************************************
                           相关函数
************************************************************************/

//-----------------------------初始化函数------------------------------
#define DynaIdFilterMng_Init(pmng) do{(pmng)->LutType = 0xff;}while(0)

//----------------------------类型是否相同------------------------------
#define DynaIdFilterMng_TypeIsSame(pmng, lutType) ((pmng)->LutType == lutType)

//---------------------------重定位函数------------------------------
//类型不相同时调用
void DynaIdFilterMng_ReLocal(struct _DynaIdFilterMng *pMng,
                              unsigned char NewLutType,
                              struct _DynaIdFilter *pNewFilter);

//----------------------------得到筛选器------------------------------
#define DynaIdFilterMng_Get(pmng) (&(pmng)->Filter)

//----------------------------得到临时筛选器------------------------------
#define DynaIdFilterMng_GetTemp(pmng) (&(pmng)->FilterTemp)

#endif //__DYNA_ID_FILTER_Mng_H


