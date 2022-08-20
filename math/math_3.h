/*********************************************************************

                ΪӦ����չ����ѧ��ʵ��

*********************************************************************/

#ifndef _MATH_3_H
#define _MATH_3_H

//---------------unsigned short����,�󴩹�ֱ���ϵĵ�y���꺯��------------
//ֱ�߱�ʾΪ:������ȷ����һ��ֱ��
//δ֪������ֱ���ϵ���֪x����,��y����
//ע:��֪y������x����,��ͨ������x.y������Ϊʵ��
//ע2:������(Ҳû���ж�)��λ��y���������
unsigned short Line_GetYInP2S(unsigned short x0,  //��0��x����
                              unsigned short y0,  //��0��y����
                              unsigned short x1,  //��1��x����,x1 != x0
                              unsigned short y1,  //��1��y����,y1 != y0
                              unsigned short x);  //δ֪y���x����

//---------------unsigned short����,�󴩹�ֱ���ϵĵ�y���꺯��------------
//ֱ�߱�ʾΪ:ֱ�ߵ�б�ʺʹ���ֱ�ߵ�һ��
//δ֪������ֱ���ϵ���֪x����,��y����
//ע1:��б�ʲ���ʾ��ֵ,�ʽ��ܼ���������������,�½��ؿ�ͨ��ӳ��x��ʵ��
//ע2:������(Ҳû���ж�)��λ��y���������
unsigned short Line_GetYInP1MutiS(unsigned short x0,  //��0��x����
                                  unsigned short y0,  //��0��y����
                                  unsigned short Muti,//б��
                                  unsigned short MutiQ,//б�ʵ�Qֵ
                                  unsigned short x);  //δ֪y���x����

//---------------unsigned short����,�󴩹�ֱ���ϵĵ�y���꺯��------------
//ֱ�߱�ʾΪ:б�ʺʹ���y���xֵ,
//δ֪������ֱ���ϵ���֪x����,��y����
//ע1.��б�ʲ���ʾ��ֵ,�ʽ��ܼ���������������,�½��ؿ�ͨ��ӳ��x��ʵ��
//ע2:����λ��y�����,�򷵻�Ϊ0
unsigned short Line_GetYInZeroMutiSU(unsigned short xz,  //ֱ�ߴ���y���xֵ
                                     unsigned short Muti,//б��
                                     unsigned short MutiQ,//б�ʵ�Qֵ
                                     unsigned short x);  //δ֪y���x����
                                     
//---------------unsigned short����,�󴩹�ֱ���ϵĵ�x���꺯��------------
//ֱ�߱�ʾΪ:б�ʺʹ���y���xֵ,
//δ֪������ֱ���ϵ���֪y����,��x����
//ע1.��б�ʲ���ʾ��ֵ,�ʽ��ܼ���������������,�½��ؿ�ͨ��ӳ��x��ʵ��
//ע2:����λ��y�����,�򷵻�Ϊ0
unsigned short Line_GetXInZeroMutiSU(unsigned short xz,  //ֱ�ߴ���y���xֵ
                                     unsigned short Muti,//б��
                                     unsigned short MutiQ,//б�ʵ�Qֵ
                                     unsigned short y);  //δ֪y���x����
                                     
//---------------unsigned short����,�󴩹�ֱ���ϵĵ�x���꺯��------------
//ֱ�߱�ʾΪ:б�ʺʹ���y���xֵ,
//δ֪������ֱ���ϵ���֪y����,��x����
//ע1.��б�ʲ���ʾ��ֵ,�ʽ��ܼ���������������,�½��ؿ�ͨ��ӳ��x��ʵ��
//ע2:����λ��y�����,�򷵻�Ϊ0
signed short Line_GetXzInZeroMutiSU(unsigned short x,  //xֵ
                                     unsigned short Muti,//б��
                                     unsigned short MutiQ,//б�ʵ�Qֵ
                                     unsigned short y);  //x��Ӧ�ĵ�y����
                                       
//---------------unsigned short����,�󴩹�ֱ���ϵĵ�y���꺯��------------
//ֱ�߱�ʾΪ:б�ʺʹ���y���xֵ,
//δ֪������ֱ���ϵ���֪x����,��y����
//ע1.��б�ʲ���ʾ��ֵ,�ʽ��ܼ���������������,�½��ؿ�ͨ��ӳ��x��ʵ��
//ע2:�����˵�λ��y���������,������Ϊ��
signed short Line_GetYInZeroMutiSS(unsigned short xz,  //ֱ�ߴ���y���xֵ
                                   unsigned short Muti,//б��
                                   unsigned short MutiQ,//б�ʵ�Qֵ
                                   unsigned short x);  //δ֪y���x����

