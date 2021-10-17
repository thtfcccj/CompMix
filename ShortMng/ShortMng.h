/********************************************************************************

                      短路管理模块

此模块配合自恢复保险使用，在其保护后断开负载，以使自恢复保险能恢复并重新接入
*******************************************************************************/
#ifndef _SHORT_MNG_H
#define _SHORT_MNG_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif
/******************************************************************************
                        相关配置
******************************************************************************/
#ifndef SHORT_MNG_WAIT_OV
  #define SHORT_MNG_WAIT_OV   2  //接入负载后，等待检测短路的时间,s为单位
#endif
#ifndef SHORT_MNG_NOR_OV
  #define SHORT_MNG_NOR_OV    3  //检查到短路时，认为还没有恢复的时间,s为单位
#endif
#ifndef SHORT_MNG_PTC_WAIT
  #define SHORT_MNG_PTC_WAIT  20  //自恢复保险恢复时间,s为单位
#endif

/******************************************************************************
                        相关结构
******************************************************************************/

struct _ShortMng{
  unsigned char Flag;  //相关状态，见定义
  unsigned char Timer;  //定时器
};

//相关状态定义为:
#define SHORT_MNG_STATE_MASK  0x07   //工作状态，定义为:
#define SHORT_MNG_STATE_IDIE     0   //等待(分电上电)接入
#define SHORT_MNG_STATE_WAIT     1   //已接入，等待稳定
#define SHORT_MNG_STATE_SHUT     2   //闭合状态，即正常工作
#define SHORT_MNG_STATE_BREAK    3   //断开状态，即保护，并等待恢复

#define SHORT_MNG_PROTECT_COUNT_SHIFT  4     //连续保护次数移位值
#define SHORT_MNG_PROTECT_COUNT_MASK  0xF0   //连续保护次数掩码

/*******************************************************************************
                        相关函数
*******************************************************************************/

//---------------------------------初始化-----------------------------
//void ShortMng_Init(struct _ShortMng *pPow);
#define ShortMng_Init(pow, wait)  do{(pow)->Flag = 0;  (pow)->Timer = wait;}while(0)

//-------------------------------任务函数------------------------------
//放入1S附近任务中扫描
void ShortMng_Task(struct _ShortMng *pPow, unsigned char Ch); 

//-------------------------------置为掉电模式------------------------------
void ShortMng_ShutDown(struct _ShortMng *pPow, unsigned char Ch);//对应通道

//-----------------------------得到当前状态-----------------------------
#define ShortMng_GetState(pow)  ((pow)->Flag & SHORT_MNG_STATE_MASK)

//-----------------------------是否在接入状态-----------------------------
//等待与正常时在接入态,否则为非接入态
#define ShortMng_IsShut(state) \
  (((state) == SHORT_MNG_STATE_WAIT) || ((state) == SHORT_MNG_STATE_SHUT))

//-----------------------------是否在短路状态-----------------------------
#define ShortMng_IsShort(pow) ((pow)->Flag & SHORT_MNG_PROTECT_COUNT_MASK) 
    
//-----------------------------得到连续短路次数-----------------------------
#define ShortMng_GetShortCount(pow) ((pow)->Flag >> SHORT_MNG_PROTECT_COUNT_SHIFT)     
   
/*******************************************************************************
                          回调函数
*******************************************************************************/
    
//----------------------------对应通道是否短路------------------------------
unsigned char ShortMng_cbIsShort(unsigned char Ch);

//----------------------------闭合对应通道------------------------------
//即允许供电
void ShortMng_cbShut(unsigned char Ch);

//----------------------------断开对应通道------------------------------
//即断开供电
void ShortMng_cbBreak(unsigned char Ch);

//----------------------------状态改变通报------------------------------
void ShortMng_cbStateNotify(unsigned char Ch,
                             unsigned char PrvState,
                             unsigned char CurState);




#endif
