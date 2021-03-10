/***********************************************************************

                    �û��ɶ��Ʋ��ұ�ģ��ʵ��
��ģ�������ϵͳ,����Ӧ���޹�
ע:��֧�ֵ��������Ĳ��ұ�ṹ
***********************************************************************/
#include "CustomLutAry.h"

struct _CustomLutAry CustomLutAry; //ʵ����

/********************************************************************************
                              ��غ���ʵ��
********************************************************************************/

//--------------------------------��ʼ������------------------------------------
void CustomLutAry_Init(signed char Inited)//�β�Ϊ�ѳ�ʼ����־
{
  for(unsigned char Id = 0; Id < CUSTOM_LUT_ARY_SIZE; Id++){
    CustomLut_Init(&CustomLutAry.Ary[Id],
                   Id,//�˱��ID�ţ����ڶ������ֽṹ
                   Inited);//�β�Ϊ�ѳ�ʼ����־
  }
}

//------------------------------��Id�õ�ʵ��------------------------------------
//δ�ҵ�ʱΪ�׸�
struct _CustomLut* CustomLutAry_pGet(unsigned char Id)
{
  if(Id >= CUSTOM_LUT_ARY_SIZE) Id = CUSTOM_LUT_ARY_SIZE - 1;
  return &CustomLutAry.Ary[Id];
}

//------------------------------��Idֱ�ӵõ����ұ�------------------------------
//δ�ҵ�ʱΪ�׸�
struct _NolinearConvertTbl *CustomLutAry_pGetLut(unsigned char Id)
{
  return CustomLutAry_pGet(Id)->Info.Tbl;
}