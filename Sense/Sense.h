/*******************************************************************************

                     ��֪���źŴ���
//��ģ�� �������ź�ʵʱת��Ϊʵ���ź�
//��ģ��רΪ���������̶��ļ򵥴�������ƣ�
//  �̶�λ�ã���������ID�Ź̶�  
//  �̶��������ʣ� �����������͹̶�
//  �̶��̶�����:  ��������������Χ�̶�
//  �̶���λ: ��ֻ����һ�ֵ�λ(�����϶��뻪�϶���̶�)
//  ����ʾ��Χ+=-32768, �����м��С����
//  ԭʼ�ź�һֱΪ������(��Ϊ�½��������ǰ���򼴿�ת��Ϊ����)
//  ��������������Χ���־��⣬��������ĳ���������������
//֧�ֲ�ͬ�˲��㷨���Լ�������У��
//֧�ֵ���궨(����㼴x��ƫ��)������궨(�����漴б��)
//��ģ����ҪΪ��������ƣ���������ֵ��أ��������乤��״̬�����ϣ������ȹ��ܡ�
*******************************************************************************/
#ifndef __SENSE_H
#define __SENSE_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/*****************************************************************************
                             �������
*****************************************************************************/

//�Ƿ�֧�ִ�ģ�飬�ⲿ��ѡʱ����
//#define SUPPORT_SENSE  

/*****************************************************************************
                             ��ؽṹ
*****************************************************************************/

//��̬�����ṹ
struct _SenseDesc{
  //�����Ϣ
  unsigned long InfoBase;  //struct _SenseInfo��EEPROM�е���ʼλ��
  unsigned char Id;       //Ϊ�˽ṹ�����ID��ʶ
  unsigned char GainQ;    //����ʹ�õ�Qֵ 
  unsigned char Dot;      //������С����λ��
  unsigned char UnitId;   //�������ʵĵ�λID 
  unsigned short KindId;  //�������ʵ�����ID
  
  signed short Min;   //���̷�Χ��Сֵ,������ֵ���жϹ���
  signed short Max;   //���̷�Χ���ֵ,������ֵ���жϹ���
  signed short DefaultZero;    //Ĭ�����ԭʼֵ,���ڳ�ʼ��
  signed short DefaultGain;    //Ĭ������ֵ,���ڳ�ʼ��
  signed short DefaultZeroVol; //Ĭ�����(��һ��)Ũ��ֵ,���ڱ궨
  signed short DefaultAdjVol;  //����궨ʱ��Ĭ�ϵڶ���Ũ��ֵ
  unsigned short DiffAdjVol;   //����궨ʱ����2�����1��������Сֵ��С�ڴ�ֵ��ֹ�궨  
  
  //֧�ַ���������ʱ,�ɼ������ֵ���õ�ת�����ֵ
  signed short (*NlLut)(signed short OrgVol);
  //֧�ַ���������ʱ,��ת�����ֵ,��ת��ǰ��ֵ����Ҫ����У׼��
  signed short (*AntiNlLut)(signed short OrgVol);
};

//Info�ṹ, �����EEPROM�洢
struct _SenseInfo{
  signed short   Zero;  //���,ԭʼ�ź�Ϊ��λ
  unsigned short Gain;  //����,ת����Ϊֱʵ�¶�
};

//���ṹ
struct _Sense{
  struct _SenseInfo Info;         //��Ϣ
  const struct _SenseDesc *pDesc;//��̬�����ṹ
  signed short PrvSignal;     //�ϴα�Уʱ��ԭʼ�ź�ֵ 
  signed short PrvVol;        //�ϴα�Уʱ��Ŀ��Ŀ��ֵ  

  //�˲����弰�㷨
  signed short *pFilterBuf;    //֧���˲�ʱ,�˲�������
  unsigned short FilterSize;     //֧���˲�ʱ,�˲�����
  unsigned short FilterPara;     //��ͬ�˲��㷨ʱ������Ĳ���
  unsigned short FilterNewPos;   //���������ڻ����е�λ��
  unsigned char FilterType;      //�˲��㷨����,������
  
  unsigned char Flag;            //��ر�־,������
  signed short  Vol;              //������Ľ��
};

//�˲��㷨����Ϊ��
#define SENSE_FILTER_TYPE_NONE     0  //���˲�
#define SENSE_FILTER_TYPE_AVERVGE  1  //���FilterSize��ƽ���˲�
#define SENSE_FILTER_TYPE_AVG_MID  2  //ȥ��ǰ��FilterPara�����ݵ�ƽ���˲�(��δ֧��)
#define SENSE_FILTER_TYPE_AVG_PEAK 3  //ȥ�������е������Сֵ���ƽ���˲�(��δ֧��)
#define SENSE_FILTER_TYPE_COUNT    4  //֧��4��

//��ر�־����Ϊ��
#define SENSE_ADJ_1ST_FINAL   0x10  //�����Уʱ�������ɱ�־
#define SENSE_VOL_VALID       0x20  //�����־
#define SENSE_LOW_OV_ERR      0x40  //�����ޱ�־,������<-32768��
#define SENSE_HI_OV_ERR       0x80  //�����ޱ�־,������>+32767��

/*******************************************************************************
                           ��غ���
*******************************************************************************/

//------------------------------��ʼ��-------------------------------------
//��ʼ��Ϊ���˲��㷨
void Sense_Init(struct _Sense *pSense,
                const struct _SenseDesc *pDesc,//��̬�����ṹ
                unsigned char IsInited); //�Ƿ��ѳ�ʼ��
                
//------------------------------�����˲��㷨--------------------------------
//���ڳ�ʼ�������
void Sense_UpdateFilter(struct _Sense *pSense,
                        unsigned char FilterType, //�˲��㷨����,������
                        signed short *pFilterBuf, //�˲�������
                        unsigned short FilterSize,//�˲��������
                        unsigned short FilterPara);//��ͬ�˲��㷨ʱ������Ĳ���
          
//-----------------------------����Ŀ��ֵ-----------------------------------
//���ڵ����Ը����¶�,
//��������ȥ��ֵ����Ҫ������˲��㷨ʱ��Ӧ�ڷ��ж��е���
void Sense_Update(struct _Sense *pSense,
                  signed short OrgSignal); //ԭʼ�ź�(������)

//----------------------------У׼����--------------------------------------
//0�궨�ɹ�������ʧ��!
signed char Sense_Calibration(struct _Sense *pSense,
                              signed short TargetVol, //Ŀ��Ũ��ֵ
                              unsigned char IsGain);//������(����ǰ�����)����������

//--------------------------------��ǰֵ�Ƿ��쳣------------------------------
#define Sense_IsInvalid(sense) ((sense)->Flag & (SENSE_LOW_OV_ERR | SENSE_HI_OV_ERR))

//--------------------------------�õ��ڷ�ΧĿ��ֵ------------------------------
//������Sense_IsInvalid()����ô˺���                        
signed short Sense_GetVolInScope(const struct _Sense *pSense);             
                              
//------------------------------------�õ���ǰֵ------------------------------
#define Sense_GetSense(sense) ((sense)->Vol)

//--------------------------����궨ʱ���ϴε����----------------------------
//����ϴε�
#define Sense_ClrPrvPoint(sense) do{(sense)->Flag &= ~SENSE_ADJ_1ST_FINAL;}while(0)
//�Ƿ��ϴ��е�
#define Sense_IsPrvPoint(sense) ((sense)->Flag & SENSE_ADJ_1ST_FINAL)

#endif






