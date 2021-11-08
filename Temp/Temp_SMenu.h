/******************************************************************************

                      温度(单例化)模块-SMenu接口

*******************************************************************************/
#ifndef __TEMP_SMENU_H
#define __TEMP_SMENU_H

#include "SMenuUser.h"

/******************************************************************************
                           SMenu接口
*******************************************************************************/
//----------------------------温度显示与标定------------------------------
//正常时显示当前带符号温度值, 进入标定模式后：
//直接输入目标温度值： 为将当前温度清零到此值, 如输入20表示清零到20度
//输入目标温度值+ 500：为将当前温度标定到此值(两点标定)  如：
//   输入460: 表示标定至-40度
//   输入570: 表示标定至70度
extern const struct _SMenuFun SiSMenu_Temp; 

#endif //__TEMP_SMENU_H









