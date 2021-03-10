/***********************************************************************

                    用户可定制查找表模块实现
此模块独立与系统,即与应用无关
注:仅支持单调上升的查找表结构
***********************************************************************/
#include "CustomLutAry.h"

struct _CustomLutAry CustomLutAry; //实例化

/********************************************************************************
                              相关函数实现
********************************************************************************/

//--------------------------------初始化函数------------------------------------
void CustomLutAry_Init(signed char Inited)//形参为已初始化标志
{
  for(unsigned char Id = 0; Id < CUSTOM_LUT_ARY_SIZE; Id++){
    CustomLut_Init(&CustomLutAry.Ary[Id],
                   Id,//此表的ID号，用于对外区分结构
                   Inited);//形参为已初始化标志
  }
}

//------------------------------由Id得到实例------------------------------------
//未找到时为首个
struct _CustomLut* CustomLutAry_pGet(unsigned char Id)
{
  if(Id >= CUSTOM_LUT_ARY_SIZE) Id = CUSTOM_LUT_ARY_SIZE - 1;
  return &CustomLutAry.Ary[Id];
}

//------------------------------由Id直接得到查找表------------------------------
//未找到时为首个
struct _NolinearConvertTbl *CustomLutAry_pGetLut(unsigned char Id)
{
  return CustomLutAry_pGet(Id)->Info.Tbl;
}