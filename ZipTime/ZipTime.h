/************************************************************************

                          ѹ��ʱ���ʽģ��
��ģ��ͬʱ�ṩѹ��ʱ���ʽʵʱʱ��
************************************************************************/
#ifndef __ZIP_TIME_H
#define __ZIP_TIME_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif
/************************************************************************
                           ��ض���
************************************************************************/
//ѹ��ʱ���ʽ��ʱ��ѹ����һ��32λ������,��λ����Ϊ:

//��: ��Χ0-59
#define ZIP_TIME_SEC_SHIFT    0
#define ZIP_TIME_SEC_MASK    ((unsigned long)0x3f << ZIP_TIME_SEC_SHIFT)   
//��: ��Χ0-59
#define ZIP_TIME_MIN_SHIFT    6
#define ZIP_TIME_MIN_MASK    ((unsigned long)0x3f << ZIP_TIME_MIN_SHIFT) 
//ʱ: ��Χ0-23
#define ZIP_TIME_HOUR_SHIFT   12
#define ZIP_TIME_HOUR_MASK    ((unsigned long)0x1f << ZIP_TIME_HOUR_SHIFT)
//ʱ������:
#define ZIP_TIME_TIME_MASK    (ZIP_TIME_SEC_MASK | ZIP_TIME_MIN_MASK | ZIP_TIME_HOUR_MASK)

//��: ��Χ1-31
#define ZIP_TIME_DATE_SHIFT   17
#define ZIP_TIME_DATE_MASK    ((unsigned long)0x1f << ZIP_TIME_DATE_SHIFT) 
//��: ��Χ1-12
#define ZIP_TIME_MOUTH_SHIFT   22
#define ZIP_TIME_MOUTH_MASK    ((unsigned long)0x0f << ZIP_TIME_MOUTH_SHIFT) 
//�꣺Ϊ���ֵ����Χ0-64,��ʼ���ɻص���������
#define ZIP_TIME_YEAR_COUNT   0x3f  //����ʾ��Χ
#define ZIP_TIME_YEAR_SHIFT   26
#define ZIP_TIME_YEAR_MASK    ((unsigned long)ZIP_TIME_YEAR_COUNT << ZIP_TIME_YEAR_SHIFT)
//��������:
#define ZIP_TIME_DATT_SHIFT     ZIP_TIME_DATE_SHIFT
#define ZIP_TIME_DATT_MASK    (ZIP_TIME_DATE_MASK | ZIP_TIME_MOUTH_MASK | ZIP_TIME_YEAR_MASK)


//�������ʾ����ʾ��Χ���˵����ⲿʹ��
#ifndef ZIP_TIME_YEAR_MAX
  #define ZIP_TIME_YEAR_MAX  ZIP_TIME_YEAR_COUNT //Ĭ�����
#endif

extern unsigned long RtcZipTime; //��ǰʵʱʱ��

/************************************************************************
                           ��غ���
************************************************************************/

//-----------------------����ͨʱ��õ�ѹ�����ʱ��----------------------
//�˺���δ����������ݵ���ȷ��
unsigned long ZipTime_FromTime(unsigned char RelativeYear, //�����ʼ��,0-63
                                unsigned char Mouth,
                                unsigned char Date,
                                unsigned char Hour,
                                unsigned char Minute,
                                unsigned char Sec);

//--------------------------��ѹ��ʱ��õ���Ҫ��Ԫ��-------------------------
//Pos����Ϊ������Ϣ:�����0,��1,��2,ʱ3,��4,��5
unsigned char ZipTime_GetInfo(unsigned char Pos, unsigned long ZipTime);

//-----------------------------�õ�����-----------------------------
//��0-4b,��5-8b,��9-15b��2000��Ϊ��׼
unsigned short ZipTime_GetDate(unsigned long ZipTime);                                  
                   
//--------------------------ѹ��ʱ���ʽתΪ������-------------------------
//�������̶�������ǰ���������
void ZipTime_ToData(unsigned long ZipTime, unsigned char *pBuf);

//--------------------------�������ָ�Ϊѹ��ʱ���ʽ-------------------------
//�������̶�������ǰ���������
unsigned long ZipTime_FromData(const unsigned char *pBuf);

//--------------------------������ʱ���Ƿ���ȷ-------------------------
//���ط�0��ȷ�����򷵻�0
signed char ZipTime_IsCheckValid(const unsigned char *pBuf);

//-------------------------------����ѹ����ʽʵʱʱ��-------------------------
//void ZipTime_UdateRTC(unsigned long RtcZipTime);
#define ZipTime_UdateRTC(rtcZipTime) do{RtcZipTime = rtcZipTime;}while(0)

//----------------------------�õ�ѹ����ʽʵʱʱ��----------------------------
//unsigned long ZipTime_GetRTC(void);
#define ZipTime_GetRTC() (RtcZipTime)

//--------------------------У��ѹ��ʱ���Ƿ���ȷ-------------------------
//����0��ȷ�����򷵻ؼ�û������ʼ����λ�ã���-1,��-2,ʱ-3,��-4,��-5
signed char ZipTime_Check(unsigned long ZipTime);

//---------------------------ѹ��ʱ��ת��Ϊ���ĸ�ʽ�ַ�------------------------
//��ʽΪXXXX-XX-XX XX:XX:XX������,����-��-�� ʱ:��:��
//��־��λ����Ϊ:0bit:��ʾ��ǰ��λ, 1bit:��ʾ�����λ,2bit:��ӡ��
//3bit:��ӡ��, 4bit:��ӡʱ, 5bit,��ӡ��, 6bit:������ʾ��
void ZipTime_ToStringCh(char *pString, 
                        unsigned long ZipTime,
                        unsigned char Flag);

/************************************************************************
                           �ص�����
************************************************************************/

//---------------------------------�õ�����ʼ----------------------------
//ʹ��ZipTimeExģ��ʱ�����2000��ʼ
//unsigned short ZipTime_cbGetStartYear(void);
#define ZipTime_cbGetStartYear() (YEAR_START)
#endif


