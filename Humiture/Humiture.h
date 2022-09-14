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

//�¶����ֵ�����ڴ�ֵΪ��ֵ
#ifndef HUMITURE_TEMP_MAX
  #define HUMITURE_TEMP_MAX   600  
#endif
//�¶���Сֵ�����ڴ�ֵΪ��ֵ
#ifndef HUMITURE_TEMP_MIN
  #define HUMITURE_TEMP_MIN   -200  
#endif

//ʪ�����ֵ�����ڴ�ֵΪ��ֵ
#ifndef HUMITURE_RH_MAX
  #define HUMITURE_RH_MAX   100  
#endif
//ʪ����Сֵ�����ڴ�ֵΪ��ֵ
#ifndef HUMITURE_RH_MIN
  #define HUMITURE_RH_MIN   0  
#endif

//���ϼ��������Ӵ��󳬹���ֵ�α�����
#ifndef HUMITURE_ERR_OV
  #define HUMITURE_ERR_OV   10 
#endif

//��ʪ��Qֵ
#define HUMITURE_TEMP_GAIN_Q  10
#define HUMITURE_RH_GAIN_Q    10

/*****************************************************************************
                             ��ؽṹ
*****************************************************************************/
#include "Sense.h"

//���ṹ
struct _HumitureInfo{
  signed short   HiTemp;       //�߱��¶ȵ�
  signed short   LowTemp;      //�ͱ��¶ȵ�
  signed char    HiRH;         //�߱�ʪ�ȵ�
  signed char    LowRH;        //�ͱ�ʪ�ȵ�  
};

//���ṹ
struct _Humiture{
  struct _HumitureInfo Info;
  unsigned char ErrCount; //���ϼ�����  
  //�ź�(������,��λ�����)
  struct _Sense Sense[2]; //�������ź�
  signed short FilterBuf[2][HUMITURE_FILTER_BUF_COUNT];//�˲��㷨������
  //ע�����������û��Իص���ʽʵ�֡�
};

extern struct _Humiture Humiture;

/*******************************************************************************
                           ��غ���
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void Humiture_Init(unsigned char IsInited); //�Ƿ��ѳ�ʼ��

//------------------------------׼���ڼ��ж�-------------------------------------
unsigned char Humiture_IsRdy(void);

//------------------------------���¹��ϼ���-------------------------------------
//ͨѶ��ɺ����,����
void Humiture_UpdateErr(signed char IsErr); 

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
signed short Humiture_GetTemp(void);

//------------------------------------�õ���ǰʪ��------------------------------
signed short Humiture_GetRh(void);

//---------------------------------�õ�����״̬--------------------------------
unsigned char Humiture_GetErr(signed char IsRh);

/*******************************************************************************
                           �ص�����
*******************************************************************************/

//--------------------------------Ĭ��ֵ----------------------------------
extern const struct _HumitureInfo HumitureInfo_cbDefault;

//-----------------------------�õ�С����λ��-------------------------------
//ͨѶ��
unsigned char Humiture_GetDotPos(signed char IsRh);

//-----------------------------�õ�������ʽ-------------------------------
//ͨѶ��
unsigned char Humiture_cbGetAlarmMode(signed char IsRh);

//-----------------------------�õ��¶ȱ���״̬---------------------------
//���û�ʵ��,����0��ȷ��1�ͱ���2�߱�
unsigned char Humiture_cbGetAlarm(signed char IsRh);

//-----------------------------ģ�鸴λ-------------------------------
//ͨѶ��
void Humiture_cbReset(void);

//-----------------------------�Ƿ�Ϊ�����̲���״̬-----------------------
//�����ⲿʹ��,��0ʱ������ȫ������+������Ũ�ȡ�
unsigned char Humiture_cbIsFullState(void);


#endif






