/*********************************************************************

                为应用扩展的数学库实现

*********************************************************************/

#ifndef _MATH_3_H
#define _MATH_3_H

//---------------unsigned short型数,求穿过直线上的点y坐标函数------------
//直线表示为:两个点确定的一条直线
//未知点落在直线上但仅知x坐标,求y坐标
//注:若知y坐标求x坐标,可通过交换x.y轴坐标为实现
//注2:不考虑(也没有判断)点位于y轴左测的情况
unsigned short Line_GetYInP2S(unsigned short x0,  //点0的x坐标
                              unsigned short y0,  //点0的y坐标
                              unsigned short x1,  //点1的x坐标,x1 != x0
                              unsigned short y1,  //点1的y坐标,y1 != y0
                              unsigned short x);  //未知y点的x坐标

//---------------unsigned short型数,求穿过直线上的点y坐标函数------------
//直线表示为:直线的斜率和穿过直线的一点
//未知点落在直线上但仅知x坐标,求y坐标
//注1:因斜率不表示负值,故仅能计算上升沿趋势线,下降沿可通过映向x轴实现
//注2:不考虑(也没有判断)点位于y轴左测的情况
unsigned short Line_GetYInP1MutiS(unsigned short x0,  //点0的x坐标
                                  unsigned short y0,  //点0的y坐标
                                  unsigned short Muti,//斜率
                                  unsigned short MutiQ,//斜率的Q值
                                  unsigned short x);  //未知y点的x坐标

//---------------unsigned short型数,求穿过直线上的点y坐标函数------------
//直线表示为:斜率和穿过y轴的x值,
//未知点落在直线上但仅知x坐标,求y坐标
//注1.因斜率不表示负值,故仅能计算上升沿趋势线,下降沿可通过映向x轴实现
//注2:若点位于y轴左测,则返回为0
unsigned short Line_GetYInZeroMutiSU(unsigned short xz,  //直线穿过y轴的x值
                                     unsigned short Muti,//斜率
                                     unsigned short MutiQ,//斜率的Q值
                                     unsigned short x);  //未知y点的x坐标

//---------------unsigned short型数,求穿过直线上的点y坐标函数------------
//直线表示为:斜率和穿过y轴的x值,
//未知点落在直线上但仅知x坐标,求y坐标
//注1.因斜率不表示负值,故仅能计算上升沿趋势线,下降沿可通过映向x轴实现
//注2:考虑了点位于y轴左测的情况,即允许为负
signed short Line_GetYInZeroMutiSS(unsigned short xz,  //直线穿过y轴的x值
                                   unsigned short Muti,//斜率
                                   unsigned short MutiQ,//斜率的Q值
                                   unsigned short x);  //未知y点的x坐标

//-----------unsigned short型数,由一条直线(即两点)得其斜率函数--------------
//注:只考虑斜率为正情况,当斜率负时,返回0(负值以下),斜率超限时返回0xffff
unsigned short Line_GetMuti(unsigned short x0,  //点0的x坐标
                            unsigned short y0,  //点0的y坐标
                            unsigned short x1,  //点1的x坐标
                            unsigned short y1,  //点1的y坐标                      
                            unsigned short MutiQ);//斜率的Q值
//-----------------------long型数,以LSB方式填充函数--------------------
void LsbL2Full(unsigned long Data,unsigned char *pData);

//-------------------LSB方式数据转换为long型数函数--------------------
unsigned long LsbFull2L(const unsigned char *pData);

//----------------------short型数,以LSB方式填充函数-------------------
void LsbS2Full(unsigned short Data,unsigned char *pData);

//----------------------short型数,以MSB方式填充函数-------------------
//返回下个填充位置
unsigned char *pLsbS2Full(unsigned short Data,unsigned char *pData);

//--------------------LSB方式数据转换为short型数函数--------------------
unsigned short LsbFull2S(const unsigned char *pData);

//-----------------------long型数,以MSB方式填充函数--------------------
void MsbL2Full(unsigned long Data,unsigned char *pData);

//-------------------MSB方式数据转换为long型数函数--------------------
unsigned long MsbFull2L(const unsigned char *pData);

//----------------------short型数,以MSB方式填充函数-------------------
void MsbS2Full(unsigned short Data,unsigned char *pData);

//----------------------short型数,以MSB方式填充函数-------------------
//返回下个填充位置
unsigned char *pMsbS2Full(unsigned short Data,unsigned char *pData);

//----------------------short型数组,以MSB方式填充函数-------------------
//返回填充结束位置
unsigned char *pMsbS2FullAry(const unsigned short *psData,
                             unsigned short Len, //short长度
                             unsigned char *pcData);

//--------------------MSB方式数据转换为short型数函数--------------------
unsigned short MsbFull2S(const unsigned char *pData);

//--------------------MSB方式数据转换为short型数组函数--------------------
//返回是1否0有变动
signed char MsbFull2SAry(unsigned short *psData,
                         unsigned short Len, //short长度
                         const unsigned char *pcData);

//----------------------------查找双字节字符所在位置-------------------------
//返回找到位置，>=查找表大小时表示未找到
unsigned short FindUsPos(const unsigned short *pLUT, //查找表
                           unsigned short LutLen,     //查找表长度
                           unsigned short Id);         //要查找的ID号
                           
//-------------------------十六进制字符转半字节----------------------
//ASC:‘A’~‘F’与‘0’~‘9’ 转换为半字节，未检查数据符合性
unsigned char Asc2HalfByte(unsigned char L);

//-------------------------十六进制字符转字节----------------------
//高低位 ASC:‘A’~‘F’与‘0’~‘9’ 转换为半字节，未检查数据符合性
unsigned char Asc2Byte(unsigned char H, unsigned char L);                          

//----------------------双字转字节防溢出处理-------------------------------
//超过255返回255
unsigned char Us2ByteFull(unsigned short Data);
                           
#endif

