/*****************************************************************************

		                    ��Flashҳ��ʵ�ֵ�Сʱ�ۼ�ģ��
��ģ����Ҫ����û��RTC������RAM��EEPROMʱ��ϵͳ����Ҫ��¼�豸����Сʱ��ص���Ϣ,
  ʹ��ʱ����Сʱ��(����unsigned long Ϊ��λ)������ֵ��Ϊʵ��ʱ��
��ģ��ʹ��һҳFlash(����ģ��ר��)�������Գ������������Ĺ���Сʱ��
���澫����1СʱΪ��λ����59��59��999����ػ�ʱ��Ҳ����Ϊ��0Сʱ
�ɽ���СʱУ������Tick��������ж��п���߾��ȣ�δ���¶ȶ�ʱ��Ӱ����в���

��ģ�������Ӳ��(ͨ��Flashģ��ʵ��)��Ӧ��
*****************************************************************************/
#ifndef _HOUR_INC_FLASH_H
#define	_HOUR_INC_FLASH_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif
/*****************************************************************************
		                      �������
*****************************************************************************/

//��ģ���ѡʱ������Ķ���
//#define SUPPORT_HOUR_INC_FLASH  //֧��Flashҳ��ʵ�ֵ�Сʱ��ʱʱ

//����ʱ��֧�ִ�ģ����Ϣ�ṹ�ָ�����
//������ʱ������Ϣ�ṹ�洢��ר�õ�ҳ��ǰ�����ֽ��ϣ����ú󣬸���EEPROM�洢
//����Ϣ������ʱ���߱Ƚϣ�����ȷ����Ϣ���лָ�
//#define SUPPORT_HOUR_INC_FLASH_RESTORE   //��δʵ�ִ˹���

//(��Ƭ������)дFlashʱ���������Сд��bit��λ
//���Flash��ͬ���е�����Bit������1д0(�粿������Flash)���е�����1bit,2bit,4bit. 
//�е�����2ByteΪ��λ,4ByteΪ��λ...�������1�������β����Ƿ��д���Ի���ʵ�ʾ���
//��ֵ����Ϊ��2^n������1��2��4��8(1Byte)��16(2Byte), 32(4Byte) 64(8Byte)...
#ifndef HOUR_INC_FLASH_WR_BCELL
  #define HOUR_INC_FLASH_WR_BCELL    8  
#endif

//��������Flashҳ�е���ʼλ�ã�4�ı�����BCELL����
#ifndef HOUR_INC_FLASH_PAGE_BASE
  #define HOUR_INC_FLASH_PAGE_BASE    (0x8000-0x600)
#endif

//����ר��Flashҳ��С������Ϊ4�ı�����BCELL����
#ifndef HOUR_INC_FLASH_PAGE_SIZE
  #define HOUR_INC_FLASH_PAGE_SIZE    512  
#endif

                            
//��дһ��Flashʱ�����С������Ϊ4�ı�����BCELL����
//ֵԽ�����Flash��д����ԽС��ռ��ջ�ռ�Խ��,����ʱ����
#ifndef HOUR_INC_FLASH_BUFFER_SIZE  
  #define HOUR_INC_FLASH_BUFFER_SIZE   (4 * 2)
#endif

//��ʱTICK���ڵ�����1Сʱʱ��������, Ĭ��1ms ����һ��(����tick�ж���)
//�����ֵ��=1ʱ����ʾ�ⲿ��RTC,1Сʱ����һ��ֱ�ӱ���,��״̬���ɱ궨
#ifndef HOUR_INC_FLASH_TO_HOUR_OV  
  #define HOUR_INC_FLASH_TO_HOUR_OV  (3600 * 1000 / 1)//Ĭ��1ms����һ��
#else //����Ϊʾ��,ע��У׼ʱ����Ϊ���ֵ����10s�����������Ϊ10s
//  #define HOUR_INC_FLASH_TO_HOUR_OV  (3600 * 1000 / 16))//16msһ��ʾ��
//  #define HOUR_INC_FLASH_TO_HOUR_OV  (3600)//1S��ʱ������1s��׼ʱ��ʱ 
//  #define HOUR_INC_FLASH_TO_HOUR_OV  (360)//10S��ʱ������10s��׼ʱ��ʱ 
//  #define HOUR_INC_FLASH_TO_HOUR_OV  1//1h��ʱ1h��ʾ������RTCģ��  
#endif

//����1h����Ĭ��ֵ(256TickΪ��λ )�������Сʱ����
#ifndef HOUR_INC_FLASH_TO_HOUR_DEFAULT
  #if (HOUR_INC_FLASH_TO_HOUR_OV > 36000)//0.1�����»��м���ʱ��ϸ��
    #define HOUR_INC_FLASH_TO_HOUR_DEFAULT  (HOUR_INC_FLASH_TO_HOUR_OV / 256)
  #else
    #define HOUR_INC_FLASH_TO_HOUR_DEFAULT   HOUR_INC_FLASH_TO_HOUR_OV
  #endif
#endif

//������Ƿ�ɱ궨,1h��ʾ������RTCģ�飬ֱ�ӵ��ã�ǿ�ƽ�ֹ�궨������ͬ��
#if HOUR_INC_FLASH_TO_HOUR_OV <= 1
  #ifdef SUPPORT_HOUR_INC_FLASH_CAL
    #undef SUPPORT_HOUR_INC_FLASH_CAL
  #endif
#endif

/***********************************************************************
		                      ��ؽṹ
***********************************************************************/


//��Ϣ�ṹ
struct _HourIncFlashInfo{
  //����1Сʱ��Task����ֵ,����У׼Сʱ����,������У׼ʱ��ֵ������INFO��
  unsigned short ToHourCount;    
};

