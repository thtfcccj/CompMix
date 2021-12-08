/******************************************************************************

                     压缩时间模块-SMenu参数接口

*******************************************************************************/
#ifndef _ZIP_TIME_SMENU_H
#define _ZIP_TIME_SMENU_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

#include "SMenuUser.h"

/******************************************************************************
                         SMenu参数接口
*******************************************************************************/
//----------------------------年修改--------------------------------
extern const struct _SMenuFun SMenu_ZipTime_Year;

//----------------------------月修改--------------------------------
extern const struct _SMenuFun SMenu_ZipTime_Mouth;

//----------------------------日修改--------------------------------
extern const struct _SMenuFun SMenu_ZipTime_Day;

//----------------------------时修改--------------------------------
extern const struct _SMenuFun SMenu_ZipTime_Hour;

//---------------------------分修改--------------------------------
//同时秒归零
extern const struct _SMenuFun SMenu_ZipTime_Minte;

#endif



