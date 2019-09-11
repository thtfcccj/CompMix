/***********************************************************************

                                  API接口
//此实现应用相关
***********************************************************************/
#ifndef __API_H
#define	__API_H

#include "MultitonAPI.h"
#include "SingletonAPI.h"

/***********************************************************************
                             相关函数
***********************************************************************/

//---------------------由类型获得双字节对像MultitonAPI函数接口---------------
//返回NULL表示不支持的类型或阵列ID超限
const struct _MultitonAPI *API_pGetMultitonD(unsigned char TypeId,
                                              unsigned short AryId);

//---------------------由类型获得单字节对像MultitonAPI函数接口---------------
//返回NULL表示不支持的类型或阵列ID超限
const struct _MultitonAPI *API_pGetMultitonS(unsigned char TypeId,
                                              unsigned char AryId);

//----------------------由类型获得单例化对像SingletonAPI函数接口-------------
//返回NULL不支持
const struct _SingletonAPI *API_pGetSingleton(unsigned char TypeId);

//--------------------------多例化API数据同步通报------------------------
//写数据时，返回是否写数据中(此时应禁止更新本地数据)
//实现应用相关
signed char  API_MultitonSync(const struct _MultitonAPI *pApi,
                              unsigned char TypeId,
                              unsigned short AryId, //阵列ID
                              unsigned char MsgType); //NET_FUN_MSG_TYPE_MASK消息类型


#endif //#define	__API_DATA_INFO_H

