/************************************************************************

                      ѹ��ʱ���ʽģ����չ����ģ��

************************************************************************/
#ifndef _ZIP_TIME_EX_H 
#define _ZIP_TIME_EX_H

//---------------------��ʱ���õ�����-----------------------
//С������ֱ��ȥ��,Ϊ��ʱ����0
unsigned short ZipTime_GetDaySub(unsigned long Sub,
                                  unsigned long Min);

//-----------------------����������--------------------------------
unsigned long ZipTime_AddDay(unsigned long ZipTime,
                               unsigned short Day);

//-----------------------��С�жϺ���------------------------------
//��ZipTime0 > ZipTime1ʱ������
//unsigned char ZipTime_CmpMore(unsigned long ZipTime0,
//                              unsigned long ZipTime1);
#define ZipTime_CmpMore(ziptime0,ziptime1) (ziptime0 > ziptime1)

//---------------------------�õ�һ���е���----------------------
//�磺2021.11.20 23:55:40����23:55:40������󷵻�
unsigned long ZipTime_GetDaySec(unsigned long ZipTime);

//---------------�õ�ǰ��ʱ����������--------------------------
//��ǰʱ���� >= �ϴ�ʱ��,���򷵻�0
unsigned long ZipTime_GetSecSub(unsigned long PrvTime, //�ϴ�ʱ��
                               unsigned long CurTime);//��ǰʱ��

#endif

