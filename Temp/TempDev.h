/*******************************************************************************

                         �¶��豸ģ��
��ģ��֧��ͬһ�����Ŀ�ڣ�������;��������ԭʼ�¶��źţ�ת��Ϊ�¶�ֵ
��ģ��̶��¶ȵ�λΪ0.01�棬���ϸ�273.5���Ա���������¶�ֵ(����Ϊ3276.1��)
*******************************************************************************/
#ifndef __TEMP_DEV_H
#define __TEMP_DEV_H

/*****************************************************************************
                                ���˵���붨��
*****************************************************************************/

//��������Ϊ��ԭʼ�ź�->�����->X����->(��ѡ)����У��->(��ѡ)����У��
//֧�ֵ���У׼(У���)������У׼(У���������)

//֧�ֶ��ֲ�ͬ���˲���ʽ:
//#define SUPPORT_TEMP_DEV_FILETER   //֧���˲�ʱ
//#define SUPPORT_TEMP_DEV_FILETER_AVERVGE   //֧�ּ�ƽ��ʱ
//#define SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID  //֧��ƽ����ȥ�������Сֵʱ

//֧���ֽ��¶�ʱͬʱ������ʼ�¶ȣ�����Ϊ�棬���Ϊ��ֵ,��Χ��(-��ֵ) ~ (255-��ֵ)
//#define TEMP_DEV_BYTE_BASE           70

//�¶����̶�Ϊ2740�ȣ���0�ȷ�����С���¶ȼ���
#define TEMP_DEV_BASE           2740

//0Ϊ��Чֵ
#define TEMP_DEV_TEMP_INVALID   0       

/*****************************************************************************
                                ��ؽṹ
*****************************************************************************/
#include "NolinearConvert.h"   //����У��

//------------------------------------Desc�ṹ----------------------------------
//����ͬһ�����Ŀ��ͬ;��(��һ·18B20,һ·ģ�⴫����,һ·��MCU�ڲ�AD)������������ò���
//�����ṹֱ�ӹ̻���Flash��
struct _TempDev;
struct _TempDevDesc{
  unsigned char GainQ;                   //����ʹ�õ�Qֵ
  unsigned char LutAryCapability;        //���߲���������
  unsigned char FilterBufCount;          //�¶Ȳ��ȶ�ʱ�˲��������,0ʱ���˲�
  unsigned char FilterCfg;              //�˲������������������
  void (*InfoToDefault)(struct _TempDev *pDev);//���豸Info�ָ���������
  const struct _NolinearConvertTbl * pLutAry;//���߲���������, NULL�޴˹���
  unsigned short (*ExVolPro)(struct _TempDev *pDev, unsigned short Temp);//����У��,
  unsigned short (*AntiExVolPro)(struct _TempDev *pDev, unsigned short Temp);//������У�����궨��
  unsigned short CalibrationTempDiffMin; //�궨ʱ���������С�²�ֵ
  unsigned short CalibrationOrgSignalDiffMin; //�궨ʱ���������С�ź���
};

//FilterCfg����Ϊ:
#define TEMP_DEV_FILETER_TYPE_SHIFT   5 //�˲���ʽ,λ
#define TEMP_DEV_FILETER_PARA_MASK    0x1f //��ͬ�˲���ʽ��Ӧ�Ĳ���
//�˲���ʽ,����Ϊ
#define TEMP_DEV_FILETER_NONE         0  //���˲�
#define TEMP_DEV_FILETER_AVERVGE      1  //��ƽ��������Ϊ��ǰλ��
#define TEMP_DEV_FILETER_AVERVGE_MID  2  //ƽ����ȥ�������Сֵ������Ϊ���ඪ������

//-------------------------------------Info�ṹ---------------------------------
//�����EEPROM�洢
struct _TempDevInfo{
  signed short Zero;    //���,ԭʼ�ź�Ϊ��λ
  unsigned short Gain;  //����,ת����Ϊ�¶�
};

//���ṹ,�����EEPROM�洢
struct _TempDev{
  struct _TempDevInfo Info;   //��Ϣ
  #ifdef SUPPORT_TEMP_DEV_FILETER//֧���˲�ʱ
    unsigned short *pFilterBuf;//���������¶�ֵ������ΪpDesc->FilterBufCount
    unsigned short FilterPara;  //��ͬ�˲���ʽ��Ӧ����ز���  
  #endif
  
