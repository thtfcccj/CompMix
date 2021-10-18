/*******************************************************************************

					            APP总体标志模块
此模块将全局杂散到的变量，统一进行管理
********************************************************************************/
#ifndef __GFLAG_H 
#define __GFLAG_H

//此模块需实现的应用层接口
#include "gFlag_App.h" //具体示例模板见gFlag_AppT.h

/****************************************************************************
                             结构定义
****************************************************************************/
struct _gFlag{
  unsigned char Bit[GFLAG_BIT_SIZE];      //位标志
  unsigned char Byte[GFLAG_BYTE_SIZE];   //通用字节标志
};
extern struct _gFlag gFlag;

/****************************************************************************
                              相关函数
****************************************************************************/
//--------------------------------初始化函数------------------------------
void gFlag_Init(void);

//--------------------------------位标志操作------------------------------
#define gFlag_BitIsSet(pos, bitMask)  (gFlag.Bit[pos] & (bitMask))
#define gFlag_SetBit(pos, bitMask)    do{gFlag.Byte[pos] |= bitMask; }while(0)  
#define gFlag_ClrBit(pos, bitMask)    do{gFlag.Byte[pos] &= ~(bitMask); }while(0)  

//-----------------------------字节位操作---------------------------------
#define gFlag_GetByte(pos)        (gFlag.Byte[pos])
#define gFlag_SetByte(pos, data)  do{gFlag.Byte[pos] = data; }while(0)                


#endif

