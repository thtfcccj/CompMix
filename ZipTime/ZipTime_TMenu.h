/*******************************************************************************

                    压缩时间设置-在TMenu系统中的菜单实现
此模块通过回调独立于应用
*******************************************************************************/

#ifndef _ZIP_TIME_TMENU_H
#define _ZIP_TIME_TMENU_H

#include "TMenu.h"

/*******************************************************************************
                             对外输出
*******************************************************************************/
extern const TMenu_t ZipTime_TMenu; //集中保存菜单结构
extern const TMenu_t ZipTimeS_TMenu;//分别保存菜单结构

/*******************************************************************************
                             回调函数
*******************************************************************************/

//----------------------------自已的父菜单----------------------------------
//集中保存
#ifndef ZIP_TIME_TMENU_PARENT 
  extern const TMenu_t Sys_TMenu;
  #define  ZIP_TIME_TMENU_PARENT  &Sys_TMenu
#endif
//分别保存
#ifndef ZIP_TIME_TMENU_PARENT2
  extern const TMenu_t Popular_TMenu;
  #define  ZIP_TIME_TMENU_PARENT2  &Popular_TMenu
#endif
  

#endif