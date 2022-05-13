/***************************************************************************

        ��չ���8λ()�ӿ�=>��HC595оƬ�ϣ�ͨ��EO8��������ʵ��

***************************************************************************/
#include "EO8.h"
#include "Delay.h"
#include "IoCtrl.h"  //IO����
#include <string.h>

struct _EO8  EO8;//ֱ�ӵ�����

//---------------------------IoCtrl��ʵ�ֵĻص�----------------------------
/*/ʱ��IO:
#define EO8_cbSetClk() do{}while(0)
#define EO8_cbClrClk() do{}while(0)
//����IO:
#define EO8_cbSetDio() do{}while(0)
#define EO8_cbClrDio() do{}while(0)
//�����ź�:
#define EO8_cbSetLe() do{}while(0)
#define EO8_cbClrLe() do{}while(0)
//��ʱʱ�䣬usΪ��λ
#define SEO8_cbBitDelay()      1 //���ݱ���ʱ��
#define SEO8_cbKeepLeDelay()   1 //�����źű���ʱ��
#define SEO8_cbCancelLeDelay() 0 //�����ź�ȡ��ʱ����ʱ��

//IO��ʼ��
#define EO8_cbIoInit()  do{}while(0)
//IO׼��,���ڷ���ǰ���� (CLK,DIδ������������ʱ��ʵ��Ϊ��)
#define EO8_cbIoStart() do{}while(0)
//IO����,���ڽ���ʱ���� (CLK,DIδ����,�����ж������ʱ��ʵ��Ϊ��)
#define EO8_cbIoEnd() do{}while(0) 

*/

/***************************************************************************
                              ��غ���
***************************************************************************/

//------------------------------��ʼ������------------------------------
void EO8_Init(void)
{
  EO8_cbIoInit();//IO��ʼ��
  EO8.O = EO8_DEFAULT;
  //EO8��������Ϊ�Ƕ�ռʽ�����ڴ˳�ʼ��
  EO8_FaskTask();//������������һ��
}

//-----------------------------����������-----------------------------
//�����ʱ�佨��<=10ms
void EO8_FaskTask(void)
{
  if(EO8.PrvO == EO8.O) return;
  EO8_cbIoStart(); //IO׼��(CLK,DIδ������������ʱ��ʵ��Ϊ��)
  //���ʱ��д����:
  unsigned char Data = EO8.O;
  for(unsigned char Shift = 0x80; Shift > 0; Shift >>= 1){
    //׼�������Ƴ�������
    if(Data & Shift) EO8_cbSetDio();
    else EO8_cbClrDio();   
    EO8_cbClrClk();//ʱ����ʼ��    
    DelayUs(SEO8_cbBitDelay());  //���ݱ���
    EO8_cbSetClk(); //ʱ�ӽ�����
    DelayUs(SEO8_cbBitDelay());  //���ݱ���    
  }
  //��LE�ź���������:
  EO8_cbSetLe(); //�ߵ�ƽ����
  DelayUs(SEO8_cbKeepLeDelay());  //���汣��
  EO8_cbClrLe(); //�͵�ƽȡ��  
  DelayUs(SEO8_cbCancelLeDelay());  //ȡ��ʱ����
  EO8_cbIoEnd(); //IO����(CLK,DIδ����,�����ж������ʱ��ʵ��Ϊ��)
  EO8.PrvO = EO8.O;
}

