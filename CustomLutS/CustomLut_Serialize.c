/******************************************************************************

               CustomLut序列化(Serialize)与反序列化(Deserialize)实现

*******************************************************************************/

#include "CustomLut_Serialize.h"

//-------------------------------序列化函数--------------------------------
//将本地 pCustomLut->Info.Tbl[]序列化至缓冲区返回结束位置
void CustomLut_Serialize(unsigned char *pData)//序列接收缓冲区
{
  const struct _NolinearConvertTbl *pTbl = CustomLut;
  for(unsigned char Count = CUSTOM_LUT_LUT_SIZE; Count > 0; Count--){
    *pData++ = pTbl->Source >> 8;
    *pData++ = pTbl->Source & 0xff;
    *pData++ = pTbl->Destination >> 8;
    *pData++ = pTbl->Destination & 0xff;
    pTbl++;
  }
}

//-----------------------------反序列化函数--------------------------------
//缓冲区数据恢复到本地
void CustomLut_Deserialize(const unsigned char *pData)
{
  struct _NolinearConvertTbl *pTbl = CustomLut;
  for(unsigned char Count = CUSTOM_LUT_LUT_SIZE; Count > 0; Count--){
    pTbl->Source = ((unsigned short)*pData << 8) | *(pData + 1); 
    pData += 2;
    pTbl->Destination = ((unsigned short)*pData << 8) | *(pData + 1); 
    pData += 2;
    pTbl++;
  }
}


