/*****************************************************************************

		            ��Flashҳ��ʵ�ֵ�Сʱ�ۼ�ģ��ʵ��

*****************************************************************************/

#include "HourIncFlash.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include "Flash.h"
#include <string.h>

struct _HourIncFlash HourIncFlash;          //ֱ��ʵ����

static const struct _HourIncFlashInfo _InfoDefault = {
  HOUR_INC_FLASH_TO_HOUR_DEFAULT,    //����1Сʱ��Task����ֵ,����У׼Сʱ����
};

/*******************************************************************************
		                      ��غ���ʵ��
*******************************************************************************/

//---------------------------�õ���������Сʱ��---------------------------
//0����ֵ�����ˣ���ֵδ����
signed char _GetHourFromBuf(const unsigned char *pBuf){
  unsigned char Hour = 0;
  for(;Hour < HOUR_INC_FLASH_BUFFER_SIZE; Hour += (HOUR_INC_FLASH_WR_BCELL + 7) / 8){
      unsigned char Data = *pBuf;
      if(Data == 0xff) return Hour;//������   
    #if HOUR_INC_FLASH_WR_BCELL <= 8 //�ֽ���ʱ
      //(�ݲ�֧��)
    #endif
  }
  return -1;
};

//----------------------------��ʼ������-------------------------------
void HourIncFlash_Init(unsigned char IsInited)
{
  memset(&HourIncFlash, 0, sizeof(struct _HourIncFlash));
  HourIncFlash.HourCalibrationOV = 0xffff;//У׼��
  //=========================Info��ʼ��=========================
  if(!IsInited){//װ��Ĭ��
    memcpy(&HourIncFlash, &_InfoDefault, sizeof(struct _HourIncFlashInfo));
    Eeprom_Wr(HourIncFlash_GetInfoBase(),
              &HourIncFlash,
              sizeof(struct _HourIncFlashInfo));
  }
  else{
    Eeprom_Rd(HourIncFlash_GetInfoBase(),
                &HourIncFlash,
                sizeof(struct _HourIncFlashInfo));
  }
  //��ȡ��ǰλ�õ���Ϣ
  //=========================��ȡ��������===========================
  struct _HourIncFlashHeader Header;
  Flash_Read(HOUR_INC_FLASH_PAGE_BASE, &Header,  sizeof(struct _HourIncFlashHeader));
  //�״����ó�ʼ��
  if((!IsInited) || (Header.EreaseCountAnti == 0xffff)){
    Header.EreaseCount = 0;
    Header.EreaseCountAnti = 0xffff; 
    Flash_Unlock();
    if(!IsInited) Flash_ErasePage(HOUR_INC_FLASH_PAGE_BASE); //�Ȳ���
    Flash_Write(HOUR_INC_FLASH_PAGE_BASE,  
                &Header,  
                sizeof(struct _HourIncFlashHeader));
    Flash_Lock(); 
    return;
  }
  //��ȡ������ͷ���󼰻ָ�
  if(Header.EreaseCount != (0xffff - Header.EreaseCountAnti)){
    HourIncFlash.Flag |= HOUR_INC_FLASH_HEADER_ERR;
    if(Header.EreaseCount >= 0x8000) return;//Ĭ��ȫΪ0
  }
  HourIncFlash.EreaseCount = Header.EreaseCount;
    
  //=========================��ȡдλ��===========================
  unsigned char Buf[HOUR_INC_FLASH_BUFFER_SIZE]; //ʹ��ջ����
  unsigned short Pos = sizeof(struct _HourIncFlashHeader);
  for(; Pos < (HOUR_INC_FLASH_PAGE_SIZE + HOUR_INC_FLASH_BUFFER_SIZE - 1); 
        Pos += HOUR_INC_FLASH_BUFFER_SIZE){
    Flash_Read(HOUR_INC_FLASH_PAGE_BASE + Pos, Buf, HOUR_INC_FLASH_BUFFER_SIZE);
    signed char InPos = _GetHourFromBuf(Buf);
    if(InPos >= 0){//������
      HourIncFlash.WrPos = Pos + InPos;
      break;
    }
    //else δ�������
  }
}

