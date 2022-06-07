/************************************************************************

                      ѹ��ʱ���ʽģ����չ����ģ��

************************************************************************/
#include "ZipTimeEx.h"
#include "ZipTime.h"

static const unsigned char _MonthToDay[] = {
  0,31,28,31,30,31,30,31,31,30,31,30,31
};

//--------------------------�õ���ȥ��׼���ڵ�����------------------------------
//���������ھ��׼����: YEAR_START.1.1 00:00:00������,ʱ���Ե�
static unsigned short _SubStdDay(unsigned long ZipTime)
{
  unsigned short Day;
  //�ᵽ���������
  unsigned short Year = (unsigned char)(ZipTime >> ZIP_TIME_YEAR_SHIFT);
  Day = Year * 365;
  //������������(��׼ȷ�㷨)
  Day += (Year + (YEAR_START % 4)) / 4;
  //�õ����µ�����,���²���
  unsigned short Mon = (unsigned char)((ZipTime & ZIP_TIME_MOUTH_MASK) >> ZIP_TIME_MOUTH_SHIFT);
  if(Mon > 1){
    Mon--;
    if(Mon >= 2){//2�·ݼ������,�����ȼ�1��
      if(((Year + (YEAR_START % 4)) % 4) == 0) Day++;
    }
    //��������
    for( ;Mon > 0; Mon--) Day += _MonthToDay[Mon];
  }
  //�õ����µ�����
  Day += (ZipTime & ZIP_TIME_DATE_MASK) >> ZIP_TIME_DATE_SHIFT;
  return Day;
}

//--------------------------�õ����ϱ�׼���ڵ�����------------------------------
//���������ھ��׼����: YEAR_START.1.1 00:00:00������,ʱ���Ե�
static unsigned long _AddStdDay(unsigned short Day)
{
  //�õ����������
  volatile unsigned char Year = 0;
  for(; Year < ZIP_TIME_YEAR_COUNT; Year++){
    unsigned short YearDay;//���������
    if(((Year + (YEAR_START % 4)) % 4) == 0) YearDay = 366;
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

//---------------------------�õ�һ���е���----------------------
//�磺2021.11.20 23:55:40����23:55:40������󷵻�
unsigned long ZipTime_GetDaySec(unsigned long ZipTime)
{
  //Сʱ����
  unsigned long Sec = (ZipTime & ZIP_TIME_HOUR_MASK) >> ZIP_TIME_HOUR_SHIFT;
  Sec *= 3600;
  //��������
  Sec += ((ZipTime & ZIP_TIME_MIN_MASK) >> ZIP_TIME_MIN_SHIFT) * 60;
  return Sec + (ZipTime & ZIP_TIME_SEC_MASK);//������
}

//---------------�õ�ǰ��ʱ����������--------------------------
//��ǰʱ���� >= �ϴ�ʱ��,���򷵻�0
unsigned long ZipTime_GetSecSub(unsigned long PrvTime, //�ϴ�ʱ��
                               unsigned long CurTime)//��ǰʱ��
{
  unsigned long Sec;
  //һ��֮��
  if((PrvTime >> ZIP_TIME_DATT_SHIFT) == (CurTime >> ZIP_TIME_DATT_SHIFT))
    Sec = 0;
  else{//������
    Sec = ZipTime_GetDaySub(CurTime, PrvTime);
    if(Sec >= (0xffffffff / (24 * 3600)) - 1) return 0xffffffff; //�쳣������
    Sec *= 24 * 3600; //һ�������
  }
  //������
  PrvTime = ZipTime_GetDaySec(PrvTime);
  CurTime = ZipTime_GetDaySec(CurTime);
  //ʱ����ػ��ˣ��磺2021.11.21 13:43:20  - 2021.11.20 23:55:40
  if(PrvTime > CurTime)//�����������1�죬��ʵ�ʶ����,Ҫ���ż�
    Sec -= PrvTime - CurTime;
  else Sec += CurTime - PrvTime;
  return Sec;
}


