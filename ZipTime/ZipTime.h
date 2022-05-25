/************************************************************************

                          压缩时间格式模块
此模块同时提供压缩时间格式实时时间
************************************************************************/
#ifndef __ZIP_TIME_H
#define __ZIP_TIME_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif
/************************************************************************
                           相关定义
************************************************************************/
//压缩时间格式将时间压缩到一个32位变量中,各位定义为:

//秒: 范围0-59
#define ZIP_TIME_SEC_SHIFT    0
#define ZIP_TIME_SEC_MASK    ((unsigned long)0x3f << ZIP_TIME_SEC_SHIFT)   
//分: 范围0-59
#define ZIP_TIME_MIN_SHIFT    6
#define ZIP_TIME_MIN_MASK    ((unsigned long)0x3f << ZIP_TIME_MIN_SHIFT) 
//时: 范围0-23
#define ZIP_TIME_HOUR_SHIFT   12
#define ZIP_TIME_HOUR_MASK    ((unsigned long)0x1f << ZIP_TIME_HOUR_SHIFT)
//时间掩码:
#define ZIP_TIME_TIME_MASK    (ZIP_TIME_SEC_MASK | ZIP_TIME_MIN_MASK | ZIP_TIME_HOUR_MASK)

//日: 范围1-31
#define ZIP_TIME_DATE_SHIFT   17
#define ZIP_TIME_DATE_MASK    ((unsigned long)0x1f << ZIP_TIME_DATE_SHIFT) 
//月: 范围1-12
#define ZIP_TIME_MOUTH_SHIFT   22
#define ZIP_TIME_MOUTH_MASK    ((unsigned long)0x0f << ZIP_TIME_MOUTH_SHIFT) 
//年：为相对值，范围0-64,起始年由回调函数决定
#define ZIP_TIME_YEAR_COUNT   0x3f  //最大表示范围
#define ZIP_TIME_YEAR_SHIFT   26
#define ZIP_TIME_YEAR_MASK    ((unsigned long)ZIP_TIME_YEAR_COUNT << ZIP_TIME_YEAR_SHIFT)
//日期掩码:
#define ZIP_TIME_DATT_SHIFT     ZIP_TIME_DATE_SHIFT
#define ZIP_TIME_DATT_MASK    (ZIP_TIME_DATE_MASK | ZIP_TIME_MOUTH_MASK | ZIP_TIME_YEAR_MASK)


//定义年表示最大表示范围，菜单或外部使用
#ifndef ZIP_TIME_YEAR_MAX
  #define ZIP_TIME_YEAR_MAX  ZIP_TIME_YEAR_COUNT //默认最大
#endif

extern unsigned long RtcZipTime; //当前实时时间

/************************************************************************
                           相关函数
************************************************************************/

//-----------------------由普通时间得到压缩后的时间----------------------
//此函数未检查输入数据的正确性
unsigned long ZipTime_FromTime(unsigned char RelativeYear, //相对起始年,0-63
                                unsigned char Mouth,
                                unsigned char Date,
                                unsigned char Hour,
                                unsigned char Minute,
                                unsigned char Sec);

//--------------------------由压缩时间得到需要的元素-------------------------
//Pos定义为返回信息:相对年0,月1,日2,时3,分4,秒5
unsigned char ZipTime_GetInfo(unsigned char Pos, unsigned long ZipTime);

//-----------------------------得到日期-----------------------------
//日0-4b,月5-8b,年9-15b以2000年为基准
unsigned short ZipTime_GetDate(unsigned long ZipTime);                                  
                   
//--------------------------压缩时间格式转为数据流-------------------------
//数据流固定年在最前，秒在最后
void ZipTime_ToData(unsigned long ZipTime, unsigned char *pBuf);

//--------------------------数据流恢复为压缩时间格式-------------------------
//数据流固定年在最前，秒在最后
unsigned long ZipTime_FromData(const unsigned char *pBuf);

//--------------------------数据流时间是否正确-------------------------
//返回非0正确，否则返回0
signed char ZipTime_IsCheckValid(const unsigned char *pBuf);

//-------------------------------更新压缩格式实时时间-------------------------
//void ZipTime_UdateRTC(unsigned long RtcZipTime);
#define ZipTime_UdateRTC(rtcZipTime) do{RtcZipTime = rtcZipTime;}while(0)

//----------------------------得到压缩格式实时时间----------------------------
//unsigned long ZipTime_GetRTC(void);
#define ZipTime_GetRTC() (RtcZipTime)

//--------------------------校验压缩时间是否正确-------------------------
//返回0正确，否则返回检没到的起始错误位置：月-1,日-2,时-3,分-4,秒-5
signed char ZipTime_Check(unsigned long ZipTime);

//---------------------------压缩时间转换为中文格式字符------------------------
//格式为XXXX-XX-XX XX:XX:XX的掩码,即年-月-日 时:分:秒
//标志按位定义为:0bit:显示年前两位, 1bit:显示年后两位,2bit:打印月
//3bit:打印日, 4bit:打印时, 5bit,打印分, 6bit:允许显示秒
void ZipTime_ToStringCh(char *pString, 
                        unsigned long ZipTime,
                        unsigned char Flag);

/************************************************************************
                           回调函数
************************************************************************/

//---------------------------------得到年起始----------------------------
//使用ZipTimeEx模块时，需从2000开始
//unsigned short ZipTime_cbGetStartYear(void);
#define ZipTime_cbGetStartYear() (YEAR_START)
#endif


