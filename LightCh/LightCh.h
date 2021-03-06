/*******************************************************************************

		                   指示灯按通道驱动模块
此模块应用无关
*******************************************************************************/
#ifndef _LIGHT_CH_H
#define	_LIGHT_CH_H

/*******************************************************************************
		                      相关函数
*******************************************************************************/
#include "IoCtrl.h"


//-----------------------------初始化----------------------------------------
#define LightCh_Init()   do{CfgLightCh(); }while(0)

//-------------------------------开灯----------------------------------------
void LightCh_On(unsigned char Ch);

//--------------------------------关灯---------------------------------------
void LightCh_Off(unsigned char Ch);

#endif
