/*******************************************************************************

                    ��ʪ��(������)ģ��
��ģ��רΪ��ʪ�ȼ�׼�������㾫��:  �¶�0.1��1��(ʵ�־���), ʪ��1%RH
*******************************************************************************/
#ifndef __HUMITURE_H
#define __HUMITURE_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/*****************************************************************************
                             �������
*****************************************************************************/

//ƽ���˲��㷨�˲�����
#ifndef HUMITURE_FILTER_BUF_COUNT
  #define HUMITURE_FILTER_BUF_COUNT   16  
#endif

//�¶ȷ��������ID��
#ifndef HUMITURE_TEMP_KIND_ID
  #define HUMITURE_TEMP_KIND_ID   0  
#endif
//ʪ�ȷ��������ID��
#ifndef HUMITURE_RH_KIND_ID
  #define HUMITURE_RH_KIND_ID   1  
#endif

//�¶ȷ���ĵ�λID��
#ifndef HUMITURE_TEMP_UNIT_ID
  #define HUMITURE_TEMP_UNIT_ID   0  
#endif
//ʪ�ȷ���ĵ�λID��
#ifndef HUMITURE_RH_UNIT_ID
  #define HUMITURE_RH_UNIT_ID   1  
#endif

/*****************************************************************************
                             ��ؽṹ
*****************************************************************************/
#include "Sense.h"

//���ṹ
struct _Humiture{
  struct _Sense Sense[2]; //�������ź�
  signed short FilterBuf[2][HUMITURE_FILTER_BUF_COUNT];//�˲��㷨������
};

extern struct _Humiture Humiture;

/*******************************************************************************
                           ��غ���
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void Humiture_Init(unsigned char IsInited); //�Ƿ��ѳ�ʼ��

//---------------------------------�����¶�ֵ-----------------------------------
#define Humiture_UpdateTemp(temp) do{Sense_Update(&Humiture.Sense[0], temp);}while(0)

//---------------------------------����ʪ��ֵ-----------------------------------
#define Humiture_UpdateRh(rh) do{Sense_Update(&Humiture.Sense[1], rh);}while(0)

//----------------------------�¶�У׼����--------------------------------------
//0�궨�ɹ�������ʧ��!
#define Humiture_CalibrationTemp(taget, isGain) \
  (Sense_Update(&Humiture.Sense[0], taget, isGain))

//----------------------------ʪ��У׼����--------------------------------------
//0�궨�ɹ�������ʧ��!
#define Humiture_CalibrationRh(taget, isGain) \
  (Sense_Update(&Humiture.Sense[1], taget, isGain))

//------------------------------------�õ���ǰ�¶�------------------------------
#define Humiture_GetTemp() (Sense_GetVolInScope(&Humiture.Sense[0]))

//------------------------------------�õ���ǰʪ��------------------------------
#define Humiture_GetRh() (Sense_GetVolInScope(&Humiture.Sense[1]))

/*******************************************************************************
                           �ص�����
*******************************************************************************/




#endif






