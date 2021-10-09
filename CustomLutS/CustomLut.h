/***********************************************************************

               (������)���Ʒ�����ת�����ұ�ģ��
��ģ�������ϵͳ,����Ӧ���޹�
ע:��֧�ֵ��������Ĳ��ұ�ṹ
***********************************************************************/
#ifndef __CUSTOM_LUT_H
#define __CUSTOM_LUT_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/*********************************************************************
                        �������
*********************************************************************/

//�����������С,�����������ٸ���
#ifndef CUSTOM_LUT_LUT_SIZE
  #define CUSTOM_LUT_LUT_SIZE    8
#endif

/*********************************************************************
                        ��ؽṹ
*********************************************************************/

#include "NolinearConvert.h"
//�˽ṹ����EEPROM
extern struct _NolinearConvertTbl CustomLut[CUSTOM_LUT_LUT_SIZE];

/*********************************************************************
                        ��غ���
*********************************************************************/

//-----------------------��ʼ������---------------------------------------
void CustomLut_Init(signed char Inited);

//-----------------------�õ����ұ���------------------------------------
//const struct _NolinearConvertTbl *CustomLut_GetLut(void);
#define CustomLut_GetLut()  (CustomLut)

//-----------------------�õ����ұ����------------------------------------
//const struct _NolinearConvertTbl *CustomLut_GetLutItem(unsigned char Item);
#define CustomLut_GetLutItem(item)  (&CustomLut[item])

//-----------------------�õ����ұ��С����--------------------------------
unsigned char CustomLut_GetSize(void);

//-----------------------�Ƿ�Ϊ����������------------------------------
//��0�ڷ�Χ��0���ڷ�Χ
signed char CustomLut_IsSimilar(NolinearConvert_t Center,//��������
                                NolinearConvert_t Cur);   //��ǰ����

//-----------------------������Դ�����������------------------------------
//����Χ��ʱ����Ϊ�ҵ��ˣ�����λ�ã���δ�ҵ�
signed char CustomLut_FindSimilarItem(NolinearConvert_t Source);//Դ����

//-----------------------������Դ������ͬ���------------------------------
//��ͬʱ����Ϊ�ҵ��ˣ�����λ�ã���δ�ҵ�
signed char CustomLut_FindSameItem(NolinearConvert_t Source);//Դ����

//-----------------------���Ը���һ�����ұ����-------------------------
//�����Ƿ񸲸ǳɹ�(0�ɹ�,�������ɹ�)
signed char CustomLut_ReplaceItem(NolinearConvert_t Source,//Դ����
                                   NolinearConvert_t Destination);//Ŀ������

//-----------------------����һ�����ұ����------------------------------
//�����Ƿ����ɹ�
signed char CustomLut_AddItem(NolinearConvert_t Source,//Դ����
                               NolinearConvert_t Destination);//Ŀ������

//-----------------------����һ�����ұ����-------------------------
void CustomLut_InsertItem(NolinearConvert_t Source,//Դ����
                          NolinearConvert_t Destination);//Ŀ������

//-----------------------ɾ��һ�����ұ����------------------------------
//�����Ƿ�ɹ�
signed char CustomLut_DelItem(unsigned char Item);//��λ��,���λΪ0x80ʱǿ��ɾ��

//-----------------------���ұ���Ĭ��ֵ����---------------------------------
//�����ұ��ó���ǰ״̬
void CustomLut_Restore(void);

/******************************************************************************
                        �ص�����
******************************************************************************/

//-----------------------���Ĭ�ϲ��ұ�ṹ����--------------------------------
//Ӧ����Ӧ���������,���ұ��СΪCUSTOM_LUT_LUT_SIZE
void CustomLut_cbFullDefaultLut(void);

//-----------------------�õ��ٽ���Դ���ݲ�ֵ����------------------------------
//�˺������ڲ�������ʱ,�жϲ���������Ƿ��ظ�,���ظ���������ԭ����ֵ
//��֧�ָ��Բ��ұ�ʱ,�ɲ�ʵ�ִ˺���
unsigned char CustomLut_cbGetNearSorceDiff(void);

//-----------------------Ŀ�������Ƿ��ֹ�޸�------------------------------------
//�ڲ��ڸ�����ɾ��ǰ������ô˺���
signed char CustomLut_cbIsDisModify(NolinearConvert_t Destination);//Ŀ������


#endif //#define __CUSTOM_LUT_H
