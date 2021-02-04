/*********************************************************************

                ΪӦ����չ����ѧ��ʵ��

*********************************************************************/

#include "math_3.h"

//---------------unsigned short����,�󴩹�ֱ���ϵĵ�y���꺯��------------
//ֱ�߱�ʾΪ:������ȷ����һ��ֱ��
//δ֪������ֱ���ϵ���֪x����,��y����
//ע:��֪y������x����,��ͨ������x.y������Ϊʵ��
//ע2:������(Ҳû���ж�)��λ��y���������
unsigned short Line_GetYInP2S(unsigned short x0,  //��0��x����
                              unsigned short y0,  //��0��y����
                              unsigned short x1,  //��1��x����,x1 != x0
                              unsigned short y1,  //��1��y����,y1 != y0
                              unsigned short x)  //δ֪y���x����
{
  unsigned short Data;
  unsigned long lData;
  if(x0 > x1){//�������껥��
    Data = x0;
    x0 = x1;
    x1 = Data;
    Data = y0;
    y0 = y1;
    y1 = Data;
  }
  if(y0 > y1)//ֱ������Ϊ�½�
    Data = y0 - y1;
  else//ֱ������Ϊ����
    Data = y1 - y0;
  lData = ((unsigned long)Data << 16) / (x1 - x0); //б�ʣ�Q16��
  if(x >= x0){//��λ��x0֮��
    Data = (lData * (x - x0)) >> 16; //��������x0,��y���ϵ�����
    if(y0 > y1)//ֱ������Ϊ�½�
      return y0 - Data;
    else//ֱ������Ϊ����
      return y0 + Data;
  }
  //��λ��x0֮ǰ
  Data = (lData * (x0 - x)) >> 16; //��������x0,��y���ϵ�����
  if(y0 > y1)//ֱ������Ϊ�½�
    return y0 + Data;
  else//ֱ������Ϊ����
    return y0 - Data;
}

//---------------unsigned short����,�󴩹�ֱ���ϵĵ�y���꺯��------------
//ֱ�߱�ʾΪ:ֱ�ߵ�б�ʺʹ���ֱ�ߵ�һ��
//δ֪������ֱ���ϵ���֪x����,��y����
//ע1:��б�ʲ���ʾ��ֵ,�ʽ��ܼ���������������,�½��ؿ�ͨ��ӳ��x��ʵ��
//ע2:������(Ҳû���ж�)��λ��y���������
unsigned short Line_GetYInP1MutiS(unsigned short x0,  //��0��x����
                                  unsigned short y0,  //��0��y����
                                  unsigned short Muti,//б��
                                  unsigned short MutiQ,//б�ʵ�Qֵ
                                  unsigned short x)  //δ֪y���x����
{
  unsigned long lData = Muti;
  unsigned short Data;
  if(x >= x0){//��λ��x0֮��
    Data = (lData * (x - x0)) >> MutiQ;
    return y0 + Data;
  }
  //��λ��x0֮ǰ
  Data = (lData * (x0 - x)) >> MutiQ;
  return y0 - Data;
}

//-----------unsigned short����,��һ��ֱ��(������)����б�ʺ���--------------
//ע:ֻ����б��Ϊ�����,��б�ʸ�ʱ,����0(��ֵ����),б�ʳ���ʱ����0xffff
unsigned short Line_GetMuti(unsigned short x0,  //��0��x����
                            unsigned short y0,  //��0��y����
                            unsigned short x1,  //��1��x����
                            unsigned short y1,  //��1��y����                      
                            unsigned short MutiQ)//б�ʵ�Qֵ
{
  unsigned short Data;
  unsigned long lData;
  if(x0 > x1){//�������껥��
    Data = x0;
    x0 = x1;
    x1 = Data;
    Data = y0;
    y0 = y1;
    y1 = Data;
  }
  
  if(y0 > y1){//ֱ������Ϊ�½�
    //Data = y0 - y1;
    return 0;//ֱ������Ϊ�½�
  }
  else//ֱ������Ϊ����
    Data = y1 - y0;
  lData = ((unsigned long)Data << MutiQ) / (x1 - x0);
  if(lData > 65536) return 0xffff; //����
  return (unsigned short)lData;
}


//---------------unsigned short����,�󴩹�ֱ���ϵĵ�y���꺯��------------
//ֱ�߱�ʾΪ:б�ʺʹ���y���xֵ,
//δ֪������ֱ���ϵ���֪x����,��y����
//ע1.��б�ʲ���ʾ��ֵ,�ʽ��ܼ���������������,�½��ؿ�ͨ��ӳ��x��ʵ��
//ע2:����λ��y�����,�򷵻�Ϊ0
unsigned short Line_GetYInZeroMutiSU(unsigned short xz,  //ֱ�ߴ���y���xֵ
                                     unsigned short Muti,//б��
                                     unsigned short MutiQ,//б�ʵ�Qֵ
                                     unsigned short x)  //δ֪y���x����
{
  unsigned long lData = Muti;
  if(x >= xz){//��λ��y���Ҳ�
    return (signed short)((lData * (x - xz)) >> MutiQ);
  }
  ////��λ��y�����,Ϊ����
  return 0;
}