//---------------------------Tick������-------------------------------
//ÿTick(>= 0.5ms)����һ�Σ��������ʱ���ж���
void HourIncFlash_TickTask(void)
{
  HourIncFlash.TickTimer++;
  if(HourIncFlash.TickTimer) return;
  //һ�����ڵ���
  HourIncFlash.ToHourTimer++;
  if(HourIncFlash.HourCalibrationOV) HourIncFlash.HourCalibrationOV--;
}

//----------------------------128Tick������----------------------------
//����128Tick ,���ڱ���Flash
void HourIncFlash_128TickTask(void)
{
  if(HourIncFlash.ToHourTimer < HourIncFlash.Info.ToHourCount) return;
  HourIncFlash.ToHourTimer = 0;
  //1Сʱ���ˣ����洦��
  HourIncFlash.WrPos += (HOUR_INC_FLASH_WR_BCELL + 7) / 8;//��һλ����
  //=========================һҳд����===========================
  if(HourIncFlash.WrPos >= HOUR_INC_FLASH_PAGE_SIZE){
    Flash_Unlock();
    Flash_ErasePage(HOUR_INC_FLASH_PAGE_BASE); //�Ȳ���
    Flash_Lock(); 
    HourIncFlash.EreaseCount++; //������
    //��д����ͷ
    struct _HourIncFlashHeader Header;
    Header.EreaseCount = HourIncFlash.EreaseCount;
    Header.EreaseCountAnti = 0xffff - HourIncFlash.EreaseCount;
    Flash_Write(HOUR_INC_FLASH_PAGE_BASE,  
                &Header,  
                sizeof(struct _HourIncFlashHeader)); 
    HourIncFlash.WrPos = sizeof(struct _HourIncFlashHeader); //��ԭλ��
    return;
  }
  //=========================һҳ����д===========================
  unsigned char Buf[(HOUR_INC_FLASH_WR_BCELL + 7) / 8];
  #if HOUR_INC_FLASH_WR_BCELL < 8 //�ֽ���ʱ
    Buf = 0;//�ݲ�֧��
  #else
    memcpy(Buf, 0, sizeof(Buf)); //����ȫ��дΪ0
  #endif
  Flash_Write(HOUR_INC_FLASH_PAGE_BASE + HourIncFlash.WrPos,  
              &Buf,  
              sizeof(Buf));
}

//---------------------------------СʱУ׼----------------------------------
//�Կ������ϴ�У׼��1Сʱʱ�����ô˺���У׼Сʱ
void HourIncFlash_HourCalibration(void)
{
  unsigned short ToHourTimer  = 0xffff - HourIncFlash.HourCalibrationOV;
  //ֵ̫С��ʱ�䲻��  
  if(ToHourTimer < (HOUR_INC_FLASH_TO_HOUR_DEFAULT / 60)) return;
  HourIncFlash.Info.ToHourCount = ToHourTimer;
  Eeprom_Wr(HourIncFlash_GetInfoBase(),
           &HourIncFlash,
           sizeof(struct _HourIncFlashInfo));
  HourIncFlash.HourCalibrationOV = 0xffff;//������ִ��У׼
  HourIncFlash.ToHourTimer = ToHourTimer;//����ʱִ�м�1Сʱ
}

//-----------------------------�õ�һҳ�ڴ洢����-----------------------------
#define _GetPosHour() (((HOUR_INC_FLASH_PAGE_SIZE - \
           sizeof(struct _HourIncFlashHeader)) * 8) / HOUR_INC_FLASH_WR_BCELL)

//-----------------------------�õ��ۼ�Сʱ��-----------------------------
unsigned long HourIncFlash_GetAddHour(void)
{
  //assert(HourIncFlash.WrPos >= sizeof(struct _HourIncFlashHeader));
  return (unsigned long)_GetPosHour() * HourIncFlash.EreaseCount + 
          (HourIncFlash.WrPos - sizeof(struct _HourIncFlashHeader));
}

//----------------------------------�õ�Сʱ�е�����--------------------------
unsigned short HourIncFlash_GetSecInHour(void)
{
  return ((unsigned long)HourIncFlash.ToHourTimer * 3600) / 
          HourIncFlash.Info.ToHourCount;
}


