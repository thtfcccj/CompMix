/*********************************************************************

                为应用扩展的数学库实现

*********************************************************************/

#include "math_3.h"

//---------------unsigned short型数,求穿过直线上的点y坐标函数------------
//直线表示为:两个点确定的一条直线
//未知点落在直线上但仅知x坐标,求y坐标
//注:若知y坐标求x坐标,可通过交换x.y轴坐标为实现
//注2:不考虑(也没有判断)点位于y轴左测的情况
unsigned short Line_GetYInP2S(unsigned short x0,  //点0的x坐标
                              unsigned short y0,  //点0的y坐标
                              unsigned short x1,  //点1的x坐标,x1 != x0
                              unsigned short y1,  //点1的y坐标,y1 != y0
                              unsigned short x)  //未知y点的x坐标
{
  unsigned short Data;
  unsigned long lData;
  if(x0 > x1){//两点坐标互换
    Data = x0;
    x0 = x1;
    x1 = Data;
    Data = y0;
    y0 = y1;
    y1 = Data;
  }
  if(y0 > y1)//直线趋势为下降
    Data = y0 - y1;
  else//直线趋势为上升
    Data = y1 - y0;
  lData = ((unsigned long)Data << 16) / (x1 - x0); //斜率，Q16数
  if(x >= x0){//点位于x0之后
    Data = (lData * (x - x0)) >> 16; //求得相对于x0,在y轴上的增量
    if(y0 > y1)//直线趋势为下降
      return y0 - Data;
    else//直线趋势为上升
      return y0 + Data;
  }
  //点位于x0之前
  Data = (lData * (x0 - x)) >> 16; //求得相对于x0,在y轴上的增量
  if(y0 > y1)//直线趋势为下降
    return y0 + Data;
  else//直线趋势为上升
    return y0 - Data;
}

//---------------unsigned short型数,求穿过直线上的点y坐标函数------------
//直线表示为:直线的斜率和穿过直线的一点
//未知点落在直线上但仅知x坐标,求y坐标
//注1:因斜率不表示负值,故仅能计算上升沿趋势线,下降沿可通过映向x轴实现
//注2:不考虑(也没有判断)点位于y轴左测的情况
unsigned short Line_GetYInP1MutiS(unsigned short x0,  //点0的x坐标
                                  unsigned short y0,  //点0的y坐标
                                  unsigned short Muti,//斜率
                                  unsigned short MutiQ,//斜率的Q值
                                  unsigned short x)  //未知y点的x坐标
{
  unsigned long lData = Muti;
  unsigned short Data;
  if(x >= x0){//点位于x0之后
    Data = (lData * (x - x0)) >> MutiQ;
    return y0 + Data;
  }
  //点位于x0之前
  Data = (lData * (x0 - x)) >> MutiQ;
  return y0 - Data;
}

//-----------unsigned short型数,由一条直线(即两点)得其斜率函数--------------
//注:只考虑斜率为正情况,当斜率负时,返回0(负值以下),斜率超限时返回0xffff
unsigned short Line_GetMuti(unsigned short x0,  //点0的x坐标
                            unsigned short y0,  //点0的y坐标
                            unsigned short x1,  //点1的x坐标
                            unsigned short y1,  //点1的y坐标                      
                            unsigned short MutiQ)//斜率的Q值
{
  unsigned short Data;
  unsigned long lData;
  if(x0 > x1){//两点坐标互换
    Data = x0;
    x0 = x1;
    x1 = Data;
    Data = y0;
    y0 = y1;
    y1 = Data;
  }
  
  if(y0 > y1){//直线趋势为下降
    //Data = y0 - y1;
    return 0;//直线趋势为下降
  }
  else//直线趋势为上升
    Data = y1 - y0;
  lData = ((unsigned long)Data << MutiQ) / (x1 - x0);
  if(lData > 65536) return 0xffff; //超限
  return (unsigned short)lData;
}


//---------------unsigned short型数,求穿过直线上的点y坐标函数------------
//直线表示为:斜率和穿过y轴的x值,
//未知点落在直线上但仅知x坐标,求y坐标
//注1.因斜率不表示负值,故仅能计算上升沿趋势线,下降沿可通过映向x轴实现
//注2:若点位于y轴左测,则返回为0
unsigned short Line_GetYInZeroMutiSU(unsigned short xz,  //直线穿过y轴的x值
                                     unsigned short Muti,//斜率
                                     unsigned short MutiQ,//斜率的Q值
                                     unsigned short x)  //未知y点的x坐标
{
  unsigned long lData = Muti;
  if(x >= xz){//点位于y轴右侧
    return (signed short)((lData * (x - xz)) >> MutiQ);
  }
  //点位于y轴左侧,为负了
  return 0;
}

