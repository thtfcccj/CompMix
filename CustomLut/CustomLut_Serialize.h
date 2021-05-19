/******************************************************************************

               CustomLut序列化(Serialize)与反序列化(Deserialize)接口

*******************************************************************************/
#ifndef _CUSTON_LUT_SERIALIZE_H
#define _CUSTON_LUT_SERIALIZE_H

/******************************************************************************
                           函数接口
*******************************************************************************/
#include "CustomLut.h"

//-------------------------------序列化函数--------------------------------
//将本地 pCustomLut->Info.Tbl[]序列化至缓冲区返回结束位置
unsigned char *CustomLut_pSerialize(const struct _CustomLut *pCustomLut,
                                     unsigned char *pData);//序列接收缓冲区

//-----------------------------反序列化函数--------------------------------
//缓冲区数据恢复到本地，返回缓冲区结束位置
const unsigned char *pCustomLut_Deserialize(struct _CustomLut *pCustomLut,
                                            const unsigned char *pData);


#endif  //#define _CUSTON_LUT_SERIALIZE_H



