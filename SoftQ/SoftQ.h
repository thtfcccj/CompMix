/* ----------------------------------------------------------------------------
 *                  �������ģ��
 * --------------------------------------------------------------------------*/

#ifndef __SOFT_Q_H 
#define __SOFT_Q_H

/***********************************************************************
//                     �������
***********************************************************************/
#include "MemMng.h"
//�����Ƿ��Զ��ڴ����֧��,֧��ʱʹ��pSoftQCreate,
//��֧���Զ��ڴ�ʱ���ڴ���亯��
#define     SOFTQ_MALLOC(Size)  MemMng_pvMalloc(Size)

//�����Ƿ�����֧��:
//#define     SOFTQ_MUTI_TASK
//��Ϊ�����֧��ʱ�����ü򵥵��ٽ�����������ӣ��ٽ�������
#ifdef SOFTQ_MUTI_TASK
  #define SOFTQ_ENTER_CRITICAL() taskENTER_CRITICAL()
  #define SOFTQ_EXIT_CRITICAL() taskEXIT_CRITICAL()
#else //����Ϊ��
  #define SOFTQ_ENTER_CRITICAL()  do{}while(0)
  #define SOFTQ_EXIT_CRITICAL()  do{}while(0)
#endif

/***********************************************************************
//                    ��ؽṹ,�����ڲ�ʹ��
***********************************************************************/

//������нṹ
struct _SoftQ{
  unsigned char *pData;               //����ָ��
  const unsigned char *pRd;           //���ж�ͷ,ָ��ǰ׼����λ��
  unsigned char *pWr;                  //����дͷ��ָ��׼��д��λ��
  unsigned short Len;                 //���г���
  unsigned char  ItemSize;            //ÿ�����С
  unsigned char Flag;                 //��ر�־
};

//��־����Ϊ��
#define   SOFTQ_FULL        0x80          //����־
#define   SOFTQ_EMPTY       0x40          //�ձ�־

/***********************************************************************
//                    ��ز�������
***********************************************************************/

//-------------------------������д�������---------------------------
//���ض���ָ�룬����ʧ��ʱ����NULL
struct _SoftQ *pSoftQCreate(unsigned short Len,//���г���
                            unsigned short ItemSize);//���С

//-------------------------������г�ʼ������---------------------------
//��֧�ֶ�̬�ڴ����ʱ��ʹ�ô˺���
void SoftQInit(struct _SoftQ *pSoftQ,    //δ��ʼ���Ķ���
                unsigned char *pData,     //Len * ItemSize��С��������
                unsigned short Len,//���г���
                unsigned short ItemSize);//���С

//-----------------------------�պ����ж�------------------------------
#define SoftQIsFull(psoftq) ((psoftq)->Flag & SOFTQ_FULL)
#define SoftQIsEmpty(psoftq) ((psoftq)->Flag & SOFTQ_EMPTY)

//-------------------------���������Ӻ���---------------------------
//���ط�0��ʾ�������������ʧ��
signed char SoftQSend(struct _SoftQ *pSoftQ,//����
                      const void *pvItem);   //���ӵ���

//-------------------------������г��Ӻ���---------------------------
//���ط�0��ʾ����Ϊ�գ�����ʧ��
signed char SoftQReceive(struct _SoftQ *pSoftQ,//����
                         void *pvItem);   //���ӵ���

//-------------------------�鿴׼���������ݺ���---------------------------
//�����ӻ�ó��Ӷ����ݣ�����0��ʾ����Ϊ�գ�����ʧ��
signed char SoftQPeek(struct _SoftQ *pSoftQ,//����
                      void *pvItem);        //��鿴������

//-------------------------�鿴׼�����ӵ��������ݺ���---------------------------
//�����ӻ�ó��Ӷ����ݣ�����0ֵ�����������ʾ����Ϊ�գ�����ʧ��
signed char SoftQPeekAny(struct _SoftQ *pSoftQ,//����
                         unsigned short Pos,  //׼�����ӵ��������λ��
                         void *pvItem);   //��鿴������

#endif

