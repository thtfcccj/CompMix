/***********************************************************************

                  索引型资源通用操作函数集

此模块配合PC端软件使用：
  CCJ多功能文件处理与编译器.exe
  目标可执行文件下载地址：https://download.csdn.net/download/thtfcccj/81188414
  源代码开源地址：https://github.com/thtfcccj/FileMergeIndex.git

此模块用于该资源或文件固化在程序FLASH中时，得到该资源大小或文件等信息
为简化相关操作，本资源前缀以RI(Resource Index)打头
***********************************************************************/
#ifndef __RESOURCE_INDEX_H 
#define __RESOURCE_INDEX_H 
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/***********************************************************************
                        索引型资源格式说明及示例
***********************************************************************/
//索引型资源是指资源数据(如字符串，图片等)不等长时，在其存储空间头部
//加入其编移位置组成的资源或文件,格式如(此示例用MSB两位构成)：
//示例：
//数据起      第二数据起  数据结束位    第一资源数据区        第二资源数据区
// 0    1      2    3      4    5      6     7    8    9      10   11   <-位置
//0x00 0x06   0x00 0x0a   0x00 0x0c   0x12 0x34 0x56  0x78   0x12 0x34  <-数据
//含义为： 0x0006： 索引头占6个字节，同时第6个偏移起为首个数据起始
//         0x000a:  第2个数据从第10个偏移起
//         0x000c:  最后一组数据描述此索引文件大小，即结束位置
//数据大小 = 下一位置偏移 - 当前位置偏移，如第1个资源数据大小=0x000a-0x0006=4

/***********************************************************************
                        相关配置
***********************************************************************/

//定义索引头对齐方式，默认为LSB对齐
//#define SUPPORT_RI_MSB  

//索引头长度，默认双字节(暂仅支持单字节或双字节)
#ifndef RI_HEADER_LEN
  #define RI_HEADER_LEN  2
#endif

/***********************************************************************
                        相关接口
***********************************************************************/

//---------------------------异常时返回空资源定义------------------------
extern const char StrSpace[];  //空格字符" "
#define RI_NULL  ((const unsigned char *)StrSpace)

//----------------------由资源ID得到数据相对位置-------------------------
unsigned short RI_GetOffPos(const unsigned char *pBase,//资源基址
                             unsigned short ResId);

//------------------------------由资源ID得到数据--------------------------
//未找到时返回RI_NULL
const unsigned char *RI_pGetData(const unsigned char *pBase,//资源基址
                                  unsigned char ResId);

//------------------------------由资源ID得到数据--------------------------
//形参为unsigned long型时
//const unsigned char *RI_pGetDataL(unsigned long Base,//资源基址
//                                   unsigned char ResId);
//直接宏实现：
#define RI_pGetDataL(base,resId) \
                     RI_pGetData((const unsigned char *)(base),resId)

//----------------------------由资源ID得到数据大小-------------------------
//未找到时返回0
unsigned short RI_GetSize(const unsigned char *pBase,//资源基址
                           unsigned short ResId);

#endif  //#ifndef __RESOURCE_INDEX_H 



