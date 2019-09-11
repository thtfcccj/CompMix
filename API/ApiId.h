/***********************************************************************

                  单例/多例模式对像/结构统一访问接口ID号定义
此ID可用于通讯时区分不同访问接口
***********************************************************************/
#ifndef __API_ID_H
#define	__API_ID_H


//---------------------------ApiID定义----------------------------------
//控制在8个以在3Bit内完成

//通讯与UI用(0-3,奇读偶写):
#define API_ID_GET_DATA          0  //得到读写数据(功能ID=读写数据ID)
#define API_ID_GET_DATA_ONLY     1  //得到只读性数据(功能ID=只读数据ID)
#define API_ID_SET_DATA          2  //设置读写数据(功能ID=读写数据ID)
#define API_ID_EXE_CMD           3  //执行本地指令(功能ID=命令功能ID)

//UI用(全部为读)
#define API_ID_GET_DATA_MIN      4  //得到读写数据最小值(功能ID=读写数据ID)
#define API_ID_GET_DATA_MAX      5  //得到读写数据最大值(功能ID=读写数据ID)
#define API_ID_GET_HEADER        6  //得到标题头(①功能ID=读写数据ID或只读数据ID)
#define API_ID_GET_CONTENT       7  //直接得到字符串信息(①功能ID=读写数据ID或只读数据ID)

#define API_ID_COUNT             8 //总数

//注①： 
//    关于得到标题头与直接得到字符串信息时，区分读写数据ID或只读数据ID问题,与应用相关，
//需通过其它途径判断(具体为NetFunMsg_IsDataOnly())

//------------------------------ApiID分类定义---------------------------------
#define API_ID_IS_UI_ONLY        0x04 //4~7定义为UI需要的内容,全部为读
//为UI与通讯共用时0-3定义:
#define API_ID_IS_NO_RW_DATA     0x01 //1,3为非读写数据(即0,2可读写)
#define API_ID_IS_WR             0x02 //0,1为读，2,4为写

//API是否为回写数据:只有两个码是
#define ApiId_IsWr(apiId)  ((apiId == API_ID_SET_DATA) || (apiId == API_ID_EXE_CMD))

//---------------------------Api名柄定义----------------------------------
//此为系统全局定义：此体大小系统相关
//多例模式时: 用于获取对像阵列相关指针和功能ID号

#ifndef ApiHandle_t
  #define ApiHandle_t  unsigned char
#endif

#endif //#define	__API_ID_H