//---------------signed short型数,求穿过直线上的点x坐标函数------------
//直线表示为:斜率和穿过y轴的x值,
//未知点落在直线上但仅知y坐标,求x坐标
//注1.因斜率不表示负值,故仅能计算上升沿趋势线,下降沿可通过映向x轴实现
//注2:若点位于y轴左测,则返回为0
signed short Line_GetXInZeroMutiU(signed short xz,  //直线穿过y轴的x值
                                  unsigned short Muti,//斜率
                                  unsigned short MutiQ,//斜率的Q值
                                  signed short y)  //未知x点的y坐标
{
  signed long lData = (signed long)y *  (1 <<  MutiQ);
  lData /= Muti;
  lData += xz;
  if(lData < -32768) return -32768;
  if(lData > 32767) return 32767;  
  return lData;
}

//---------------signed short型数,求穿过直线上的点x坐标函数------------
//直线表示为:斜率和穿过y轴的x值,
//未知点落在直线上但仅知y坐标,求x坐标
//注1.因斜率不表示负值,故仅能计算上升沿趋势线,下降沿可通过映向x轴实现
//注2:若点位于y轴左测,则返回为0
signed short Line_GetXzInZeroMutiU(signed short x,  //x值
                                   unsigned short Muti,//斜率
                                   unsigned short MutiQ,//斜率的Q值
                                   signed short y)  //x对应的点y坐标
{
  signed long lData = (signed long)y * (1 <<  MutiQ);
  lData /= Muti;
  lData = (signed long)x - lData;
  if(lData < -32768) return -32768;
  if(lData > 32767) return 32767;  
  return lData;
}

//---------------unsigned short型数,求穿过直线上的点y坐标函数------------
//直线表示为:斜率和穿过y轴的x值,
//未知点落在直线上但仅知x坐标,求y坐标
//注1.因斜率不表示负值,故仅能计算上升沿趋势线,下降沿可通过映向x轴实现
//注2:考虑了点位于y轴左测的情况,即允许为负
signed short Line_GetYInZeroMutiSS(unsigned short xz,  //直线穿过y轴的x值
                                   unsigned short Muti,//斜率
                                   unsigned short MutiQ,//斜率的Q值
                                   unsigned short x)  //未知y点的x坐标
{
  unsigned long lData = Muti;
  if(x >= xz){//点位于y轴右侧
    return (signed short)((lData * (x - xz)) >> MutiQ);
  }
  //点位于y轴左侧,为负了
  return (signed short)0 - ((lData * (xz - x)) >> MutiQ);
}

//-----------------------long型数,以LSB方式填充函数--------------------
void LsbL2Full(unsigned long Data,unsigned char *pData)
{
  *pData++ = Data;
  Data >>= 8;
  *pData++ = Data;
  Data >>= 8;
  *pData++ = Data;
  Data >>= 8;
  *pData = Data;
}

//-------------------LSB方式数据转换为long型数函数--------------------
unsigned long LsbFull2L(const unsigned char *pData)
{
  unsigned long Data;
  pData += 3;
  Data = *pData--;
  Data <<= 8;
  Data |= *pData--;
  Data <<= 8;
  Data |= *pData--;
  Data <<= 8;
  Data |= *pData;
  return Data;
}

//----------------------short型数,以LSB方式填充函数-------------------
void LsbS2Full(unsigned short Data,unsigned char *pData)
{
  *pData++ = Data;
  Data >>= 8;
  *pData = Data;
}

//----------------------short型数,以LSB方式填充函数-------------------
//返回下个填充位置
unsigned char *pLsbS2Full(unsigned short Data,unsigned char *pData)
{
  *pData++ = Data;
  Data >>= 8;
  *pData++ = Data;  
  return pData;
}

//--------------------LSB方式数据转换为short型数函数--------------------
unsigned short LsbFull2S(const unsigned char *pData)
{
  unsigned short Data = (unsigned short)*pData++;
  return Data | (*pData << 8);
}

//-----------------------long型数,以MSB方式填充函数--------------------
void MsbL2Full(unsigned long Data,unsigned char *pData)
{
  //为保证对齐，没有用memcpy;
  *pData++ = (unsigned char)(Data >> 24);
  *pData++ = (unsigned char)(Data >> 16);
  *pData++ = (unsigned char)(Data >> 8);
  *pData = (unsigned char)(Data); 
}

//-------------------MSB方式数据转换为long型数函数--------------------
unsigned long MsbFull2L(const unsigned char *pData)
{
  unsigned long Data;
  Data = (unsigned long )(*pData++) << 24;
  Data |= (unsigned long )(*pData++) << 16;
  Data |= (unsigned long )(*pData++) << 8;
  Data |= (unsigned long )(*pData);   
  return Data;
}