//-----------unsigned short����,��һ��ֱ��(������)����б�ʺ���--------------
//ע:ֻ����б��Ϊ�����,��б�ʸ�ʱ,����0(��ֵ����),б�ʳ���ʱ����0xffff
unsigned short Line_GetMuti(unsigned short x0,  //��0��x����
                            unsigned short y0,  //��0��y����
                            unsigned short x1,  //��1��x����
                            unsigned short y1,  //��1��y����                      
                            unsigned short MutiQ);//б�ʵ�Qֵ
//-----------------------long����,��LSB��ʽ��亯��--------------------
void LsbL2Full(unsigned long Data,unsigned char *pData);

//-------------------LSB��ʽ����ת��Ϊlong��������--------------------
unsigned long LsbFull2L(const unsigned char *pData);

//----------------------short����,��LSB��ʽ��亯��-------------------
void LsbS2Full(unsigned short Data,unsigned char *pData);

//----------------------short����,��MSB��ʽ��亯��-------------------
//�����¸����λ��
unsigned char *pLsbS2Full(unsigned short Data,unsigned char *pData);
                            
//----------------------lonng ������λ���,��MSB��ʽ��亯��-------------------
//������䳬��ֵ�������¸����λ��
unsigned char *pMsbS2FullOV(unsigned long Data,unsigned char *pData);

//--------------------LSB��ʽ����ת��Ϊshort��������--------------------
unsigned short LsbFull2S(const unsigned char *pData);

//-----------------------long����,��MSB��ʽ��亯��--------------------
void MsbL2Full(unsigned long Data,unsigned char *pData);

//-------------------MSB��ʽ����ת��Ϊlong��������--------------------
unsigned long MsbFull2L(const unsigned char *pData);

//----------------------short����,��MSB��ʽ��亯��-------------------
void MsbS2Full(unsigned short Data,unsigned char *pData);

//----------------------short����,��MSB��ʽ��亯��-------------------
//�����¸����λ��
unsigned char *pMsbS2Full(unsigned short Data,unsigned char *pData);

//----------------------short������,��MSB��ʽ��亯��-------------------
//����������λ��
unsigned char *pMsbS2FullAry(const unsigned short *psData,
                             unsigned short Len, //short����
                             unsigned char *pcData);

//--------------------MSB��ʽ����ת��Ϊshort��������--------------------
unsigned short MsbFull2S(const unsigned char *pData);

//--------------------MSB��ʽ����ת��Ϊshort�����麯��--------------------
//������1��0�б䶯
signed char MsbFull2SAry(unsigned short *psData,
                         unsigned short Len, //short����
                         const unsigned char *pcData);

//----------------------------����˫�ֽ��ַ�����λ��-------------------------
//�����ҵ�λ�ã�>=���ұ��Сʱ��ʾδ�ҵ�
unsigned short FindUsPos(const unsigned short *pLUT, //���ұ�
                           unsigned short LutLen,     //���ұ���
                           unsigned short Id);         //Ҫ���ҵ�ID��
                           
//-------------------------ʮ�������ַ�ת���ֽ�----------------------
//ASC:��A��~��F���롮0��~��9�� ת��Ϊ���ֽڣ�δ������ݷ�����
unsigned char Asc2HalfByte(unsigned char L);

//-------------------------ʮ�������ַ�ת�ֽ�----------------------
//�ߵ�λ ASC:��A��~��F���롮0��~��9�� ת��Ϊ���ֽڣ�δ������ݷ�����
unsigned char Asc2Byte(unsigned char H, unsigned char L);                          

//----------------------˫��ת�ֽڷ��������-------------------------------
//����255����255
unsigned char Us2ByteFull(unsigned short Data);
                           
//----------------------˫�ֽ����ڴ�copy-------------------------------
void memcpy2(unsigned short *pDest,
             unsigned short *pSource,
             unsigned long Size); //˫�ֽ��ʹ�С   

//----------------------˫�ֽ����ڴ�����-------------------------------
void memset2(unsigned short *pDest,
             unsigned short Data,
             unsigned long Size); //˫�ֽ��ʹ�С                
       
//----------------------Signed short��ƽ��-------------------------------
//����ƽ�����ֵ
signed short AverageS2(signed short *pData, unsigned short Size);
             
                           
#endif

