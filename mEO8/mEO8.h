/***************************************************************************

              （多例化）扩展输出8位(Extend Output 8Bit)接口
此模块负责将缓冲的位数据，通过并口,或串口(如595移位)驱动输出
***************************************************************************/
#ifndef __M_mEO8_H
#define __M_mEO8_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/***************************************************************************
                              相关结构
***************************************************************************/

//主结构
struct _mEO8{
  unsigned char O;       //当前输出
  unsigned char PrvO;    //上次输出
};

/*******************************************************************************
                              相关函数
*******************************************************************************/

//------------------------------初始化函数------------------------------
void mEO8_Init(struct _mEO8 *pEo,
               unsigned char DefaultO);//默认输出

//-----------------------------快速任务函数-----------------------------
//最长调用时间建议<=10ms
void mEO8_FaskTask(struct _mEO8 *pEo);

//-----------------------------------刷新------------------------------
//周期调用可用于抗干扰
#define mEO8_Refresh(eo) do{(eo)->PrvO = (eo)->O + 1;}while(0)

//---------------------------------读位输出--------------------------------
//可随时读
#define mEO8_IsSet(eo, mask)  ((eo)->O & (mask))

//--------------------------------位输出操作---------------------------------
//普通写可在中断内调用，但输出有延时
#define mEO8_Set(eo, mask)  do{(eo)->O |= (mask); }while(0)
#define mEO8_Clr(eo, mask)  do{(eo)->O &= ~(mask); }while(0)

//快速写不可在中断内调用，但可立即得到输出结果
#define mEO8_SetNow(eo, mask)  do{mEO8_Set(eo, mask); mEO8_FaskTask(eo); }while(0)
#define mEO8_ClrNow(eo, mask)  do{mEO8_Clr(eo, mask); mEO8_FaskTask(eo); }while(0)

//-----------------------------读写字节输出--------------------------------
//可随时读
#define mEO8_GetAll(eo)  ((eo)->O)
//普通写可在中断内调用，但输出有延时
#define mEO8_SetAll(eo, byte)      do{(eo)->O = byte; }while(0)
//快速写不可在中断内调用，但可立即得到输出结果
#define mEO8_SetAllFast(eo, byte)  do{mEO8_SetAll(eo,byte); mEO8_FaskTask(eo); }while(0)

#endif //#define __M_mEO8_H

