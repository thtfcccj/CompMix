/*******************************************************************************

                    ѹ��ʱ������-��TMenuϵͳ�еĲ˵�ʵ��
��ģ��ͨ���ص�������Ӧ��
*******************************************************************************/

#ifndef _ZIP_TIME_TMENU_H
#define _ZIP_TIME_TMENU_H

#include "TMenu.h"

/*******************************************************************************
                             �������
*******************************************************************************/
extern const TMenu_t ZipTime_TMenu; //�˵��ṹ

/*******************************************************************************
                             �ص�����
*******************************************************************************/

//----------------------------���ѵĸ��˵�----------------------------------
#ifndef ZIP_TIME_TMENU_PARENT
  extern const TMenu_t Sys_TMenu;
  #define  ZIP_TIME_TMENU_PARENT  &Sys_TMenu
#endif

//-------------------------------���õ�ǰʱ��----------------------------------
//void Menu_ZipTime_cbSet(unsigned long ZipTime);
  
#endif