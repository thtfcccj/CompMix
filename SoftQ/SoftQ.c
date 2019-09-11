/* ----------------------------------------------------------------------------
 *                  �������ģ��
 * --------------------------------------------------------------------------*/

#include"SoftQ.h"
#include"string.h"

//-------------------------������д�������---------------------------
//���ض���ָ�룬����ʧ��ʱ����NULL
struct _SoftQ *pSoftQCreate(unsigned short Len,//���г���
                            unsigned short ItemSize)//�ڸ����С
{
  //Ϊ���нṹ�Ͷ��ж�̬�����ڴ�
  unsigned int QSize = Len * ItemSize;
  struct _SoftQ *pSoftQ = SOFTQ_MALLOC((sizeof(struct _SoftQ) + QSize));
  if(pSoftQ == NULL) return NULL;  //��ʼ�����ɹ�
  SoftQInit(pSoftQ,(unsigned char *)(pSoftQ + 1),Len,ItemSize);
  return pSoftQ;
}
                                        
//-------------------------������г�ʼ������---------------------------
void SoftQInit(struct _SoftQ *pSoftQ,    //δ��ʼ���Ķ���
                unsigned char *pData,     //Len * ItemSize��С��������
                unsigned short Len,//���г���
                unsigned short ItemSize)//���С
{
  memset(pSoftQ,0,sizeof(struct _SoftQ));
  pSoftQ->Len = Len;
  pSoftQ->ItemSize = ItemSize;  
  unsigned char *pQHeader = (unsigned char *)(pSoftQ + 1);
  pSoftQ->pData = pQHeader;
  pSoftQ->pRd = pQHeader;
  pSoftQ->pWr = pQHeader;
  pSoftQ->Flag = SOFTQ_EMPTY;//����ʱΪ��
}
//-------------------------���������Ӻ���---------------------------
//���ط�0��ʾ�������������ʧ��
signed char SoftQSend(struct _SoftQ *pSoftQ,//����
                      const void *pvItem)   //���ӵ���
{ 
  SOFTQ_ENTER_CRITICAL();
  if(pSoftQ->Flag & SOFTQ_FULL){//����
    SOFTQ_EXIT_CRITICAL();    
    return -1;
  }
  unsigned char ItemSize = pSoftQ->ItemSize;
  unsigned char *pWr = pSoftQ->pWr;
  memcpy(pWr,pvItem,ItemSize);
  pWr += ItemSize;
  if(pWr >= (pSoftQ->pData + ItemSize * pSoftQ->Len))
    pWr = pSoftQ->pData;  //�ػ�����ʼ��
  pSoftQ->pWr = pWr;
  if(pWr == pSoftQ->pRd)  //׷�϶�ͷ�ˣ�����
    pSoftQ->Flag = SOFTQ_FULL;  //дһ���ˣ����в��ٿ�
  else pSoftQ->Flag = 0;        //дһ���ˣ����в��ٿ�
  return 0;
}

//-------------------------������г��Ӻ���---------------------------
//���ط�0��ʾ����Ϊ�գ�����ʧ��
signed char SoftQReceive(struct _SoftQ *pSoftQ,//����
                         void *pvItem)   //��ӵ���
{
  SOFTQ_ENTER_CRITICAL();
  if(pSoftQ->Flag & SOFTQ_EMPTY){//�յģ����ܶ�
    SOFTQ_EXIT_CRITICAL();    
    return -1;
  }
  unsigned char ItemSize = pSoftQ->ItemSize;
  const unsigned char *pRd = pSoftQ->pRd;
  memcpy(pvItem,pRd,ItemSize);
  pRd += ItemSize;
  if(pRd >= (pSoftQ->pData + ItemSize * pSoftQ->Len))
    pRd = pSoftQ->pData;  //�ػ�����ʼ��
  pSoftQ->pRd = pRd;
  if(pRd == pSoftQ->pWr)  //��ͷ׷��дͷ�ˣ��ѿ�
    pSoftQ->Flag = SOFTQ_EMPTY;//��ȡһ���ˣ����в�����
  else pSoftQ->Flag = 0;//��ȡһ���ˣ����в�����
  SOFTQ_EXIT_CRITICAL();
  return 0;
}

//-------------------------�鿴׼���������ݺ���---------------------------
//�����ӻ�ó��Ӷ����ݣ�����0��ʾ����Ϊ�գ�����ʧ��
signed char SoftQPeek(struct _SoftQ *pSoftQ,//����
                      void *pvItem)   //��鿴������
{
  SOFTQ_ENTER_CRITICAL();
  if(pSoftQ->Flag & SOFTQ_EMPTY){//�յģ����ܶ�
    SOFTQ_EXIT_CRITICAL();    
    return -1;
  }
  memcpy(pvItem,pSoftQ->pRd,pSoftQ->ItemSize);
  SOFTQ_EXIT_CRITICAL();
  return 0;
}

//-------------------------�鿴׼�����ӵ��������ݺ���---------------------------
//�����ӻ�ó��Ӷ����ݣ�����0ֵ�����������ʾ����Ϊ�գ�����ʧ��
signed char SoftQPeekAny(struct _SoftQ *pSoftQ,//����
                         unsigned short Pos,  //׼�����ӵ��������λ��
                         void *pvItem)   //��鿴������
{
  SOFTQ_ENTER_CRITICAL();
  if(pSoftQ->Flag & SOFTQ_EMPTY){//�յģ����ܶ�
    SOFTQ_EXIT_CRITICAL();    
    return -1;
  }
  
  const unsigned char *pCurRd = pSoftQ->pRd + Pos * pSoftQ->ItemSize;//��ǰ��λ��
  const unsigned char *pWr = pSoftQ->pWr;//Ԥ��
  if(pSoftQ->pRd < pWr){ //��ǰдû�лػ�ʱ
    if(pCurRd >= pWr) return -1; //������
  }
  else{//дλ�ûػ���
    unsigned short Count = pSoftQ->ItemSize * pSoftQ->Len;
    if(pCurRd < (pSoftQ->pData + Count)){}//��ǰ��δ�ػ�����λ����ȷ
    else{//��ǰ����Ҳ�ػ���,�ص�һ��
      pCurRd -= Count;
      if(pCurRd >= pWr) return -1; //������
    }
  }
  
  memcpy(pvItem, pCurRd, pSoftQ->ItemSize);
  SOFTQ_EXIT_CRITICAL();
  return 0;
}


