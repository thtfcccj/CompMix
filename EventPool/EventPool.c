/*******************************************************************************

                            事件池实现

*******************************************************************************/

#include "EventPool.h"
#include "MemMng.h"
#include <string.h>

/*******************************************************************************
                          相关函数-生产者
********************************************************************************/

//------------------------------事件池创建-----------------------------------
//开机时调用，NULL创建失败
struct _EventPool *EventPool_pCreate(unsigned char Id,//多种事件池时，标误Id号
                                      unsigned char  EventSize,//每个事件大小
                                      unsigned short Capability)//所需容量
{
  void *pv = MemMng_pvMalloc(sizeof(struct _EventPool) + Capability * EventSize);
  if(pv == NULL) return NULL; //不够了
  struct _EventPool *pEventPool = (struct _EventPool *)pv;
  //初始化结构
  pEventPool->Id = Id;
  pEventPool->EventSize = EventSize;  
  pEventPool->Capability = Capability;  
  pEventPool->pData = (unsigned char*)pv + sizeof(struct _EventPool);
  pEventPool->WrPos = 0;
  pEventPool->RdPos = 0;
  return pEventPool;
}

//---------------------------------写事件------------------------------------
//生产者产生或取消事件时调用
//返回0成功，非0失败
signed char EventPool_Wr(struct _EventPool *pEventPool,
                          const unsigned char *pEvent)
{
  if(pEventPool->WrPos >= pEventPool->Capability) return -1;  //超设计容量了
  unsigned char EventSize = pEventPool->EventSize;  
  
  //有待读取的事件时，未读取部分还未读走，此区域自产自消就行了。
  if(pEventPool->WrPos > pEventPool->RdPos){
    unsigned char Id = pEventPool->Id;
    unsigned char *pStart = pEventPool->pData + pEventPool->RdPos * EventSize;
    //不是取消正在写入的事件时，直接自生自消了
    if(EventPool_cbIsCancel(Id, pEvent) && 
       !EventPool_cbIsSame(Id, pEvent, pStart)){
      unsigned char *pWr = pStart;//起始位置
      unsigned char *pEnd = pEventPool->pData + pEventPool->WrPos * EventSize;    
      for(; pStart < pEnd; pStart += EventSize){
        if(EventPool_cbIsSame(Id, pEvent, pStart)) continue; //相同时取消了即不动
        //不是当前事件时往前移
        memcpy(pWr, pStart, EventSize);
        *pWr += EventSize;
      }
      unsigned short CancelCount = (pEnd - pWr) / EventSize;
      if(CancelCount){
        pEventPool->WrPos -= CancelCount;//已自产自消了
        return 0;
      }
    }
  }
  //插入一条事件
  memcpy(pEventPool->pData + pEventPool->WrPos * EventSize, pEvent, EventSize);
  pEventPool->WrPos++;
  return 0;
}

//---------------------------------复位--------------------------------------
//用于系统复位时，将读写清空
//注：外部也需同时告知消费者清空了
void EventPool_Reset(struct _EventPool *pEventPool)
{
  pEventPool->WrPos = 0;
  pEventPool->RdPos = 0;
}

/*******************************************************************************
                          相关函数-消费者
********************************************************************************/

//-----------------------------读取事件---------------------------------
//返回NULL表示读完了
unsigned char *EventPool_pRd(const struct _EventPool *pEventPool)
{
  if(pEventPool->RdPos >= pEventPool->WrPos) return NULL;
  return pEventPool->pData + pEventPool->RdPos * pEventPool->EventSize;
}

//----------------------------读取事件完成-------------------------------
//EventPool_pRd()并确认处理完成后调用此函数
void EventPool_RdFinal(struct _EventPool *pEventPool)
{
  if(pEventPool->RdPos >= pEventPool->WrPos) return; //已读完了
  unsigned char EventSize = pEventPool->EventSize;  
  unsigned char *pStart = pEventPool->pData;
  //当前事件
  const unsigned char *pEvent = pStart + pEventPool->RdPos * EventSize;
  unsigned char Id = pEventPool->Id;
  //不是取消事件时无处理！
  if(!EventPool_cbIsCancel(Id, pEvent)){
    pEventPool->RdPos++;  //指向下一位置
    return;
  }
  //为待取消事件时,将与此事件相关的事件全部取消了(含自身)
  unsigned char *pWr = pStart;//写位置  
  const unsigned char *pEnd = pEvent; //自身位置(含)即结束
  for(; pStart <= pEnd; pStart += EventSize){//含自身
    if(EventPool_cbIsSame(Id, pEvent, pStart)) continue; //相同时取消了即不动
    //不是当前事件时往前移
    memcpy(pWr, pStart, EventSize);
    *pWr += EventSize;
  }
  unsigned short CancelCount = (pEnd - pWr) / EventSize; //已取消数量
  //未告知消费者的那部分，往前移
  if(CancelCount){
    memcpy(pWr, pEvent + EventSize, 
           (pEventPool->WrPos - pEventPool->RdPos) * EventSize); 
  }
  pEventPool->RdPos -= CancelCount;//已读事件减小了
  pEventPool->WrPos -= CancelCount;//总事件同步也同步减小了    
}

//-------------------------------重新读取---------------------------------
//用于重新读信息
void EventPool_RdReStart(struct _EventPool *pEventPool)
{
  //清理事件池：删除所有待取消事件：
  //已读过的，在调用完成时都取消了，读之后若为取消事件，则要取消之前的
  unsigned short Count = pEventPool->WrPos - pEventPool->RdPos;
  for(unsigned short i = 0; i < Count; i++){ //模拟自然读
    EventPool_RdFinal(pEventPool);
  }
  pEventPool->RdPos = 0;//复位以重新读
}


