/********************************************************************************

                      ��·����ģ��

��ģ������Իָ�����ʹ�ã����䱣����Ͽ����أ���ʹ�Իָ������ָܻ������½���
*******************************************************************************/
#ifndef _SHORT_MNG_H
#define _SHORT_MNG_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif
/******************************************************************************
                        �������
******************************************************************************/
#ifndef SHORT_MNG_WAIT_OV
  #define SHORT_MNG_WAIT_OV   2  //���븺�غ󣬵ȴ�����·��ʱ��,sΪ��λ
#endif
#ifndef SHORT_MNG_NOR_OV
  #define SHORT_MNG_NOR_OV    3  //��鵽��·ʱ����Ϊ��û�лָ���ʱ��,sΪ��λ
#endif
#ifndef SHORT_MNG_PTC_WAIT
  #define SHORT_MNG_PTC_WAIT  20  //�Իָ����ջָ�ʱ��,sΪ��λ
#endif

/******************************************************************************
                        ��ؽṹ
******************************************************************************/

struct _ShortMng{
  unsigned char Flag;  //���״̬��������
  unsigned char Timer;  //��ʱ��
};

//���״̬����Ϊ:
#define SHORT_MNG_STATE_MASK  0x07   //����״̬������Ϊ:
#define SHORT_MNG_STATE_IDIE     0   //�ȴ�(�ֵ��ϵ�)����
#define SHORT_MNG_STATE_WAIT     1   //�ѽ��룬�ȴ��ȶ�
#define SHORT_MNG_STATE_SHUT     2   //�պ�״̬������������
#define SHORT_MNG_STATE_BREAK    3   //�Ͽ�״̬�������������ȴ��ָ�

#define SHORT_MNG_PROTECT_COUNT_SHIFT  4     //��������������λֵ
#define SHORT_MNG_PROTECT_COUNT_MASK  0xF0   //����������������

/*******************************************************************************
                        ��غ���
*******************************************************************************/

//---------------------------------��ʼ��-----------------------------
//void ShortMng_Init(struct _ShortMng *pPow);
#define ShortMng_Init(pow, wait)  do{(pow)->Flag = 0;  (pow)->Timer = wait;}while(0)

//-------------------------------������------------------------------
//����1S����������ɨ��
void ShortMng_Task(struct _ShortMng *pPow, unsigned char Ch); 

//-------------------------------��Ϊ����ģʽ------------------------------
void ShortMng_ShutDown(struct _ShortMng *pPow, unsigned char Ch);//��Ӧͨ��

//-----------------------------�õ���ǰ״̬-----------------------------
#define ShortMng_GetState(pow)  ((pow)->Flag & SHORT_MNG_STATE_MASK)

//-----------------------------�Ƿ��ڽ���״̬-----------------------------
//�ȴ�������ʱ�ڽ���̬,����Ϊ�ǽ���̬
#define ShortMng_IsShut(state) \
  (((state) == SHORT_MNG_STATE_WAIT) || ((state) == SHORT_MNG_STATE_SHUT))

//-----------------------------�Ƿ��ڶ�·״̬-----------------------------
#define ShortMng_IsShort(pow) ((pow)->Flag & SHORT_MNG_PROTECT_COUNT_MASK) 
    
//-----------------------------�õ�������·����-----------------------------
#define ShortMng_GetShortCount(pow) ((pow)->Flag >> SHORT_MNG_PROTECT_COUNT_SHIFT)     
   
/*******************************************************************************
                          �ص�����
*******************************************************************************/
    
//----------------------------��Ӧͨ���Ƿ��·------------------------------
unsigned char ShortMng_cbIsShort(unsigned char Ch);

//----------------------------�պ϶�Ӧͨ��------------------------------
//��������
void ShortMng_cbShut(unsigned char Ch);

//----------------------------�Ͽ���Ӧͨ��------------------------------
//���Ͽ�����
void ShortMng_cbBreak(unsigned char Ch);

//----------------------------״̬�ı�ͨ��------------------------------
void ShortMng_cbStateNotify(unsigned char Ch,
                             unsigned char PrvState,
                             unsigned char CurState);




#endif
