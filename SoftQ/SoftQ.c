/* ----------------------------------------------------------------------------
 *                  软件队列模块
 * --------------------------------------------------------------------------*/

#include"SoftQ.h"
#include"string.h"

//-------------------------软件队列创建函数---------------------------
//返回队列指针，创建失败时返回NULL
struct _SoftQ *pSoftQCreate(unsigned short Len,//队列长度
                            unsigned short ItemSize)//第个项大小
{
  //为队列结构和队列动态分配内存
  unsigned int QSize = Len * ItemSize;
  struct _SoftQ *pSoftQ = SOFTQ_MALLOC((sizeof(struct _SoftQ) + QSize));
  if(pSoftQ == NULL) return NULL;  //初始化不成功
  SoftQInit(pSoftQ,(unsigned char *)(pSoftQ + 1),Len,ItemSize);
  return pSoftQ;
}
                                        
//-------------------------软件队列初始化函数---------------------------
void SoftQInit(struct _SoftQ *pSoftQ,    //未初始化的队列
                unsigned char *pData,     //Len * ItemSize大小的数据区
                unsigned short Len,//队列长度
                unsigned short ItemSize)//项大小
{
  memset(pSoftQ,0,sizeof(struct _SoftQ));
  pSoftQ->Len = Len;
  pSoftQ->ItemSize = ItemSize;  
  unsigned char *pQHeader = (unsigned char *)(pSoftQ + 1);
  pSoftQ->pData = pQHeader;
  pSoftQ->pRd = pQHeader;
  pSoftQ->pWr = pQHeader;
  pSoftQ->Flag = SOFTQ_EMPTY;//创建时为空
}
//-------------------------软件队列入队函数---------------------------
//返回非0表示队列已满，入队失败
signed char SoftQSend(struct _SoftQ *pSoftQ,//队列
                      const void *pvItem)   //出队的项
{ 
  SOFTQ_ENTER_CRITICAL();
  if(pSoftQ->Flag & SOFTQ_FULL){//满了
    SOFTQ_EXIT_CRITICAL();    
    return -1;
  }
  unsigned char ItemSize = pSoftQ->ItemSize;
  unsigned char *pWr = pSoftQ->pWr;
  memcpy(pWr,pvItem,ItemSize);
  pWr += ItemSize;
  if(pWr >= (pSoftQ->pData + ItemSize * pSoftQ->Len))
    pWr = pSoftQ->pData;  //回还到开始了
  pSoftQ->pWr = pWr;
  if(pWr == pSoftQ->pRd)  //追上读头了，已满
    pSoftQ->Flag = SOFTQ_FULL;  //写一个了，队列不再空
  else pSoftQ->Flag = 0;        //写一个了，队列不再空
  return 0;
}

//-------------------------软件队列出队函数---------------------------
//返回非0表示队列为空，出队失败
signed char SoftQReceive(struct _SoftQ *pSoftQ,//队列
                         void *pvItem)   //入队的项
{
  SOFTQ_ENTER_CRITICAL();
  if(pSoftQ->Flag & SOFTQ_EMPTY){//空的，不能读
    SOFTQ_EXIT_CRITICAL();    
    return -1;
  }
  unsigned char ItemSize = pSoftQ->ItemSize;
  const unsigned char *pRd = pSoftQ->pRd;
  memcpy(pvItem,pRd,ItemSize);
  pRd += ItemSize;
  if(pRd >= (pSoftQ->pData + ItemSize * pSoftQ->Len))
    pRd = pSoftQ->pData;  //回还到开始了
  pSoftQ->pRd = pRd;
  if(pRd == pSoftQ->pWr)  //读头追上写头了，已空
    pSoftQ->Flag = SOFTQ_EMPTY;//读取一个了，队列不再满
  else pSoftQ->Flag = 0;//读取一个了，队列不再满
  SOFTQ_EXIT_CRITICAL();
  return 0;
}

//-------------------------查看准备出队数据函数---------------------------
//不出队获得出队端数据，返回0表示队列为空，出队失败
signed char SoftQPeek(struct _SoftQ *pSoftQ,//队列
                      void *pvItem)   //需查看的数据
{
  SOFTQ_ENTER_CRITICAL();
  if(pSoftQ->Flag & SOFTQ_EMPTY){//空的，不能读
    SOFTQ_EXIT_CRITICAL();    
    return -1;
  }
  memcpy(pvItem,pSoftQ->pRd,pSoftQ->ItemSize);
  SOFTQ_EXIT_CRITICAL();
  return 0;
}

//-------------------------查看准备出队的任意数据函数---------------------------
//不出队获得出队端数据，返回0值正常，否则表示队列为空，出队失败
signed char SoftQPeekAny(struct _SoftQ *pSoftQ,//队列
                         unsigned short Pos,  //准备出队的最近数据位置
                         void *pvItem)   //需查看的数据
{
  SOFTQ_ENTER_CRITICAL();
  if(pSoftQ->Flag & SOFTQ_EMPTY){//空的，不能读
    SOFTQ_EXIT_CRITICAL();    
    return -1;
  }
  
  const unsigned char *pCurRd = pSoftQ->pRd + Pos * pSoftQ->ItemSize;//当前读位置
  const unsigned char *pWr = pSoftQ->pWr;//预读
  if(pSoftQ->pRd < pWr){ //当前写没有回环时
    if(pCurRd >= pWr) return -1; //读多了
  }
  else{//写位置回环了
    unsigned short Count = pSoftQ->ItemSize * pSoftQ->Len;
    if(pCurRd < (pSoftQ->pData + Count)){}//当前读未回环，读位置正确
    else{//当前读多也回环了,回到一起
      pCurRd -= Count;
      if(pCurRd >= pWr) return -1; //读多了
    }
  }
  
  memcpy(pvItem, pCurRd, pSoftQ->ItemSize);
  SOFTQ_EXIT_CRITICAL();
  return 0;
}


