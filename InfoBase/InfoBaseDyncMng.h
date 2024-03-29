/***************************************************************************
                       InfoBase中的大容量动态区域分配
需要大容量动态区域分配时需此模块
此模块独立于应用，需提前在用户层定义好在InfoBase中的管理区域：
//   INFO_BASE_DYNC_BASE与INFO_BASE_DYNC_COUNT
***************************************************************************/
#ifndef __INFO_BASE_DYNC_MNG_H 
#define __INFO_BASE_DYNC_MNG_H 

#include "Eeprom.h" //EepromLen_t

extern EepromLen_t InfoBaseDyncMngBase;

/***************************************************************************
                                 普通函数
***************************************************************************/

//-----------------------------得下余下可分配区域大小--------------------------
EepromLen_t InfoBaseDyncMng_GetFreeSize(void);

//-----------------------------动态内存分配函数--------------------------
//返回基址位置，为0表示内存不够
//分配失败,可通过InfoBaseDyncMng_GetFreeSize()查询可用个数以减少支出
EepromLen_t InfoBaseDyncMng_Malloc(unsigned char ClassSize, //类型大小
                                   unsigned short Count);    //类型个数

/***************************************************************************
                              回调函数
***************************************************************************/
//----------------------------内存溢出通报回调---------------------------------
//void InfoBaseDyncMng_cbOvNotify(void);
#include "MemMng.h"  //直接实现为
#define InfoBaseDyncMng_cbOvNotify() MemMng_cbOvNotify();


#endif
