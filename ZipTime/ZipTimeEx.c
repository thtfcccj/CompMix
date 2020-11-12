/************************************************************************

                      压缩时间格式模块扩展功能模块

************************************************************************/
#include "ZipTimeEx.h"
#include "ZipTime.h"

static const unsigned char _MonthToDay[] = {
  0,31,28,31,30,31,30,31,31,30,31,30,31
};

//--------------------------得到减去标准日期的天数------------------------------
//即给定日期距标准日期: 2000.1.1 00:00:00的天数,时间略掉
static unsigned short _SubStdDay(unsigned long ZipTime)
{
  unsigned short Day;
  //提到整年的天数
  unsigned short Year = (unsigned char)(ZipTime >> ZIP_TIME_YEAR_SHIFT);
  Day = Year * 365;
  //加上闰年天数,当年不算,2000年是闰年
  if(Year) Day  += ((Year - 1) / 4) + 1; 
  //得到整月的天数,当月不算
  unsigned short Mon = (unsigned char)((ZipTime & ((long)0xf << 21)) >> 21);
  if(Mon > 1){
    Mon--;
    if(Mon >= 2){//2月份检查闰月,是则先加1天
      if(!(Year % 4)) Day++;
    }
    //整月天数
    for( ;Mon > 0; Mon--) Day += _MonthToDay[Mon];
  }
  //得到当月的天数
  Day += (ZipTime & ZIP_TIME_DATE_MASK) >> ZIP_TIME_DATE_SHIFT;
  return Day;
}

//--------------------------得到加上标准日期的天数------------------------------
//即给定日期距标准日期: 如：2000.1.1 00:00:00的天数,时间略掉
static unsigned long _AddStdDay(unsigned short Day)
{
  //得到整年的天数
  unsigned char Year = 0;
  for(; Year < ZIP_TIME_YEAR_COUNT; Year++){
    unsigned short YearDay;//整年的天数
    if(!(Year % 4)) YearDay = 366;
    else YearDay = 365;
    if(Day >= YearDay) Day -= YearDay;
    else break;
  }
  if(Day > 365) return (unsigned long)Year << ZIP_TIME_YEAR_SHIFT;//不能超过对应年
  //得到整月,当月不算
  unsigned short Mon = 1;
  for(; Mon <= 12; Mon++){
    unsigned short  MonDay = _MonthToDay[Mon];//整月的天数
    if(Mon == 2){//2月份检查闰月
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


//------------------------------由时间差得到天数--------------------------------
//小数天数直接去除,为负时返回0
unsigned short ZipTime_GetDaySub(unsigned long Sub,
                                  unsigned long Min)
{
  unsigned short SubDay = _SubStdDay(Sub);
  unsigned short MinDay = _SubStdDay(Min);
  if(SubDay > MinDay) return SubDay - MinDay;
  else return 0;//不够减
}
//---------------------------------加天数函数-----------------------------------
unsigned long ZipTime_AddDay(unsigned long ZipTime,
                                unsigned short Day)
{
  //计算数
  Day += _SubStdDay(ZipTime);
  //保留一天中的时间
  ZipTime &= ZIP_TIME_SEC_MASK | ZIP_TIME_MIN_MASK | ZIP_TIME_HOUR_MASK;
  return _AddStdDay(Day) | ZipTime; //加上时间返回
}