//Flashͷ(֧�ֱ���ʱ��ͷ����ʱ����һ���洢��Info��)
struct _HourIncFlashHeader{
  unsigned short EreaseCount;     //����������һ��Ϊһ�ֻأ�����Ӧ����
  unsigned short EreaseCountAnti; //�����������룬����У��
};

//���ṹ
struct _HourIncFlash{
  struct _HourIncFlashInfo Info;
  //СʱFLASH����
  unsigned short EreaseCount; //����������һ��Ϊһ�ֻأ�����Ӧ����
  unsigned short InPageHour;  //һҳ�ھ�����Сʱ����ע���ֵ�λ�Ƿ��ޣ�
  //����Сʱ��
  unsigned short OnHour;      
  
  //Сʱ��RAM����:
  #if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1Сʱ���¼���ʱ
  unsigned short ToHourTimer; //��1Сʱʱ����ֵ  
  #endif  
  
  #ifdef SUPPORT_HOUR_INC_FLASH_CAL //����У׼ʱ
  unsigned short HourCalibrationOV;//1СʱУ׼ר��(��ToHourTimer����ǰ�����ʲ�����)
  #endif
  
  #if (HOUR_INC_FLASH_TO_HOUR_OV > 36000)//0.1�����»��м���ʱ,U16װ�����ˡ�
  unsigned char TickTimer;    //Tick����
  #endif
  
  unsigned char Flag;         //��ر�־��������
};

//��ر�־����Ϊ
#define HOUR_INC_FLASH_HEADER_ERR 0x80  //����ͷ����


extern struct _HourIncFlash HourIncFlash;          //ֱ��ʵ����

/*******************************************************************************
		                      �����Ϊ����
*******************************************************************************/

//------------------------------��ʼ������---------------------------------
void HourIncFlash_Init(unsigned char IsInited);

//------------------------------Tick������-------------------------------
//if(HOUR_INC_FLASH_TO_HOUR_OV > 1)ʱ����ã����������Ӧ��ʱ�������ڵ���
void HourIncFlash_TickTask(void);

//----------------------------128Tick������------------------------------
//if(HOUR_INC_FLASH_TO_HOUR_OV <= 1ʱ) 1Сʱ������һ�Σ���ֱ�ӱ���Сʱ
//if(HOUR_INC_FLASH_TO_HOUR_OV < 36000) ʱHourIncFlash_TickTask�����
//�������HOUR_INC_FLASH_TO_HOUR_OV * 128��ʱ�������ڵ���
signed char HourIncFlash_128TickTask(void);

//-------------------------�Ƿ��У׼����-------------------------------------
#ifdef SUPPORT_HOUR_INC_FLASH_CAL
  #define HourIncFlash_HaveCalibration() (HourIncFlash.HourCalibrationOV > 0)
#elif !defined(HourIncFlash_HaveCalibration) //�����ⲿʵ��
  #define HourIncFlash_HaveCalibration() (0)
#endif

//------------------------------У׼����-------------------------------------
//HourIncFlash_HaveCalibration()ʱ�ɵ���, Ĭ��һ��1Сʱ����
#ifdef SUPPORT_HOUR_INC_FLASH_CAL
void HourIncFlash_Calibration(unsigned short Sec); //��Ϊ��λ
#elif !defined(HourIncFlash_Calibration) //�����ⲿʵ��
  #define HourIncFlash_Calibration(sec) do{}while(0)
#endif

//----------------------------�õ�Сʱ���ֵ----------------------------
#ifdef SUPPORT_HOUR_INC_FLASH_CAL //����У׼ʱ
  #define HourIncFlash_GetHourCount() (HourIncFlash.Info.ToHourCount)
#else //������ʱ��ʹ���û�����ֵ
  #define HourIncFlash_GetHourCount() HOUR_INC_FLASH_TO_HOUR_DEFAULT
#endif

//---------------------------------ģ��ָ���0ʱ��---------------------------
void HourIncFlash_ResetToHour0(void);

/*******************************************************************************
		                      ������Ժ���
*******************************************************************************/

//-----------------------------�õ��ۼ�Сʱ��------------------------------
//AbsHourΪ����ľ���ʱ�䣬Ϊ0ʱ�ɻ�ô�ģ�鵱ǰСʱ��
//��ֵ��ʾ����ľ���ʱ��ȵ�ǰСʱ��Ҫ��
signed long HourIncFlash_GetAddHour(unsigned long AbsHour);

//----------------------------�õ��ۼ�Сʱ������------------------------------
#define HourIncFlash_GetDay(absHour)  (HourIncFlash_GetAddHour(absHour) / 24)

//----------------------------�õ��ۼ�Сʱ�е�Сʱ��---------------------------
#define HourIncFlash_GetHour(absHour)  (HourIncFlash_GetAddHour(absHour) % 24)

//----------------------------------�õ�Сʱ�е�����--------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1Сʱ���¼���ʱ
unsigned short HourIncFlash_GetSecInHour(void);
#endif

//---------------------------�õ�Сʱ�еķ�����-----------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1Сʱ���¼���ʱ
#define HourIncFlash_GetMinue()  (HourIncFlash_GetSecInHour() / 60)
#endif

//----------------------------------�õ����е�����--------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1Сʱ���¼���ʱ
#define HourIncFlash_GetSec()  (HourIncFlash_GetSecInHour() % 60)
#endif

//----------------------------------�õ�Tick����----------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 36000)//0.1�����»��м���ʱ,U16װ�����ˡ�
#define HourIncFlash_GetTick()   (HourIncFlash.TickTimer)
#endif

#endif