//----------------------short型数,以MSB方式填充函数-------------------
void MsbS2Full(unsigned short Data,unsigned char *pData)
{
  //为保证对齐，没有用memcpy;
  *pData++ = (unsigned char)(Data >> 8);
  *pData = (unsigned char)(Data); 
}

//----------------------short型数,以MSB方式填充函数-------------------
//返回下个填充位置
unsigned char *pMsbS2Full(unsigned short Data,unsigned char *pData)
{
  //为保证对齐，没有用memcpy;
  *pData++ = (unsigned char)(Data >> 8);
  *pData++ = (unsigned char)(Data);
  return pData;
}

//----------------------lonng 型数两位填充,以MSB方式填充函数-------------------
//超限填充超限值，返回下个填充位置
unsigned char *pMsbS2FullOV(unsigned long Data,unsigned char *pData)
{
  if(Data > 0xffff) Data = 0xffff;
  //为保证对齐，没有用memcpy;
  *pData++ = (unsigned char)(Data >> 8);
  *pData++ = (unsigned char)(Data);
  return pData;
}

//----------------------short型数组,以MSB方式填充函数-------------------
//返回填充结束位置
unsigned char * pMsbS2FullAry(const unsigned short *psData,
                               unsigned short Len, //short长度
                               unsigned char *pcData)
{
  //为保证对齐，没有用memcpy;
  for(; Len > 0; Len--){
    unsigned short Data = *psData++;
    *pcData++ = (unsigned char)(Data >> 8);
    *pcData++ = (unsigned char)(Data);
  }
  return pcData;
}

//--------------------MSB方式数据转换为short型数函数--------------------
unsigned short MsbFull2S(const unsigned char *pData)
{
  unsigned short Data;
  Data = (unsigned short )(*pData++) << 8;
  Data |= (unsigned short )(*pData);   
  return Data;  
}

//--------------------MSB方式数据转换为short型数组函数--------------------
//返回是1否0有变动
signed char MsbFull2SAry(unsigned short *psData,
                         unsigned short Len, //short长度
                         const unsigned char *pcData)
{
  signed char Resume = 0;
  //为保证对齐，没有用memcpy;
  for(; Len > 0; Len--, psData++){
    unsigned short Data;
    Data = (unsigned short)(*pcData++) << 8;
    Data |= (unsigned short)(*pcData++); 
    if(*psData != Data){//有变化了
      *psData = Data;
      Resume = 1;
    }
  }
  return Resume; 
}

//----------------------------查找双字节字符所在位置-------------------------
//返回找到位置，>=查找表大小时表示未找到
unsigned short FindUsPos(const unsigned short *pLUT, //查找表
                           unsigned short LutLen,     //查找表长度
                           unsigned short Id)         //要查找的ID号
{
  unsigned short Pos = 0;
  for(; Pos < LutLen; Pos++, pLUT++){
    if(*pLUT == Id) break;
  }
  return Pos;
}

//-------------------------十六进制字符转半字节----------------------
//ASC:‘A’~‘F’与‘0’~‘9’ 转换为半字节，未检查数据符合性
unsigned char Asc2HalfByte(unsigned char L)
{
  if(L >= 'A') return L - ('A' - 10); //A开始
  else return L - '0'; //0开始
}

//-------------------------十六进制字符转字节----------------------
//高低位 ASC:‘A’~‘F’与‘0’~‘9’ 转换为半字节，未检查数据符合性
unsigned char Asc2Byte(unsigned char H, unsigned char L)
{
  unsigned char Data;
  if(H >= 'A') Data = H - ('A' - 10); //A开始
  else Data = H - '0'; //0开始
  Data <<= 4;
  if(L >= 'A') return Data + (L - ('A' - 10)); //A开始
  else return Data + (L - '0'); //0开始
}

//----------------------双字转字节防溢出处理-------------------------------
//超过255返回255
unsigned char Us2ByteFull(unsigned short Data)
{
  if(Data <= 255) return Data;
  return 255;
}

//------------------------------BCD转换-----------------------------
unsigned char Bcd2Data(unsigned char BCD)
{
  return (((BCD >> 4)) * 10) + (BCD & 0x0f);
}
unsigned char Data2Bcd(unsigned char Data)
{
  return ((Data / 10) << 4) | (Data % 10);
}

//----------------------双字节型内存设置-------------------------------
void memset2(unsigned short *pDest,
             unsigned short Data,
             unsigned long Size) //双字节型大小        
{
  for(; Size > 0; Size--){
    *pDest++ = Data;
  }
}            
       
//----------------------Signed short求平均-------------------------------
//返回平均后的值
signed short AverageS2(signed short *pData, unsigned short Size)
{
  signed long Count = 0;
  signed short *pEndData = pData + Size;
  for(; pData < pEndData; pData++){
    Count += *pData;
  }  
  return Count / Size;
}


