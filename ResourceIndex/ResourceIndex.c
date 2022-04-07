/***********************************************************************

                  索引型资源通用操作函数实现

***********************************************************************/
#include "ResourceIndex.h"

//FLASH_CAPABILITY:  为当前FLASH容量，全局声明

//---------------------------异常时返回空资源定义------------------------
const char StrSpace[] = " ";  //空格字符" "

/***********************************************************************
                          函数实现
***********************************************************************/

//----------------------由资源ID得到数据相对位置-------------------------
unsigned short RI_GetOffPos(const unsigned char *pBase,//资源基址
                              unsigned short ResId)
{
  #if RI_HEADER_LEN == 1
    return *(pBase + ResId);
  #else // RI_HEADER_LEN == 2
    pBase += 2 * ResId; //short为单位
    #if SUPPORT_RI_MSB //高位在前
      return ((unsigned short)*pBase << 8) + *(pBase + 1);
    #else //低位在前
      return (((unsigned short)*(pBase + 1)) << 8) + *pBase;
    #endif
  #endif
}

//------------------------------由资源ID得到数据--------------------------
//未找到时返回RI_NULL
const unsigned char *RI_pGetData(const unsigned char *pBase,//资源基址
                                  unsigned char ResId)
{
  unsigned short OffPos = RI_GetOffPos(pBase, ResId);
  if(OffPos >= (FLASH_CAPABILITY - (unsigned long)pBase)) //异常
    return RI_NULL;
  return pBase + OffPos;  
}

//----------------------------由资源ID得到数据大小-------------------------
//未找到时返回0
unsigned short RI_GetSize(const unsigned char *pBase,//资源基址
                           unsigned short ResId)
{
  unsigned short CurOffPos = RI_GetOffPos(pBase, ResId);
  unsigned short NextOffPos = RI_GetOffPos(pBase, ResId + 1);
  if(CurOffPos > NextOffPos) return 0;//异常
  unsigned short Len = NextOffPos - CurOffPos; 
  if(Len >= (FLASH_CAPABILITY - (unsigned long)pBase)) return 0; //异常
  return Len;
}

//------------------------------得到资源内ID总数---------------------------
unsigned short RID_GetIdCount(const unsigned char *pBase)
{
  #if RI_HEADER_LEN == 1
    return  RI_GetOffPos(pBase, 0);
  #else // RI_HEADER_LEN == 2
    return  RI_GetOffPos(pBase, 0) / 2;
  #endif
}




