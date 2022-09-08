/*******************************************************************************

                         �¶��豸ģ��-��DH11�ϵ�ʵ��
��ʵ��Ӳ����أ��¶Ⱦ���1��
*******************************************************************************/
#include "Humiture.h"
#include <string.h>

#include "Eeprom.h"
#include "InfoBase.h"
#include "DH11.h"

struct _Humiture Humiture;

/*******************************************************************************
                           ��غ���ʵ��
*******************************************************************************/
//�¶ȵľ�̬�����ṹ
struct _SenseDesc TempDesc = {
  //�����Ϣ
  Temp_GetInfoBase(),  //struct _SenseInfo��EEPROM�е���ʼλ��
  0,       //Ϊ�˽ṹ�����ID��ʶ
  HUMITURE_TEMP_GAIN_Q,      //����ʹ�õ�Qֵ 
  1,      //������С����λ��
  HUMITURE_TEMP_KIND_ID,      //�������ʵĵ�λID 
  HUMITURE_TEMP_UNIT_ID,       //�������ʵ�����ID
  
  0,       //���̷�Χ��Сֵ,������ֵ���жϹ���
  60,      //���̷�Χ���ֵ,������ֵ���жϹ���
  0,       //Ĭ�����ԭʼֵ,���ڳ�ʼ��
  1024,    //Ĭ������ֵ,���ڳ�ʼ��
  0,       //Ĭ�����(��һ��)Ũ��ֵ,���ڱ궨
  40,      //����궨ʱ��Ĭ�ϵڶ���Ũ��ֵ 
  
  NULL,//֧�ַ���������ʱ,�ɼ������ֵ���õ�ת�����ֵ
  NULL,//֧�ַ���������ʱ,��ת�����ֵ,��ת��ǰ��ֵ����Ҫ����У׼��
};

//ʪ�ȵľ�̬�����ṹ
struct _SenseDesc RhDesc = {
  //�����Ϣ
  Rh_GetInfoBase(),  //struct _SenseInfo��EEPROM�е���ʼλ��
  1,     //Ϊ�˽ṹ�����ID��ʶ
  HUMITURE_RH_GAIN_Q,    //����ʹ�õ�Qֵ 
  0,     //������С����λ��
  HUMITURE_RH_KIND_ID,      //�������ʵĵ�λID 
  HUMITURE_RH_UNIT_ID,       //�������ʵ�����ID
  
  0,      //���̷�Χ��Сֵ,������ֵ���жϹ���
  100,   //���̷�Χ���ֵ,������ֵ���жϹ���
  50,    //Ĭ�����ԭʼֵ,���ڳ�ʼ��
  1024,  //Ĭ������ֵ,���ڳ�ʼ��
  50,    //Ĭ�����(��һ��)Ũ��ֵ,���ڱ궨
  80,    //����궨ʱ��Ĭ�ϵڶ���Ũ��ֵ 
  
  NULL,//֧�ַ���������ʱ,�ɼ������ֵ���õ�ת�����ֵ
  NULL,//֧�ַ���������ʱ,��ת�����ֵ,��ת��ǰ��ֵ����Ҫ����У׼��
};


//------------------------------��ʼ������-------------------------------------
void Humiture_Init(unsigned char IsInited) //�Ƿ��ѳ�ʼ��
{
  //memset(&Humiture, 0, sizeof(struct _Humiture));
  //��ʼ������
  if(!IsInited){//װ��Ĭ��
    memcpy(&Humiture.Info, 
           &HumitureInfo_cbDefault, 
           sizeof(struct _HumitureInfo));
    Eeprom_Wr(Humiture_GetInfoBase(),
              &Humiture.Info,
              sizeof(struct _HumitureInfo));
  }
  else{
    Eeprom_Rd(Humiture_GetInfoBase(),
              &Humiture.Info,
              sizeof(struct _HumitureInfo));
  }
  
  //�¶ȳ�ʼ��
  Sense_Init(&Humiture.Sense[0], &TempDesc, IsInited);  
  Sense_UpdateFilter(&Humiture.Sense[0],
                     SENSE_FILTER_TYPE_AVERVGE,//�㷨
                     Humiture.FilterBuf[0],//�˲�������
                     HUMITURE_FILTER_BUF_COUNT,//�˲��������
                     0);//��ͬ�˲��㷨ʱ������Ĳ���
  //ʪ�ȳ�ʼ��  
  Sense_Init(&Humiture.Sense[1], &RhDesc, IsInited);
  Sense_UpdateFilter(&Humiture.Sense[1],
                     SENSE_FILTER_TYPE_AVERVGE,//�㷨
                     Humiture.FilterBuf[1],//�˲�������
                     HUMITURE_FILTER_BUF_COUNT,//�˲��������
                     0);//��ͬ�˲��㷨ʱ������Ĳ���
}

//---------------------------------�õ�����״̬--------------------------------
unsigned char Humiture_GetErr(signed char IsRh)
{
  return Dh11_GetLastErr();
}




