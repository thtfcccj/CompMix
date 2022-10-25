/***********************************************************************

		                   显示地址倒计时器
此模块仅负责维护显示地址定时器，不负责具体显示，应用无关
***********************************************************************/
#ifndef _DISP_ADR_TIMER_H
#define	_DISP_ADR_TIMER_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/***********************************************************************
		                      相关配置
***********************************************************************/

//是否支持此模块
//#define SUPPORT_DISP_ADR_TIMER   

//定义持续时间,256ms为单位
#ifndef DISP_ADR_TIMER_COUNT
  #define	DISP_ADR_TIMER_COUNT  (4 * 10)     
#endif
                            
/***********************************************************************
		                      相关结构
***********************************************************************/

extern unsigned char DispAdrTimer; 

//放在main中声明
#define DECLEAR_DISP_ADR_TIMER   unsigned char DispAdrTimer = 0;

/***********************************************************************
		                      相关函数
***********************************************************************/

//----------------------普通任务函数-------------------------------
//放入256ms进程中扫描
#define DispAdrTimer_Task()  do{if(DispAdrTimer) DispAdrTimer--;}while(0)

//-----------------------------启动函数----------------------------
#define DispAdrTimer_Start()  do{DispAdrTimer = DISP_ADR_TIMER_COUNT;}while(0)
//指定秒启动
#define DispAdrTimer_StartS(sec)  do{DispAdrTimer = (sec) * 4;}while(0)

//-------------------------是否在显示状态---------------------------
#define DispAdrTimer_IsDisp()   (DispAdrTimer)

//-------------------------主动结束显示---------------------------
#define DispAdrTimer_EndDisp()    do{DispAdrTimer = 0;}while(0)

#endif
