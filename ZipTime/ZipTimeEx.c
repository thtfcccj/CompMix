/************************************************************************

                      ѹ��ʱ���ʽģ����չ����ģ��

************************************************************************/
#include "ZipTimeEx.h"
#include "ZipTime.h"

static const unsigned char _MonthToDay[] = {
  0,31,28,31,30,31,30,31,31,30,31,30,31
};

//--------------------------�õ���ȥ��׼���ڵ�����------------------------------
//���������ھ��׼����: 2000.1.1 00:00:00������,ʱ���Ե�
static unsigned short _SubStdDay(unsigned long ZipTime)
{
  unsigned short Day;
  //�ᵽ���������
  unsigned short Year = (unsigned char)(ZipTime >> ZIP_TIME_YEAR_SHIFT);
  Day = Year * 365;
  //������������,���겻��,2000��������
  if(Year) Day  += ((Year - 1) / 4) + 1; 
  //�õ����µ�����,���²���
  unsigned short Mon = (unsigned char)((ZipTime & ((long)0xf << 21)) >> 21);
  if(Mon > 1){
    Mon--;
    if(Mon >= 2){//2�·ݼ������,�����ȼ�1��
      if(!(Year % 4)) Day++;
    }
    //��������
    for( ;Mon > 0; Mon--) Day += _MonthToDay[Mon];
  }
  //�õ����µ�����
  Day += (ZipTime & ZIP_TIME_DATE_MASK) >> ZIP_TIME_DATE_SHIFT;
  return Day;
}

//--------------------------�õ����ϱ�׼���ڵ�����------------------------------
//���������ھ��׼����: �磺2000.1.1 00:00:00������,ʱ���Ե�
static unsigned long _AddStdDay(unsigned short Day)
{
  //�õ����������
  unsigned char Year = 0;
  for(; Year < ZIP_TIME_YEAR_COUNT; Year++){
    unsigned short YearDay;//���������
    if(!(Year % 4)) YearDay = 366;
    else YearDay = 365;
    if(Day >= YearDay) Day -= YearDay;
    else break;
  }
  if(Day > 365) return (unsigned long)Year << ZIP_TIME_YEAR_SHIFT;//���ܳ�����Ӧ��
  //�õ�����,���²���
  unsigned short Mon = 1;
  for(; Mon <= 12; Mon++){
    unsigned short  MonDay = _MonthToDay[Mon];//���µ�����
    if(Mon == 2){//2�·ݼ������
      if(!(Year % 4)) MonDay++; 
    }
    if(Day >= MonDay) Day -= MonDay;
    else break;
  }    
  unsigned long ZipTime = (unsigned long)Year << ZIP_TIME_YEAR_SHIFT;
  ZipTime |= ((unsigned long)Mon) << ZIP_TIME_MOUTH_SHIFT;  
  ZipTime |= ((unsigned long)Day) << ZIP_TIME_DATE_SHIFT; 
  return ZipTime;
}


//------------------------------��ʱ���õ�����--------------------------------
//С������ֱ��ȥ��,Ϊ��ʱ����0
unsigned short ZipTime_GetDaySub(unsigned long Sub,
                                  unsigned long Min)
{
  unsigned short SubDay = _SubStdDay(Sub);
  unsigned short MinDay = _SubStdDay(Min);
  if(SubDay > MinDay) return SubDay - MinDay;
  else return 0;//������
}
//---------------------------------����������-----------------------------------
unsigned long ZipTime_AddDay(unsigned long ZipTime,
                                unsigned short Day)
{
  //������
  Day += _SubStdDay(ZipTime);
  //����һ���е�ʱ��
  ZipTime &= ZIP_TIME_SEC_MASK | ZIP_TIME_MIN_MASK | ZIP_TIME_HOUR_MASK;
  return _AddStdDay(Day) | ZipTime; //����ʱ�䷵��
}

