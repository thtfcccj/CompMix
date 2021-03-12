/*****************************************************************************

		                    ��Flashҳ��ʵ�ֵ�Сʱ�ۼ�ģ��
��ģ��ʹ��һҳFlash(����ģ��ר��)�������Գ������������Ĺ���Сʱ��
��ģ����Ҫ����û��RTC������RAM��EEPROMʱ��ϵͳ����Ҫ��¼�豸����Сʱ��ص���Ϣ��
��ģ�������Ӳ��(ͨ��Flashģ��ʵ��)��Ӧ��
*****************************************************************************/
#ifndef _HOUR_INC_FLASH_H
#define	_HOUR_INC_FLASH_H

/*****************************************************************************
		                      �������
*****************************************************************************/

//����ʱ��֧�ִ�ģ����Ϣ�ṹ�ָ�����
//������ʱ������Ϣ�ṹ�洢��ר�õ�ҳ��ǰ�����ֽ��ϣ����ú󣬸���EEPROM�洢
//����Ϣ������ʱ���߱Ƚϣ�����ȷ����Ϣ���лָ�
//#define SUPPORT_HOUR_INC_FLASH_RESTORE   //��δʵ�ִ˹���

//(��Ƭ������)дFlashʱ���������Сд��bit��λ
//���Flash��ͬ���е�����
//1Bit������1д0(�粿������Flash)���е�����2bit, �еĽ�����1ByteΪ��λ
//�е�����2ByteΪ��λ,4ByteΪ��λ
//ֵӦΪ��1��2��4��8(1Byte)��16(2Byte), 32(4Byte)  ->��ֻ֧���ֽ�Ϊ��λ
#ifndef HOUR_INC_FLASH_WR_BCELL
  #define HOUR_INC_FLASH_WR_BCELL    8  //Ĭ���ֽ�Ϊ��λд��
#endif

//��������Flashҳ�е���ʼλ��
#ifndef HOUR_INC_FLASH_PAGE_BASE
  #define HOUR_INC_FLASH_PAGE_BASE    (0x8000-0x800)
#endif

//����ר��Flashҳ��С
#ifndef HOUR_INC_FLASH_PAGE_SIZE
  #define HOUR_INC_FLASH_PAGE_SIZE    512  
#endif

//����1h����Ĭ��ֵ(256TickΪ��λ )�������Сʱ����
#ifndef HOUR_INC_FLASH_TO_HOUR_DEFAULT  
  #define HOUR_INC_FLASH_TO_HOUR_DEFAULT   14062  //�����ʱΪ14062    
#endif
                            
//��дһ��Flashʱ�����С��4Ϊ��λ
//ֵԽ�����Flash��д����ԽС��ռ��ջ�ռ�Խ��,����ʱ����
#ifndef HOUR_INC_FLASH_BUFFER_SIZE  
  #define HOUR_INC_FLASH_BUFFER_SIZE   (4 * 2)
#endif

/***********************************************************************
		                      ��ؽṹ
***********************************************************************/

//��Ϣ�ṹ
struct _HourIncFlashInfo{
  unsigned short ToHourCount;    //����1Сʱ��Task����ֵ,����У׼Сʱ����
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
  unsigned short WrPos;       //Flashдλ��(ÿ��λ��1Сʱ,��ͷ����)
  
  //Сʱ��RAM����:
  unsigned short ToHourTimer; //��1Сʱʱ����ֵ
  unsigned short HourCalibrationOV;//1СʱУ׼ר��(��ToHourTimer����ǰ�����ʲ�����)
  unsigned char TickTimer;    //Tick����
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
//ÿTick(>= 0.5ms)����һ�Σ��������ʱ���ж���
void HourIncFlash_TickTask(void);

//----------------------------128Tick������------------------------------
//����128Tick ,���ڱ���Flash
void HourIncFlash_128TickTask(void);

//---------------------------------СʱУ׼----------------------------------
//�Կ������ϴ�У׼��1Сʱʱ�����ô˺���У׼Сʱ
void HourIncFlash_HourCalibration(void);

/*******************************************************************************
		                      ������Ժ���
*******************************************************************************/

//-----------------------------�õ��ۼ�Сʱ��-----------------------------
unsigned long  HourIncFlash_GetAddHour(void);

//----------------------------�õ��ۼ�����---------------------------------
#define HourIncFlash_GetDay()  (HourIncFlash_GetAddHour() / 24)

//----------------------------�õ����е�Сʱ��-----------------------------
#define HourIncFlash_GetHour()  (HourIncFlash_GetAddHour() % 24)

//----------------------------------�õ�Сʱ�е�����--------------------------
unsigned short HourIncFlash_GetSecInHour(void);

//---------------------------�õ�Сʱ�еķ�����-----------------------------
#define HourIncFlash_GetMinue()  (HourIncFlash_GetSecInHour() / 60)

//----------------------------------�õ����е�����--------------------------
#define HourIncFlash_GetSec()  (HourIncFlash_GetSecInHour() % 60)

//----------------------------------�õ�Tick����----------------------------
#define HourIncFlash_GetTick()   (HourIncFlash.TickTimer)




#endif
