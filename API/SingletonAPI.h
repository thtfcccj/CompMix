/***********************************************************************

                  单例模式对像/结构统一访问接口定义

***********************************************************************/
#ifndef __SINGLETON_API_H
#define	__SINGLETON_API_H

#include "ApiId.h" //ApiHandle_t

/*******************************************************************************
                            接口函数原型定义
*******************************************************************************/

//-----------------得到读写数据(一般为配置，有时称为对称性数据)-------------------
typedef void (*SingletonAPI_GetData_t)(unsigned char *pData,//接收据缓冲区
                                       unsigned char FunId); //功能ID

//-----------------设置读写数据(一般为配置，有时称为对称性数据)--------------
//返回写结果
typedef unsigned char (*SingletonAPI_SetData_t)(const unsigned char *pData,//对应数据
                                        unsigned char FunId); //功能ID

//----------------------------得到只读性数据-----------------------------------
typedef void (*SingletonAPI_GetDataOnly_t)(unsigned char *pData,//接收据缓冲区
                                           unsigned char FunId); //功能ID                               

//----------------------------执行本地指令-----------------------------------
//返回执行结果(低3Bit)+已使用参数长度(高5Bit)
typedef unsigned char (*SingletonAPI_ExeCmd_t)(unsigned char FunId, //功能ID 
                                                 const unsigned char *pPara);//参数，无时为NULL


//-------------------------得到数据最小值-------------------------------
typedef void (*SingletonAPI_GetDataMin_t)(unsigned char *pData,//接收据缓冲区
                                          unsigned char FunId); //功能ID

//-------------------------得到数据最大值-------------------------------
typedef void (*SingletonAPI_GetDataMax_t)(unsigned char *pData, //接收据缓冲区
                                          unsigned char FunId); //功能ID

//------------------------------得到标题头----------------------------------
//返回标题头字符串
typedef const char *(*SingletonAPI_GetHeader_t)(unsigned char IsOnly,  //读写数据(0)or只读数据
                                                 unsigned char FunId); //功能ID 
                       
//--------------------------------得到内容数据----------------------------------
//返回字符串
typedef const char *(*SingletonAPI_GetContent_t)(char *pBuf,  //可能用到的缓冲区
                                  unsigned char IsOnly,  //读写数据(0)or只读数据
                                  unsigned char FunId); //功能ID

//--------------------------------得到数据信息----------------------------------
//返回ApiIdType标识
typedef unsigned char (*SingletonAPI_GetInfo_t)(unsigned char IsOnly,  //读写数据(0)or只读数据
                                                  unsigned char FunId); //功能ID

/*******************************************************************************
                            函数结构
*******************************************************************************/
//实现时，没有此项时定义为NULL,放置顺序需符合ApiID定义
struct _SingletonAPI{
  //通讯与UI用:
  SingletonAPI_GetData_t GetData;         //得到读写数据(一般为配置，有时称为对称性数据)
  SingletonAPI_GetDataOnly_t GetDataOnly; //得到只读性数据
  SingletonAPI_SetData_t SetData;         //设置读写数据(一般为配置，有时称为对称性数据)
  SingletonAPI_ExeCmd_t ExeCmd;           //执行本地指令(只写,UI/层显等控制本机时需实现)
  //UI用:
  SingletonAPI_GetDataMin_t GetDataMin;    //得到读写数据最小值(配置时使用)
  SingletonAPI_GetDataMax_t GetDataMax;    //得到读写数据最大值(配置时使用)
  SingletonAPI_GetHeader_t GetHeader;      //得到标题头(读写数据/只读数据共用)
  SingletonAPI_GetContent_t GetContent;    //直接得到字符串信息(读写数据/只读数据共用)
  SingletonAPI_GetInfo_t    GetInfo;      //数据信息
};

#endif //#define	__SINGLETON_API_H

