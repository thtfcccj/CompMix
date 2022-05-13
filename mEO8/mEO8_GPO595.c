/***************************************************************************

        ��չ���8λ()�ӿ�=>��HC595оƬ�ϣ�ͨ��mEO8��������ʵ��

***************************************************************************/
#include "mEO8.h"
#include "Delay.h"
#include "IoCtrl.h"  //IO����
#include <string.h>

//---------------------------IoCtrl��ʵ�ֵĻص�----------------------------
/*/ʱ��IO:
#define mEO8_cbSetClk() do{}while(0)
#define mEO8_cbClrClk() do{}while(0)
//����IO:
#define mEO8_cbSetDio() do{}while(0)
#define mEO8_cbClrDio() do{}while(0)

//�����ź�(������ʱ��Ӧʵ��Ϊ����, ��������ֱ��ʵ��)
#define mEO8_cbSetLe(eo) do{}while(0)
#define mEO8_cbClrLe(eo) do{}while(0)

//��ʱʱ�䣬usΪ��λ
#define mEO8_cbBitDelay()      1 //���ݱ���ʱ��
#define mEO8_cbKeepLeDelay()   1 //�����źű���ʱ��
#define mEO8_cbCancelLeDelay() 0 //�����ź�ȡ��ʱ����ʱ��

//IOͳһ��ʼ�������ڴ˵���
  
//IO׼��,���ڷ���ǰ���� (CLK,DIδ������������ʱ��ʵ��Ϊ��)
#define mEO8_cbIoStart(pEo) do{}while(0)
//IO����,���ڽ���ʱ���� (CLK,DIδ����,�����ж������ʱ��ʵ��Ϊ��)
#define mEO8_cbIoEnd(pEo) do{}while(0) 
*/

/***************************************************************************
                              ��غ���
***************************************************************************/

//------------------------------��ʼ������------------------------------
void mEO8_Init(struct _mEO8 *pEo,
                unsigned char DefaultO) //Ĭ�����
{
  pEo->O = DefaultO;
  pEo->PrvO = DefaultO + 1;  
  mEO8_FaskTask(pEo);//������������һ��
}

//-----------------------------����������-----------------------------
//�����ʱ�佨��<=10ms
void mEO8_FaskTask(struct _mEO8 *pEo)
{
  if(pEo->PrvO == pEo->O) return;
  mEO8_cbIoStart(pEo); //IO׼��(CLK,DIδ������������ʱ��ʵ��Ϊ��,Ҳ�������ٽ���ָʾ)
  //���ʱ��д����:
  unsigned char Data = pEo->O;
  for(unsigned char Shift = 0x80; Shift > 0; Shift >>= 1){
    //׼�������Ƴ�������
    if(Data & Shift) mEO8_cbSetDio();
    else mEO8_cbClrDio();   
    mEO8_cbClrClk();//ʱ����ʼ��    
    DelayUs(mEO8_cbBitDelay());  //���ݱ���
    mEO8_cbSetClk(); //ʱ�ӽ�����
    DelayUs(mEO8_cbBitDelay());  //���ݱ���    
  }
  //��LE�ź���������:
  mEO8_cbSetLe(pEo); //�ߵ�ƽ����
  DelayUs(mEO8_cbKeepLeDelay());  //���汣��
  mEO8_cbClrLe(pEo); //�͵�ƽȡ��  
  DelayUs(mEO8_cbCancelLeDelay());  //ȡ��ʱ����
  mEO8_cbIoEnd(pEo); //IO����(CLK,DIδ����,�����ж������ʱ��ʵ��Ϊ��,Ҳ�������ٽ���ָʾ)
  pEo->PrvO = pEo->O;
}

