/*******************************************************************************

                    �û��ɶ��Ʋ��ұ�ģ��
��ģ�������ϵͳ,����Ӧ���޹�
ע:��֧�ֵ��������Ĳ��ұ�ṹ
********************************************************************************/
#ifndef __CUSTOM_LUT_H
#define __CUSTOM_LUT_H

/*******************************************************************************
                                �������
*******************************************************************************/

//�����������С,�����������ٸ���
#ifndef CUSTOM_LUT_LUT_SIZE
  #define CUSTOM_LUT_LUT_SIZE    10     //Ĭ�����10����
#endif

/*******************************************************************************
                                 ��ؽṹ
*******************************************************************************/
#include "Eeprom.h"
#include "NolinearConvert.h"
//��Ϣ�ṹ
struct _CustomLutInfo{
  //���Բ��ұ�,��0xff��β
  struct _NolinearConvertTbl Tbl[CUSTOM_LUT_LUT_SIZE];
};

//���нṹ
struct _CustomLut{
  unsigned char CustomLutId;         
  struct _CustomLutInfo Info; //��Ϣ�ṹ
};

/********************************************************************************
                        ��غ���
********************************************************************************/

//--------------------------------��ʼ������------------------------------------
void CustomLut_Init(struct _CustomLut *pCustomLut,
                    unsigned char CustomLutId,//�˱��ID�ţ����ڶ������ֽṹ
                    signed char Inited);//�β�Ϊ�ѳ�ʼ����־

//----------------------------�õ����ұ���------------------------------------
#define CustomLut_pGetLut(customLut)   ((customLut)->Info)

//----------------------------�õ����ұ����-------------------------------
#define CustomLut_GetLutItem(customLut, item)  ((customLut)->Info.Tbl[item])

//----------------------------�õ����ұ��С����--------------------------------
unsigned char CustomLut_GetSize(struct _CustomLut *pCustomLut);

//-----------------------���Ը���һ�����ұ����------------------------------
//�����Ƿ񸲸ǳɹ�(0�ɹ�,�������ɹ�)
signed char CustomLut_ReplaceItem(struct _CustomLut *pCustomLut,
                                  NolinearConvert_t Source,//Դ����
                                  NolinearConvert_t Destination);//Ŀ������

//----------------------------����һ�����ұ����------------------------------
//�����Ƿ����ɹ�
signed char CustomLut_AddItem(struct _CustomLut *pCustomLut,
                               NolinearConvert_t Source,//Դ����
                               NolinearConvert_t Destination);//Ŀ������

//----------------------------ɾ��һ�����ұ����------------------------------
//�����Ƿ�ɹ�
signed char CustomLut_DelItem(struct _CustomLut *pCustomLut,
                               unsigned char Item);//��λ��

//----------------------------���ұ���Ĭ��ֵ����--------------------------------
//�����ұ��ó���ǰ״̬
void CustomLut_Restore(struct _CustomLut *pCustomLut);

/********************************************************************************
                        �ص�����
********************************************************************************/

//----------------------------������Ϣ�ṹ��ַ----------------------------------
EepromAdr_t CustomLut_cbGetInfoBase(unsigned char CustomLutId);

//----------------------------���Ĭ�ϲ��ұ�ṹ����----------------------------
//Ӧ����Ӧ���������,���ұ��СΪCUSTOM_LUT_LUT_SIZE
void CustomLut_cbFullDefaultLut(struct _NolinearConvertTbl *pTbl);

//----------------------------�õ��ٽ���Դ���ݲ�ֵ����-------------------------
//�˺������ڲ�������ʱ,�жϲ���������Ƿ��ظ�,���ظ���������ԭ����ֵ
//��֧�ָ��Բ��ұ�ʱ,�ɲ�ʵ�ִ˺���
unsigned char CustomLut_cbGetNearSorceDiff(unsigned char CustomLutId);


#endif //#define __CUSTOM_LUT_H
