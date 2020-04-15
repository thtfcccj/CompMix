/***********************************************************************

                  多例模式对像/结构阵列统一访问接口定义
//对像数量应<65536
***********************************************************************/
#ifndef __MULTITON_API_H
#define	__MULTITON_API_H

#include "ApiId.h" //ApiHandle_t

/*******************************************************************************
                            接口函数原型定义
*******************************************************************************/

//-----------------得到读写数据(一般为配置，有时称为对称性数据)-------------------
typedef void (*MultitonAPI_GetData_t)(unsigned char *pData,//接收据缓冲区
                                      ApiHandle_t Handle, //句柄+功能ID
                                      unsigned short AryId);//多例对像ID号

//-----------------设置读写数据(一般为配置，有时称为对称性数据)--------------
//返回写结果
typedef unsigned char (*MultitonAPI_SetData_t)(const unsigned char *pData,//对应数据
                                      ApiHandle_t Handle,//句柄+功能ID
                                      unsigned short AryId);//多例对像ID号

//----------------------------得到只读性数据-----------------------------------
typedef void (*MultitonAPI_GetDataOnly_t)(unsigned char *pData,//接收据缓冲区
                                          ApiHandle_t Handle, //句柄+功能ID
                                          unsigned short AryId);//多例对像ID号                                  

//----------------------------执行本地指令-----------------------------------
//返回执行结果(低3Bit)+已使用参数长度(高5Bit)
typedef unsigned char (*MultitonAPI_ExeCmd_t)(ApiHandle_t Handle,//句柄+功能ID
                                              const unsigned char *pPara,//参数，无时为NULL
                                              unsigned short AryId);//多例对像ID号

//-------------------------得到数据最小值-------------------------------
typedef void (*MultitonAPI_GetDataMin_t)(unsigned char *pData,//接收据缓冲区
                                         ApiHandle_t Handle,//句柄+功能ID
                                         unsigned short AryId);//多例对像ID号

//-------------------------得到数据最大值-------------------------------
typedef void (*MultitonAPI_GetDataMax_t)(unsigned char *pData, //接收据缓冲区
                                         ApiHandle_t Handle,//句柄+功能ID
                                         unsigned short AryId);//多例对像ID号

//------------------------------得到标题头----------------------------------
//返回标题头字符串
typedef const char *(*MultitonAPI_GetHeader_t)(unsigned char IsOnly,  //读写数据(0)or只读数据
                                               ApiHandle_t Handle);//句柄+功能ID  
                       
//--------------------------------得到内容数据----------------------------------
//返回字符串
typedef const char *(*MultitonAPI_GetContent_t)(char *pBuf,  //可能用到的缓冲区
                                  unsigned char IsOnly,  //读写数据(0)or只读数据
                                  ApiHandle_t Handle,  //句柄+功能ID
                                  unsigned short AryId); //多例对像ID号

//--------------------------------得到数据类型----------------------------------
//返回ApiIdType标识
typedef unsigned char (*MultitonAPI_GetType_t)(unsigned char IsOnly,  //读写数据(0)or只读数据
                                                 unsigned char FunId, //功能ID
                                                 unsigned short AryId); //多例对像ID号

/*******************************************************************************
                            函数结构
*******************************************************************************/
//实现时，没有此项时定义为NULL,放置顺序需符合ApiID定义
struct _MultitonAPI{
  //通讯与UI用:
  MultitonAPI_GetData_t GetData;         //得到读写数据(一般为配置，有时称为对称性数据)
  MultitonAPI_GetDataOnly_t GetDataOnly; //得到只读性数据
  MultitonAPI_SetData_t SetData;         //设置读写数据(一般为配置，有时称为对称性数据)
  MultitonAPI_ExeCmd_t ExeCmd;           //执行指令
  //UI用:
  MultitonAPI_GetDataMin_t GetDataMin;    //得到读写数据最小值(配置时使用)
  MultitonAPI_GetDataMax_t GetDataMax;    //得到读写数据最大值(配置时使用)
  MultitonAPI_GetHeader_t GetHeader;      //得到标题头(读写数据/只读数据共用)
  MultitonAPI_GetContent_t GetContent;    //直接得到字符串信息(读写数据/只读数据共用)
  MultitonAPI_GetType_t    GetType;       //字符信息(即数据类型)
};

#endif //#define	__MULTITON_API_H

