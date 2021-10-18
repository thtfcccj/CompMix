/***********************************************************************

		                    SWD复用IO口切换模块
此模块可用于SWD与IO口之间的切换，也可用于通过关闭SWD来加解密
此模块接口统一，实现MCU硬件相关
***********************************************************************/
#ifndef _SWD_IO_H
#define	_SWD_IO_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"   //__DEBUG
#endif

/***********************************************************************
		                      相关配置
***********************************************************************/

//支持SWD通过IO解锁时定义
//#define SUPPORT_SWD_IO_UNLOCK

//通过识别IO口(如按键)来解开SWD功能时，持续时间，超过此时间恢复为SWD功能
//解锁后，须重启才可加锁，以防止复用的IO口为其它功能时失效
#ifndef SWD_IO_UNLICK_TIME  
  #define SWD_IO_UNLICK_TIME   (4 * 10)       
#endif

/***********************************************************************
		                      相关结构
***********************************************************************/

#ifdef  SUPPORT_SWD_IO_UNLOCK //支持SWD通过IO解锁时
  extern unsigned char SwdIo_LockTimer;
#endif

/***********************************************************************
		                      相关函数
***********************************************************************/

//---------------------SWD口配置为普通IO-------------------------------
//配置为IO模式, 应在初始化时优先调用，建议此
#ifndef __DEBUG   //正常工作时
  void SwdIo_ToIo(void); 
#else  //调试模式无效
  #define SwdIo_ToIo() do{}while(0)
#endif

//---------------------SWD配置为普通IO后置输出0-------------------
//应在初始化时调用SwdIo_ToIo()后，若口不用,调用此函数配置回输出0模式
#ifndef __DEBUG  //正常工作时
  void SwdIo_IoOut0(void); 
#else  //调试模式无效
  #define SwdIo_IoOut0() do{}while(0)
#endif

//-------------------------------配置回SWD模式---------------------------
//同时将SWD口配置为输入模式以用于SWD接入
#ifndef __DEBUG //正常工作时  
  void SwdIo_ToSwd(void);
#else  //调试模式无效
  #define SwdIo_ToSwd() do{}while(0)
#endif

//----------------------普通任务函数-------------------------------
//放入256ms进程中扫描
#ifdef  SUPPORT_SWD_IO_UNLOCK //支持SWD通过IO解锁时
  void SwdIo_Task(void);
#else
  #define SwdIo_Task() do{}while(0)
#endif

//-----------------------------复位计数------------------------------
//复位计数，用于手动触发IO解锁模块生新计时
#ifdef  SUPPORT_SWD_IO_UNLOCK //支持SWD通过IO解锁时
  #define SwdIo_ResetTimer() do{SwdIo_LockTimer = SWD_IO_UNLICK_TIME;}while(0)
#else
  #define SwdIo_ResetTimer() do{}while(0)
#endif

/***********************************************************************
		                     回调函数
***********************************************************************/

//----------------------解锁键是否为无效状态--------------------------
#ifdef  SUPPORT_SWD_IO_UNLOCK //支持SWD通过IO解锁时
  #include "IoCtrl.h"
  #define SwdIo_cb_IsInvalid() UnLock_IsInvalid()
#endif

#endif
  
  
  
  