  unsigned short CurTemp;    //��ǰ��׼�¶�ֵ
  //����У׼���:
  unsigned short  PrvAdjTemp;    //�ϴα�Уʱ��Ŀ���¶�ֵ
  unsigned short  PrvAdjSignal;  //�ϴα�Уʱ��ԭʼ�ź�ֵ
  //����
  unsigned char Ch;           //�ڲ������ͨ��ID
  unsigned char Flag;         //��ر�־,������
};

//��ر�־����Ϊ��
#define TEMP_DEV_ADJ_MODE        0x80  //�¶������Уģʽ,����Ϊ���궨ģʽ
#define TEMP_DEV_ADJ_1ST_FINAL   0x20  //�����Уģʽʱ���¶�����¼���
#define TEMP_DEV_UPDATE_LATER    0x10  //�����Ӻ��־

/*******************************************************************************
                           �����Ϊ����
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void TempDev_Init(struct _TempDev *pDev,
                  unsigned char IsInited, //�Ƿ��ѳ�ʼ��
                  unsigned char Ch);      //�����ͨ��

//---------------------------------�����¶�ֵ-----------------------------------
//���ڵ����Ը����¶�,�����ڷ��ж������ڵ���
void TempDev_Update(struct _TempDev *pDev,
                    unsigned short OrgSignal); //ԭʼ�ź�(Ϊ��ʱ���ϸ��̶�ֵΪ��)

//---------------------------�Ժ��������-------------------------------
//����TempDev_Update()�������Ľ����е���(����32ms����)
#ifdef SUPPORT_TEMP_DEV_FILETER //֧���˲�ʱ
  void TempDev_UpdateLater(struct _TempDev *pDev);
#else
  #define TempDev_UpdateLater(dev) do{}while(0)
#endif

//----------------------------�¶�У׼����--------------------------------------
//�β�Ϊ��ǰʱ�̵�Ŀ���¶�ֵ
//���У����,��������ε��ô˺����Ľ��Ϊ׼
void TempDev_Calibration(struct _TempDev *pDev,
                         unsigned short Temp); //Ŀ��Ũ��ֵ

//--------------------------֧���ֽ��¶ȱ�ʾʱ��ת------------------------------
//�����ڴ洢ʱ��ʡ�ռ�
#ifdef TEMP_DEV_BYTE_BASE
  unsigned char TempDev_ToByte(unsigned short Temp);
  unsigned short TempDev_FromByte(unsigned char ByteTemp);
#endif

/*******************************************************************************
                           ������Ժ���
*******************************************************************************/
//------------------------------------�õ���ǰ�¶�------------------------------
#define TempDev_GetTemp(dev) ((dev)->CurTemp)

//------------------------------��Уģʽ���------------------------------------
//�Ƿ��ʶ������궨ģʽ
#define TempDev_IsAdj2Mode(dev) ((dev)->Flag &  TEMP_DEV_ADJ_MODE) 
//�Ƿ��ʶ��1��궨ģʽ
#define TempDev_IsAdj1Mode(dev) (!((dev)->Flag & TEMP_DEV_ADJ_MODE))
//�л���1��궨ģʽ
#define TempDev_SetAdj1Mode(dev) do{(dev)->Flag &= ~TEMP_DEV_ADJ_MODE;}while(0)

//�л�������궨ģʽ
#define TempDev_SetAdj2Mode(dev) do{(dev)->Flag |= TEMP_DEV_ADJ_MODE;}while(0)
//����궨ģʽʱ������ϴε�
#define TempDev_ClrAdj2PrvPoint(dev) do{(dev)->Flag &= ~TEMP_DEV_ADJ_1ST_FINAL;}while(0)
//�л�������궨ģʽͬʱ����ϴε�
#define TempDev_SetAdj2Mode_ClrPrv(dev) do{TempDev_SetAdj2Mode(dev); \
  TempDev_ClrAdj2PrvPoint(dev);}while(0)

/*******************************************************************************
                           �ص�����
*******************************************************************************/

//------------------------�ɶ����ͨ��ID�õ�Desc�ṹ----------------------------
//����ʵ���Ҳ���ΪNULL
const struct _TempDevDesc *TempDev_cbpGetDesc(unsigned char Ch);

#endif






