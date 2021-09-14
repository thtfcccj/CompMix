/*******************************************************************************

                         �¶�(������)ģ��
��ģ��̶��¶ȵ�λΪ1�棬���ϸ�55���Ա���������¶�ֵ(����Ϊ200��)
*******************************************************************************/
#ifndef __TEMP_H
#define __TEMP_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/*****************************************************************************
                             �������
*****************************************************************************/

//�����Ƿ�֧���²�����
//#define SUPPORT_TEMP_LUT     

//��������Ϊ��ԭʼ�ź�->�����->X����->(��ѡ)����У��->(��ѡ)����У��
//֧�ֵ���У׼(У���)������У׼(У���������)

//�¶����̶�Ϊ55�ȣ���0�ȷ�����С���¶ȼ���
#define TEMP_BASE    64   

/*****************************************************************************
                             ��ؽṹ
*****************************************************************************/
//---------------------------Ӳ��֧��-------------------------------------
#if defined(SUPPORT_HC32)
 #include "Temp_HC32.h"
#elif defined(SUPPORT_NTC10K_3380K)
 #include "Temp_NTC10K_3380K.h"
#endif

//-------------------------------------Info�ṹ---------------------------------
//�����EEPROM�洢
struct _TempInfo{
  signed short Zero;    //���,ԭʼ�ź�Ϊ��λ
  unsigned short Gain;  //����,ת����Ϊ�¶�
};

//���ṹ,�����EEPROM�洢
struct _Temp{
  struct _TempInfo Info;   //��Ϣ
  unsigned short  PrvAdjSignal;  //�ϴα�Уʱ��ԭʼ�ź�ֵ 
  unsigned char  PrvAdjTemp;    //�ϴα�Уʱ��Ŀ���¶�ֵ  
  
  #ifdef SUPPORT_TEMP_FILETER//֧���˲�ʱ
    #ifdef SUPPORT_TEMP_FILETER_AVERVGE_MID
      unsigned char FilterBuf[SUPPORT_TEMP_FILETER * 2];//���������¶�ֵ��λֵ
    #else
      unsigned char FilterBuf[SUPPORT_TEMP_FILETER ];//���������¶�ֵ
    #endif
    unsigned char FilterPos;  //�˲��洢λ��  
  #endif
    
  unsigned char CurTemp;    //��ǰ��׼�¶�ֵ
  unsigned char Flag;         //��ر�־,������
};

//��ر�־����Ϊ��
#define TEMP_ADJ_1ST_FINAL   0x20  //�����Уģʽʱ���¶�����¼���
#define TEMP_UPDATE_LATER    0x10  //�����Ӻ��־
#define TEMP_LOW_OV_ERR      0x20  //�����ޱ�־(�п�����δ�궨)
#define TEMP_HI_OV_ERR       0x40  //�����ޱ�־(�п�����δ�궨)

extern struct _Temp Temp;
/*******************************************************************************
                           �����Ϊ����
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void Temp_Init(unsigned char IsInited); //�Ƿ��ѳ�ʼ��

//---------------------------------�����¶�ֵ-----------------------------------
//���ڵ����Ը����¶�,�����ڷ��ж������ڵ���
void Temp_Update(void); //ԭʼ�ź�(Ϊ��ʱ���ϸ��̶�ֵΪ��)

//---------------------------�Ժ��������-------------------------------
//����Temp_Update()�������Ľ����е���(����32ms����)
#ifdef SUPPORT_TEMP_FILETER //֧���˲�ʱ
  void Temp_UpdateLater(void);
#else
  #define Temp_UpdateLater() do{}while(0)
#endif

//----------------------------�¶�У׼����--------------------------------------
//0�궨�ɹ�������ʧ��!
signed char Temp_Calibration(unsigned char TargetTemp, //Ŀ��Ũ��ֵ
                              unsigned char IsGain);//������(����ǰ�����)����������

/*******************************************************************************
                           ������Ժ���
*******************************************************************************/

//--------------------------------��ǰ�¶��Ƿ��쳣------------------------------
#define Temp_IsInvalid() (Temp.Flag & (TEMP_LOW_OV_ERR | TEMP_HI_OV_ERR))

//------------------------------------�õ���ǰ�¶�------------------------------
#define Temp_GetTemp() (Temp.CurTemp)

//--------------------------����궨ʱ���ϴε����------------------------------
//����ϴε�
#define Temp_ClrPrvPoint() \
  do{Temp.Flag &= ~TEMP_ADJ_1ST_FINAL;}while(0)
//�Ƿ��ϴ��е�
#define Temp_IsPrvPoint() (Temp.Flag & TEMP_ADJ_1ST_FINAL)
    
/*******************************************************************************
                           �ص�����
*******************************************************************************/

//---------------------------�õ���ǰ�¶�ֵ��Ӧ�ź�-----------------------------
unsigned short Temp_cbGetCurTempSignal(void);

//------------------------------------�¶�У��--------------------------------
//�磺���Զ�У�����������Ӵ�����
#ifdef SUPPORT_TEMP_LUT
  unsigned char Temp_cbAppendPro(unsigned char OrgTemp);
#else//��֧��ʱ
  #define Temp_cbAppendPro(temp) (temp)  
#endif

//----------------------------------�¶ȷ�У��---------------------------------
//��Temp_cbAppendPro()�ɶ�ʹ��
#ifdef SUPPORT_TEMP_LUT
  unsigned char Temp_cbAppendPro(unsigned char CurTemp);
#else//��֧��ʱ
  #define Temp_cbAntiAppendPro(temp) (temp)
#endif

#endif





