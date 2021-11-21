/************************************************************************

                      压缩时间格式模块扩展功能模块

************************************************************************/
#ifndef _ZIP_TIME_EX_H 
#define _ZIP_TIME_EX_H

//---------------------由时间差得到天数-----------------------
//小数天数直接去除,为负时返回0
unsigned short ZipTime_GetDaySub(unsigned long Sub,
                                  unsigned long Min);

//-----------------------加天数函数--------------------------------
unsigned long ZipTime_AddDay(unsigned long ZipTime,
                               unsigned short Day);

//-----------------------大小判断函数------------------------------
//当ZipTime0 > ZipTime1时返回真
//unsigned char ZipTime_CmpMore(unsigned long ZipTime0,
//                              unsigned long ZipTime1);
#define ZipTime_CmpMore(ziptime0,ziptime1) (ziptime0 > ziptime1)

//---------------------------得到一天中的秒----------------------
//如：2021.11.20 23:55:40，则将23:55:40换成秒后返回
unsigned long ZipTime_GetDaySec(unsigned long ZipTime);

//---------------得到前后时间相差的秒数--------------------------
//当前时间须 >= 上次时间,否则返回0
unsigned long ZipTime_GetSecSub(unsigned long PrvTime, //上次时间
                               unsigned long CurTime);//当前时间

#endif

