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

#endif

