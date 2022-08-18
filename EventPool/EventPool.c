/*******************************************************************************

                            �¼���ʵ��

*******************************************************************************/

#include "EventPool.h"
#include "MemMng.h"
#include <string.h>

/*******************************************************************************
                          ��غ���-������
********************************************************************************/

//------------------------------�¼��ش���-----------------------------------
//����ʱ���ã�NULL����ʧ��
struct _EventPool *EventPool_pCreate(unsigned char Id,//�����¼���ʱ������Id��
                                      unsigned char  EventSize,//ÿ���¼���С
                                      unsigned short Capability)//��������
{
  void *pv = MemMng_pvMalloc(sizeof(struct _EventPool) + Capability * EventSize);
  if(pv == NULL) return NULL; //������
  struct _EventPool *pEventPool = (struct _EventPool *)pv;
  //��ʼ���ṹ
  pEventPool->Id = Id;
  pEventPool->EventSize = EventSize;  
  pEventPool->Capability = Capability;  
  pEventPool->pData = (unsigned char*)pv + sizeof(struct _EventPool);
  pEventPool->WrPos = 0;
  pEventPool->RdPos = 0;
  return pEventPool;
}

//---------------------------------д�¼�------------------------------------
//�����߲�����ȡ���¼�ʱ����
//����0�ɹ�����0ʧ��
signed char EventPool_Wr(struct _EventPool *pEventPool,
                          const unsigned char *pEvent)
{
  if(pEventPool->WrPos >= pEventPool->Capability) return -1;  //�����������
  unsigned char EventSize = pEventPool->EventSize;  
  
  //�д���ȡ���¼�ʱ��δ��ȡ���ֻ�δ���ߣ��������Բ����������ˡ�
  //����д����¼��п������������յ��ˣ��ʲ�����
  unsigned char RdEndPos = pEventPool->RdPos + 1;
  unsigned char Id = pEventPool->Id;
  if((pEventPool->WrPos > RdEndPos) && (EventPool_cbIsCancel(Id, pEvent))){
    unsigned char *pStart = pEventPool->pData + RdEndPos * EventSize;
    unsigned char *pWr = pStart;//��ʼλ��
    unsigned char *pEnd = pEventPool->pData + pEventPool->WrPos * EventSize; 
    for(; pStart < pEnd; pStart += EventSize){
      if(EventPool_cbIsSame(Id, pEvent, pStart)) continue; //��ͬʱȡ���˼�����
      //���ǵ�ǰ�¼�ʱ��ǰ��
      memcpy(pWr, pStart, EventSize);
      *pWr += EventSize;
    }
    unsigned short CancelCount = (pEnd - pWr) / EventSize;
    if(CancelCount){
      pEventPool->WrPos -= CancelCount;//���Բ�������
      return 0;
    }
  }
  //����һ���¼�
  memcpy(pEventPool->pData + pEventPool->WrPos * EventSize, pEvent, EventSize);
  pEventPool->WrPos++;
  return 0;
}

//---------------------------------��λ--------------------------------------
//����ϵͳ��λʱ������д���
//ע���ⲿҲ��ͬʱ��֪�����������
void EventPool_Reset(struct _EventPool *pEventPool)
{
  pEventPool->WrPos = 0;
  pEventPool->RdPos = 0;
}

/*******************************************************************************
                          ��غ���-������
********************************************************************************/

//-----------------------------��ȡ�¼�---------------------------------
//����NULL��ʾ������
unsigned char *EventPool_pRd(const struct _EventPool *pEventPool)
{
  if(pEventPool->RdPos >= pEventPool->WrPos) return NULL;
  return pEventPool->pData + pEventPool->RdPos * pEventPool->EventSize;
}

//----------------------------��ȡ�¼����-------------------------------
//EventPool_pRd()��ȷ�ϴ�����ɺ���ô˺���
void EventPool_RdFinal(struct _EventPool *pEventPool)
{
  if(pEventPool->RdPos >= pEventPool->WrPos) return; //�Ѷ�����
  unsigned char EventSize = pEventPool->EventSize;  
  unsigned char *pStart = pEventPool->pData;
  //��ǰ�¼�
  const unsigned char *pCurEvent = pStart + pEventPool->RdPos * EventSize;
  pEventPool->RdPos++;  //�����ѷ�����ɣ�ָ����һλ��  
  unsigned char Id = pEventPool->Id;

  //����ȡ���¼�ʱ�޴���
  if(!EventPool_cbIsCancel(Id, pCurEvent)){
    return;
  }
  //Ϊ��ȡ���¼�ʱ,������¼���ص��¼�ȫ��ȡ����(������)
  unsigned char *pWr = pStart;//дλ��  
  const unsigned char *pEnd = pCurEvent + EventSize; //����λ��(����)
  for(; pStart < pEnd; pStart += EventSize){
    if(EventPool_cbIsSame(Id, pCurEvent, pStart)) continue; //��ͬʱȡ���˼�����
    //���ǵ�ǰ�¼�ʱ��ǰ��
    memcpy(pWr, pStart, EventSize);
    pWr += EventSize;
  }
  unsigned short CancelCount = (pEnd - pWr) / EventSize; //��ȡ������
  //δ��֪�����ߵ��ǲ��֣���ǰ��
  if(CancelCount){
    memcpy(pWr, pCurEvent + EventSize, 
           (pEventPool->WrPos - pEventPool->RdPos) * EventSize); 
  }
  pEventPool->RdPos -= CancelCount;//�Ѷ��¼���С��
  pEventPool->WrPos -= CancelCount;//���¼�ͬ��Ҳͬ����С��    
}

//-------------------------------���¶�ȡ---------------------------------
//�������¶���Ϣ
void EventPool_RdReStart(struct _EventPool *pEventPool)
{
  //�����¼��أ�ɾ�����д�ȡ���¼���
  //�Ѷ����ģ��ڵ������ʱ��ȡ���ˣ���֮����Ϊȡ���¼�����Ҫȡ��֮ǰ��
  unsigned short Count = pEventPool->WrPos - pEventPool->RdPos;
  for(unsigned short i = 0; i < Count; i++){ //ģ����Ȼ��
    EventPool_RdFinal(pEventPool);
  }
  pEventPool->RdPos = 0;//��λ�����¶�
}


