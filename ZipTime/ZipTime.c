/************************************************************************

                          ѹ��ʱ���ʽģ��ʵ��
************************************************************************/

#include "ZipTime.h"

unsigned long RtcZipTime = 0; //��ǰʵʱʱ��

/************************************************************************
                           ��غ���ʵ��
************************************************************************/

//-----------------------����ͨʱ��õ�ѹ�����ʱ��----------------------
//�˺���δ����������ݵ���ȷ��
unsigned long ZipTime_FromTime(unsigned char RelativeYear, //�����ʼ��,0-63
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

//--------------------------��ѹ��ʱ��õ���Ҫ��Ԫ��-------------------------
//Pos����Ϊ������Ϣ:�����0,��1,��2,ʱ3,��4,��5
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
  return 0; //�쳣����
}

//-----------------------------�õ�����-----------------------------
//��0-4b,��5-8b,��9-15b��2000��Ϊ��׼
unsigned short ZipTime_GetDate(unsigned long ZipTime)
{
  unsigned short Date = ZipTime >> ZIP_TIME_DATE_SHIFT;
  return Date + ((unsigned short)(YEAR_START - 2000) << 9);
}  

//-------------------��ZipTime_GetDate()���ڵ�ѹ��ʱ��-----------------
//����2000��Ϊ��׼����,ת��Ϊ�������׼,ʱ�����0
unsigned long ZipTime_Date2Zip(unsigned short Date)
{
  unsigned long ZipTime = 
    (unsigned long)((Date >> 9) - (YEAR_START - 2000)) << ZIP_TIME_YEAR_SHIFT;
  ZipTime |= (unsigned long)(Date & 0x1ff) << ZIP_TIME_DATE_SHIFT;
  return ZipTime;
}

//--------------------------ѹ��ʱ���ʽתΪ������-------------------------
//�������̶�������ǰ���������
void ZipTime_ToData(unsigned long ZipTime, unsigned char *pBuf)
{
  //Ϊ��֤���룬û����memcpy;
  *pBuf++ = (unsigned char)(ZipTime >> 24);
  *pBuf++ = (unsigned char)(ZipTime >> 16);
  *pBuf++ = (unsigned char)(ZipTime >> 8);
  *pBuf = (unsigned char)(ZipTime);
}

//--------------------------�������ָ�Ϊѹ��ʱ���ʽ-------------------------
//�������̶�������ǰ���������
unsigned long ZipTime_FromData(const unsigned char *pBuf)
{
  //Ϊ��֤���룬û����memcpy;
  unsigned long ZipTime;
  ZipTime = (unsigned long )(*pBuf++) << 24;
  ZipTime |= (unsigned long )(*pBuf++) << 16;
  ZipTime |= (unsigned long )(*pBuf++) << 8;
  ZipTime |= (unsigned long )(*pBuf);  
  return ZipTime;
}

//--------------------------������ʱ���Ƿ���ȷ-------------------------
//���ط�0��ȷ�����򷵻�0
signed char ZipTime_IsCheckValid(const unsigned char *pBuf)
{
  //����У��
  if(*pBuf++ != 0) return 1;
  if(*pBuf++ != 0) return 1;
  if(*pBuf++ != 0) return 1;
  if(*pBuf != 0) return 1;
  return 0;
}
     
//--------------------------У��ѹ��ʱ���Ƿ���ȷ-------------------------
//����0��ȷ�����򷵻ؼ�û������ʼ����λ�ã���-1,��-2,ʱ-3,��-4,��-5
signed char ZipTime_Check(unsigned long ZipTime)
{
  //����У�鷵�أ�
  if(ZipTime == 0xffffffff) return -1;//��Ϊ16,����
  if(ZipTime == 0) return -1;//��Ϊ0,����
  //��ϸ���:
  unsigned char Info = ZipTime_GetInfo(1, ZipTime); //��
  if((Info < 1) || (Info > 12)) return -1;//����
  Info = ZipTime_GetInfo(2, ZipTime); //��
  if((Info < 1) || (Info > 31)) return -2;//����  
  Info = ZipTime_GetInfo(3, ZipTime); //ʱ
  if(Info >= 24) return -3;//����   
  Info = ZipTime_GetInfo(4, ZipTime); //��
  if(Info >= 60) return -4;//����    
  Info = ZipTime_GetInfo(5, ZipTime); //��
  if(Info >= 60) return -5;//����
      
  return 0;  //���ͨ����
}

//---------------------------ѹ��ʱ��ת��Ϊ���ĸ�ʽ�ַ�------------------------
//��ʽΪXXXX-XX-XX XX:XX:XX������,����-��-�� ʱ:��:��
//��־��λ����Ϊ:0bit:��ʾ��ǰ��λ, 1bit:��ʾ�����λ,2bit:��ӡ��
//3bit:��ӡ��, 4bit:��ӡʱ, 5bit,��ӡ��, 6bit:������ʾ��
void ZipTime_ToStringCh(char *pString, 
                        unsigned long ZipTime,
                        unsigned char Flag)
{
  if(Flag & 0x03){//����ݶ���ʱ
    unsigned short Year = ZipTime_GetInfo(0, ZipTime) + ZipTime_cbGetStartYear();
    if(Flag & 0x01){
      *pString++ = (Year / 1000) + '0';
      Year %= 1000;
      *pString++ = (Year / 100) + '0';
      Flag |= 0x02;//ǿ��
    }
    Year %= 100;
    if(Flag & 0x02){
      *pString++ = (Year / 10) + '0';
      *pString++ = (Year % 10) + '0';
    }
    *pString++ = '-';
  }
  unsigned char Data;
  //��
  if(Flag & 0x04){
    Data = ZipTime_GetInfo(1, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
    *pString++ = '-';
  }
  if(Flag & 0x08){//��
    Data = ZipTime_GetInfo(2, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
    *pString++ = ' ';
  }
  if(Flag & 0x10){//ʱ
    Data = ZipTime_GetInfo(3, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
    *pString++ = ':';
  }
  if(Flag & 0x20){//��    
    Data = ZipTime_GetInfo(4, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
    *pString++ = ':';
  }
  if(Flag & 0x40){//��
    Data = ZipTime_GetInfo(5, ZipTime);
    *pString++ = (Data / 10) + '0';
    *pString++ = (Data % 10) + '0';
  }
  *pString = '\0';
}

