/* ----------------------------------------------------------------------------
 *                  软件队列模块
 * --------------------------------------------------------------------------*/

#ifndef __SOFT_Q_H 
#define __SOFT_Q_H

/***********************************************************************
//                     相关配置
***********************************************************************/
#include "MemMng.h"
//定义是否自动内存分配支持,支持时使用pSoftQCreate,
//若支持自动内存时，内存分配函数
#define     SOFTQ_MALLOC(Size)  MemMng_pvMalloc(Size)

//定义是否多进程支持:
//#define     SOFTQ_MUTI_TASK
//当为多进程支持时，采用简单的临界区操作出入队：临界区定义
#ifdef SOFTQ_MUTI_TASK
  #define SOFTQ_ENTER_CRITICAL() taskENTER_CRITICAL()
  #define SOFTQ_EXIT_CRITICAL() taskEXIT_CRITICAL()
#else //定义为空
  #define SOFTQ_ENTER_CRITICAL()  do{}while(0)
  #define SOFTQ_EXIT_CRITICAL()  do{}while(0)
#endif

/***********************************************************************
//                    相关结构,仅供内部使用
***********************************************************************/

//软件队列结构
struct _SoftQ{
  unsigned char *pData;               //数据指针
  const unsigned char *pRd;           //队列读头,指向当前准备读位置
  unsigned char *pWr;                  //队列写头，指向准备写入位置
  unsigned short Len;                 //队列长度
  unsigned char  ItemSize;            //每个项大小
  unsigned char Flag;                 //相关标志
};

//标志定义为：
#define   SOFTQ_FULL        0x80          //满标志
#define   SOFTQ_EMPTY       0x40          //空标志

/***********************************************************************
//                    相关操作函数
***********************************************************************/

//-------------------------软件队列创建函数---------------------------
//返回队列指针，创建失败时返回NULL
struct _SoftQ *pSoftQCreate(unsigned short Len,//队列长度
                            unsigned short ItemSize);//项大小

//-------------------------软件队列初始化函数---------------------------
//不支持动态内存分配时可使用此函数
void SoftQInit(struct _SoftQ *pSoftQ,    //未初始化的队列
                unsigned char *pData,     //Len * ItemSize大小的数据区
                unsigned short Len,//队列长度
                unsigned short ItemSize);//项大小

//-----------------------------空和满判断------------------------------
#define SoftQIsFull(psoftq) ((psoftq)->Flag & SOFTQ_FULL)
#define SoftQIsEmpty(psoftq) ((psoftq)->Flag & SOFTQ_EMPTY)

//-------------------------软件队列入队函数---------------------------
//返回非0表示队列已满，入队失败
signed char SoftQSend(struct _SoftQ *pSoftQ,//队列
                      const void *pvItem);   //出队的项

//-------------------------软件队列出队函数---------------------------
//返回非0表示队列为空，出队失败
signed char SoftQReceive(struct _SoftQ *pSoftQ,//队列
                         void *pvItem);   //出队的项

//-------------------------查看准备出队数据函数---------------------------
//不出队获得出队端数据，返回0表示队列为空，出队失败
signed char SoftQPeek(struct _SoftQ *pSoftQ,//队列
                      void *pvItem);        //需查看的数据

//-------------------------查看准备出队的任意数据函数---------------------------
//不出队获得出队端数据，返回0值正常，否则表示队列为空，出队失败
signed char SoftQPeekAny(struct _SoftQ *pSoftQ,//队列
                         unsigned short Pos,  //准备出队的最近数据位置
                         void *pvItem);   //需查看的数据

#endif