//---------------unsigned short����,�󴩹�ֱ���ϵĵ�y���꺯��------------
//ֱ�߱�ʾΪ:б�ʺʹ���y���xֵ,
//δ֪������ֱ���ϵ���֪x����,��y����
//ע1.��б�ʲ���ʾ��ֵ,�ʽ��ܼ���������������,�½��ؿ�ͨ��ӳ��x��ʵ��
//ע2:�����˵�λ��y���������,������Ϊ��
signed short Line_GetYInZeroMutiSS(unsigned short xz,  //ֱ�ߴ���y���xֵ
                                   unsigned short Muti,//б��
                                   unsigned short MutiQ,//б�ʵ�Qֵ
                                   unsigned short x)  //δ֪y���x����
{
  unsigned long lData = Muti;
  if(x >= xz){//��λ��y���Ҳ�
    return (signed short)((lData * (x - xz)) >> MutiQ);
  }
  ////��λ��y�����,Ϊ����
  return (signed short)0 - ((lData * (xz - x)) >> MutiQ);
}

//-----------------------long����,��LSB��ʽ��亯��--------------------
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

//-------------------LSB��ʽ����ת��Ϊlong��������--------------------
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

//----------------------short����,��LSB��ʽ��亯��-------------------
void LsbS2Full(unsigned short Data,unsigned char *pData)
{
  *pData++ = Data;
  Data >>= 8;
  *pData = Data;
}

//--------------------LSB��ʽ����ת��Ϊshort��������--------------------
unsigned short LsbFull2S(const unsigned char *pData)
{
  unsigned short Data = (unsigned short)*pData++;
  return Data | (*pData << 8);
}

//-----------------------long����,��MSB��ʽ��亯��--------------------
void MsbL2Full(unsigned long Data,unsigned char *pData)
{
  //Ϊ��֤���룬û����memcpy;
  *pData++ = (unsigned char)(Data >> 24);
  *pData++ = (unsigned char)(Data >> 16);
  *pData++ = (unsigned char)(Data >> 8);
  *pData = (unsigned char)(Data); 
}

//-------------------MSB��ʽ����ת��Ϊlong��������--------------------
unsigned long MsbFull2L(const unsigned char *pData)
{
  unsigned long Data;
  Data = (unsigned long )(*pData++) << 24;
  Data |= (unsigned long )(*pData++) << 16;
  Data |= (unsigned long )(*pData++) << 8;
  Data |= (unsigned long )(*pData);   
  return Data;
}

//----------------------short����,��MSB��ʽ��亯��-------------------
void MsbS2Full(unsigned short Data,unsigned char *pData)
{
  //Ϊ��֤���룬û����memcpy;
  *pData++ = (unsigned char)(Data >> 8);
  *pData = (unsigned char)(Data); 
}

//----------------------short����,��MSB��ʽ��亯��-------------------
//�����¸����λ��
unsigned char *pMsbS2Full(unsigned short Data,unsigned char *pData)
{
  //Ϊ��֤���룬û����memcpy;
  *pData++ = (unsigned char)(Data >> 8);
  *pData++ = (unsigned char)(Data);
  return pData;
}

//----------------------short������,��MSB��ʽ��亯��-------------------
//����������λ��
unsigned char * pMsbS2FullAry(const unsigned short *psData,
                               unsigned short Len, //short����
                               unsigned char *pcData)
{
  //Ϊ��֤���룬û����memcpy;
  for(; Len > 0; Len--){
    unsigned short Data = *psData++;
    *pcData++ = (unsigned char)(Data >> 8);
    *pcData++ = (unsigned char)(Data);
  }
  return pcData;
}

//--------------------MSB��ʽ����ת��Ϊshort��������--------------------
unsigned short MsbFull2S(const unsigned char *pData)
{
  unsigned short Data;
  Data = (unsigned short )(*pData++) << 8;
  Data |= (unsigned short )(*pData);   
  return Data;  
}

//--------------------MSB��ʽ����ת��Ϊshort�����麯��--------------------
//������1��0�б䶯
signed char MsbFull2SAry(unsigned short *psData,
                         unsigned short Len, //short����
                         const unsigned char *pcData)
{
  signed char Resume = 0;
  //Ϊ��֤���룬û����memcpy;
  for(; Len > 0; Len--, psData++){
    unsigned short Data;
    Data = (unsigned short)(*pcData++) << 8;
    Data |= (unsigned short)(*pcData++); 
    if(*psData != Data){//�б仯��
      *psData = Data;
      Resume = 1;
    }
  }
  return Resume; 
}

//----------------------------����˫�ֽ��ַ�����λ��-------------------------
//�����ҵ�λ�ã�>=���ұ��Сʱ��ʾδ�ҵ�
unsigned short FindUsPos(const unsigned short *pLUT, //���ұ�
                           unsigned short LutLen,     //���ұ���
                           unsigned short Id)         //Ҫ���ҵ�ID��
{
  unsigned short Pos = 0;
  for(; Pos < LutLen; Pos++, pLUT++){
    if(*pLUT == Id) break;
  }
  return Pos;
}



