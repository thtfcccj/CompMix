/*******************************************************************************

     �¶�ģ��Ӳ����ض���-ʹ���ⲿ10KNTC, 3380K ���߲�����ʵ��

*******************************************************************************/
#include "Temp.h"
#include "NolinearConvert.h"

/*******************************************************************************
                            ��ػص�����ʵ�� 
*******************************************************************************/
#ifdef SUPPORT_TEMP_LUT

//------------------------------------У׼����--------------------------------
//������Ʒ������ȡ������,���Ҳȡ8����
static const struct _NolinearConvertTbl _OrgTemp2Temp[] = {
  /* 0 */ (128 - 33)<<7, (64 - 40)<<8,//16.5
  /* 1 */ (128 - 31)<<7, (64 - 30)<<8,//15.5
  /* 2 */ (64 - 14)<<8,  (64 - 25)<<8,
  /* 3 */ (64 - 10)<<8,  (64 - 15)<<8, 
  /* 4 */ (64 - 0 )<<8,  (64 -  0)<<8,  //У׼��1
  /* 5 */ (64 + 40)<<8,  (64 + 40)<<8,  //У׼��2, ��������1.2�� 
  /* 6 */ (64 + 54)<<8,  (64 + 55)<<8,  //��������
  /* 7 */ (64 + 65)<<8,  (64 + 70)<<8,  //��5��
};

//------------------------------------�¶�У��--------------------------------
//�磺���Զ�У�����������Ӵ�����,����ΪQ8�¶ȣ����Ϊ����¶�
unsigned char Temp_cbAppendPro(unsigned short OrgTempQ8)
{
  OrgTempQ8 = NolinearConvert(OrgTempQ8, //����Դ
                             _OrgTemp2Temp,  //������������ұ��������
                             8);
  if(OrgTempQ8 & 0x80) OrgTempQ8 += 0x80; //��������
  return (unsigned char)(OrgTempQ8 >> 8);
}

//----------------------------------�¶ȷ�У��---------------------------------
//��Temp_cbAppendPro()�ɶ�ʹ��,����Ϊ����¶ȣ����Ϊԭʼ�¶�(��Q8)
unsigned char Temp_cbAntiAppendPro(unsigned char CurTemp)
{
  NolinearConvert_AntiCopy(NolinearConvertBuf.Tbl, //ʹ�û���������
                           _OrgTemp2Temp, 
                           8);
  unsigned short Q8 = (unsigned short)CurTemp << 8;
  Q8 = NolinearConvert(Q8, //����Դ
                       NolinearConvertBuf.Tbl,  //������������ұ��������
                       8);
  if(Q8 & 0x80) Q8 += 0x80; //��������
  return (unsigned char)(Q8 >> 8);
}

#endif //SUPPORT_TEMP_LUT
