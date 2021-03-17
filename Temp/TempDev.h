/*******************************************************************************

                         �¶��豸ģ��
��ģ��֧��ͬһ�����Ŀ�ڣ�������;��������ԭʼ�¶��źţ�ת��Ϊ�¶�ֵ
��ģ��̶��¶ȵ�λΪ0.01�棬���ϸ�273.5���Ա���������¶�ֵ(������Ϊ65261.0��)
*******************************************************************************/
#ifndef __TEMP_DEV_H
#define __TEMP_DEV_H

/*****************************************************************************
                                ���˵���붨��
*****************************************************************************/
//��������Ϊ��ԭʼ�ź�->�����->X����->(��ѡ)����У��->(��ѡ)����У��
//֧�ֵ���У׼(У���)������У׼(У���������)
//֧�ֶ��ֲ�ͬ���˲���ʽ

#define TEMP_DEV_BASE   2735    //�¶����̶�Ϊ273.5��

/*****************************************************************************
                                ��غ���
*****************************************************************************/
#include "NolinearConvert.h"   //����У��

//------------------------------------Desc�ṹ----------------------------------
//����ͬһ�����Ŀ��ͬ;��(��һ·18B20,һ·AD,һ·��MCU��)������������ò���
//�����ṹֱ�ӹ̻���Flash��
struct _TempDev;
struct _TempDevDesc{
  unsigned char GainQ;                   //����ʹ�õ�Qֵ
  unsigned char LutAryCapability;        //���߲���������
  unsigned char FilterBufCount;          //�¶Ȳ��ȶ�ʱ�˲��������,0ʱ���˲�
  unsigned char FilterCfg;              //�˲������������������
  void (*InfoToDefault)(struct _TempDev);//���豸Info�ָ���������
  const struct _NolinearConvertTbl * const *ppLutAry;//���߲���������, NULL�޴˹���
  unsigned short (*ExVolPro)(struct _TempDev *pDev, unsigned short Temp);//����У��
  unsigned short (*AntiExVolPro)(struct _TempDev *pDev, unsigned short Temp);//������У�����궨��
};

//FilterCfg����Ϊ:
#define TEMP_DEV_FILETER_TYPE_SHIFT   5 //�˲���ʽ,λ
#define TEMP_DEV_FILETER_PARA_MASK    0x1f //��ͬ�˲���ʽ��Ӧ�Ĳ���
//�˲���ʽ,����Ϊ
#define TEMP_DEV_FILETER_AVERVGE      0  //��ƽ�����޲���
#define TEMP_DEV_FILETER_AVERVGE_MID  1  //��ƽ����ȥ�������Сֵ������Ϊ���ඪ������

//-------------------------------------Info�ṹ---------------------------------
//�����EEPROM�洢
struct _TempDevInfo{
  signed short Zero;    //���,ԭʼ�ź�Ϊ��λ
  unsigned short Gain;  //����,ת����Ϊ�¶�
};

//���ṹ,�����EEPROM�洢
struct _TempDev{
  struct _TempDevInfo Info;   //��Ϣ
  unsigned short *pFilterBuf;//���������¶�ֵ������ΪpDesc->FilterBufCount
  unsigned short CurTemp;    //��ǰ��׼�¶�ֵ
  //����У׼���:
  unsigned short  AdjTemp;     //��ǰ��Уʱ��Ŀ���¶�ֵ  
  unsigned short  PrvAdjTemp;  //�ϴα�Уʱ��Ŀ���¶�ֵ
  unsigned short  PrvAdjOrg;    //�ϴα�Уʱ��ԭʼ�ź�ֵ
  //����
  unsigned short FilterPara;  //��ͬ�˲���ʽ��Ӧ����ز���
  unsigned char Ch;           //�ڲ������ͨ��ID
  unsigned char Flag;         //��ر�־,������
};

//��ر�־����Ϊ��
#define TEMP_DEV_ADJ_MODE        0x80  //�¶������Уģʽ,����Ϊ���궨ģʽ
#define TEMP_DEV_ADJ_DOING       0x40  //�¶ȱ�У������,�û����ñ�У����ʱʹ��
#define TEMP_DEV_ADJ_CLR_FINAL   0x20  //�¶����궨���
#define TEMP_DEV_TASK_MASK       0x0F  //���λ�̶��������ڼ���,256ms��λ

/*******************************************************************************
                           �����Ϊ����
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void TempDev_Init(struct _TempDev *pDev,
                  unsigned char IsInited, //�Ƿ��ѳ�ʼ��
                  unsigned char Ch);      //�����ͨ��

//---------------------------------�����¶�ֵ-----------------------------------
//ͬ�ڵ����Ը����¶�,�����ڷ��ж������ڵ���
void TempDev_Update(struct _TempDev *pDev,
                    unsigned short OrgSignal); //ԭʼ�ź�(Ϊ��ʱ���ϸ��̶�ֵΪ��)

//----------------------------�¶�У׼����--------------------------------------
//�β�Ϊ��ǰʱ�̵�Ŀ���¶�ֵ
//���У����,��������ε��ô˺����Ľ��Ϊ׼
void SenTemp_Calibration(struct _TempDev *pDev,
                         unsigned short Temp); //Ŀ��Ũ��ֵ

/*******************************************************************************
                           ������Ժ���
*******************************************************************************/
//------------------------------------�õ���ǰ�¶�------------------------------
#define TempDev_GetTemp(dev) ((dev)->CurTemp)

//------------------------------��Уģʽ���------------------------------------
//�Ƿ��ʶ������궨ģʽ
#define SenTemp_IsDispAdjMode(dev) \
  (((dev)->Flag & (TEMP_DEV_ADJ_MODE | 0x04)) == TEMP_DEV_ADJ_MODE)
    
//�����Уģʽ,����Ϊ���궨ģʽ
#define SenTemp_IsAdjMode(dev) ((dev)->Flag & TEMP_DEV_ADJ_MODE)
//���궨ģʽ
#define SenTemp_SetAdjZeroMode(dev) do{(dev)->Flag &= ~TEMP_DEV_ADJ_MODE;}while(0)
//����궨ģʽ
#define SenTemp_SetAdjGainMode(dev) do{(dev)->Flag |= TEMP_DEV_ADJ_MODE;}while(0)

/*******************************************************************************
                           �ص�����
*******************************************************************************/

//------------------------�ɶ����ͨ��ID�õ�Desc�ṹ----------------------------
const struct _TempDevDesc *TempDev_cbpGetDesc(unsigned char Ch);

#endif






