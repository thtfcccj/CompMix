/************************************************************************

                          压缩时间格式模块实现
************************************************************************/

#include "ZipTime.h"

unsigned long RtcZipTime = 0; //当前实时时间

/************************************************************************
                           相关函数实现
************************************************************************/

//-----------------------由普通时间得到压缩后的时间----------------------
//此函数未检查输入数据的正确性
unsigned long ZipTime_FromTime(unsigned char RelativeYear, //相对起始年,0-63
                                unsigned char Mouth,
                                unsigned char Date,
                                unsigned char Hour,
                                unsigned char Minute,
                                unsigned char Sec)
{
  unsigned long ZipTime = ((unsigned long)Sec) << ZIP_TIME_SEC_SHIFT;
  ZipTime |= ((unsigned long)Minute) << ZIP_TIME_MIN_SHIFT;
  ZipTime |= ((unsigned long)Hour) << ZIP_TIME_HOUR_SHIFT; 
  ZipTime |= ((unsigned long)Date) << ZIP_TIME_DATE_SHIFT;
  ZipTime |= ((unsigned long)Mouth) << ZIP_TIME_MOUTH_SHIFT;   
  ZipTime |= ((unsigned long)RelativeYear) << ZIP_TIME_YEAR_SHIFT;
  return ZipTime;
}

//--------------------------由压缩时间得到需要的元素-------------------------
//Pos定义为返回信息:相对年0,月1,日2,时3,分4,秒5
unsigned char ZipTime_GetInfo(unsigned char Pos, unsigned long ZipTime)
{
  switch(Pos){
  case 0: return (ZipTime & ZIP_TIME_YEAR_MASK) >> ZIP_TIME_YEAR_SHIFT;
  case 1: return (ZipTime & ZIP_TIME_MOUTH_MASK) >> ZIP_TIME_MOUTH_SHIFT;  
  case 2: return (ZipTime & ZIP_TIME_DATE_MASK) >> ZIP_TIME_DATE_SHIFT;
  case 3: return (ZipTime & ZIP_TIME_HOUR_MASK) >> ZIP_TIME_HOUR_SHIFT;    
  case 4: return (ZipTime & ZIP_TIME_MIN_MASK) >> ZIP_TIME_MIN_SHIFT;
  case 5: return (ZipTime & ZIP_TIME_SEC_MASK) >> ZIP_TIME_SEC_SHIFT;
  }
  return 0; //异常返回
}

//-----------------------------得到日期-----------------------------
//日0-4b,月5-8b,年9-15b以2000年为基准
unsigned short ZipTime_GetDate(unsigned long ZipTime)
{
  unsigned short Date = ZipTime >> ZIP_TIME_DATE_SHIFT;
  return Date + ((unsigned short)(YEAR_START - 2000) << 9);
}  

//-------------------由ZipTime_GetDate()日期得压缩时间-----------------
//将以2000年为基准的年,转换为本地年基准,时间填充0
unsigned long ZipTime_Date2Zip(unsigned short Date)
{
  unsigned long ZipTime = 
    (unsigned long)((Date >> 9) - (YEAR_START - 2000)) << ZIP_TIME_YEAR_SHIFT;
  ZipTime |= (unsigned long)(Date & 0x1ff) << ZIP_TIME_DATE_SHIFT;
  return ZipTime;
}

//--------------------------压缩时间格式转为数据流-------------------------
//数据流固定年在最前，秒在最后
void ZipTime_ToData(unsigned long ZipTime, unsigned char *pBuf)
{
  //为保证对齐，没有用memcpy;
  *pBuf++ = (unsigned char)(ZipTime >> 24);
  *pBuf++ = (unsigned char)(ZipTime >> 16);
  *pBuf++ = (unsigned char)(ZipTime >> 8);
  *pBuf = (unsigned char)(ZipTime);
}

//--------------------------数据流恢复为压缩时间格式-------------------------
//数据流固定年在最前，秒在最后
unsigned long ZipTime_FromData(const unsigned char *pBuf)
{
  //为保证对齐，没有用memcpy;
  unsigned long ZipTime;
  ZipTime = (unsigned long )(*pBuf++) << 24;
  ZipTime |= (unsigned long )(*pBuf++) << 16;
  ZipTime |= (unsigned long )(*pBuf++) << 8;
  ZipTime |= (unsigned long )(*pBuf);  
  return ZipTime;
}

//--------------------------数据流时间是否正确-------------------------
//返回非0正确，否则返回0
signed char ZipTime_IsCheckValid(const unsigned char *pBuf)
{
  //快速校验
  if(*pBuf++ != 0) return 1;
  if(*pBuf++ != 0) return 1;
  if(*pBuf++ != 0) return 1;
  if(*pBuf != 0) return 1;
  return 0;
}
     
//--------------------------校验压缩时间是否正确-------------------------
//返回0正确，否则返回检没到的起始错误位置：月-1,日-2,时-3,分-4,秒-5
signed char ZipTime_Check(unsigned long ZipTime)
{
  //快速校验返回：
  if(ZipTime == 0xffffffff) return -1;//月为16,超限
  if(ZipTime == 0) return -1;//月为0,超限
  //详细检查:
  unsigned char Info = ZipTime_GetInfo(1, ZipTime); //月
  if((Info < 1) || (Info > 12)) return -1;//超限
  Info = ZipTime_GetInfo(2, ZipTime); //日
  if((Info < 1) || (Info > 31)) return -2;//超限  
  Info = ZipTime_GetInfo(3, ZipTime); //时
  if(Info >= 24) return -3;//超限   
  Info = ZipTime_GetInfo(4, ZipTime); //分
  if(Info >= 60) return -4;//超限    
  Info = ZipTime_GetInfo(5, ZipTime); //秒
  if(Info >= 60) return -5;//超限
      
  return 0;  //检查通过！
}

//---------------------------压缩时间转换为中文格式字符------------------------
//格式为XXXX-XX-XX XX:XX:XX的掩码,即年-月-日 时:分:秒
//标志按位定义为:0bit:显示年前两位, 1bit:显示年后两位,2bit:打印月
//3bit:打印日, 4bit:打印时, 5bit,打印分, 6bit:允许显示秒
void ZipTime_ToStringCh(char *pString, 
                        unsigned long ZipTime,
                        unsigned char Flag)
{
  if(Flag & 0x03){//有年份定义时
    unsigned short Year = ZipTime_GetInfo(0, ZipTime) + ZipTime_cbGetStartYear();
    if(Flag & 0x01){
      *pString++ = (Year / 1000) + '0';
      Year %= 1000;
      *pString++ = (Year / 100) + '0';
      Flag |= 0x02;//强制
    }
    Year %= 100;
    if(Flag & 0x02){
      *pString++ = (Year / 10) + '0';
      *pString++ = (Year % 10) + '0';
    }
    *pString++ = '-';
  }
  unsigned char Data;
  //月
  if(Flag & 0x04){
    Data = ZipTime_GetInfo(1, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
    *pString++ = '-';
  }
  if(Flag & 0x08){//日
    Data = ZipTime_GetInfo(2, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
    *pString++ = ' ';
  }
  if(Flag & 0x10){//时
    Data = ZipTime_GetInfo(3, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
    *pString++ = ':';
  }
  if(Flag & 0x20){//分    
    Data = ZipTime_GetInfo(4, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
    *pString++ = ':';
  }
  if(Flag & 0x40){//秒
    Data = ZipTime_GetInfo(5, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
  }
  *pString = '\0';
}

