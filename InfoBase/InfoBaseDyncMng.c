/**********************************************************************

               InfoBase中的大容量动态区域分配实现

**********************************************************************/

#include "InfoBaseDyncMng.h"
#include "InfoBase.h"

EepromLen_t InfoBaseDyncMngBase = 0;

/***************************************************************************
                                 普通函数实现
***************************************************************************/
//-----------------------------得下余下可分配区域大小--------------------------
EepromLen_t InfoBaseDyncMng_GetFreeSize(void)
{
  return INFO_BASE_DYNC_COUNT - InfoBaseDyncMngBase;
}

//-----------------------------动态内存分配函数--------------------------
//返回基址位置，为0表示内存不够
//分配失败,可通过InfoBaseDyncMng_GetFreeSize()查询可用个数以减少支出
EepromLen_t InfoBaseDyncMng_Malloc(unsigned char ClassSize, //类型大小
                                   unsigned short Count)    //类型个数
{
  EepromLen_t NextBase = InfoBaseDyncMngBase + (ClassSize * Count);
  if(NextBase > INFO_BASE_DYNC_COUNT){//不够分配了
    InfoBaseDyncMng_cbOvNotify();//通报
    return 0; 
  }
  EepromLen_t CurBase = InfoBaseDyncMngBase;
  InfoBaseDyncMngBase = NextBase;
  
  return CurBase + INFO_BASE_DYNC_BASE; //加上基址
}


