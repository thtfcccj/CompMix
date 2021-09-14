/*****************************************************************************

		                 ��EEPROM��ʵ�ֵ�Сʱ�ۼ�ģ��ʵ��

*****************************************************************************/

#include "HourIncEeprom.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include <string.h>

struct _HourIncEeprom HourIncEeprom;          //ֱ��ʵ����

static const struct _HourIncEepromInfo _InfoDefault = {
  0,//Сʱ����ֵ
  HOUR_INC_EEPROM_TO_HOUR_DEFAULT,    //����1Сʱ��Task����ֵ,����У׼Сʱ����
};

/*******************************************************************************
		                      ��غ���ʵ��
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void HourIncEeprom_Init(unsigned char IsInited)
{
  memset(&HourIncEeprom, 0, sizeof(struct _HourIncEeprom));
  //��Ԥ��
  Eeprom_Rd(HourIncEeprom_GetInfoBase(),
                &HourIncEeprom,
                sizeof(struct _HourIncEepromInfo));
  HourIncEeprom.HourCalibrationOV = 0xffff;//У׼��
  if(!IsInited || (HourIncEeprom.Info.Hour >= 0xff000000)){//װ��Ĭ��
    memcpy(&HourIncEeprom, &_InfoDefault, sizeof(struct _HourIncEepromInfo));
    Eeprom_Wr(HourIncEeprom_GetInfoBase(),
              &HourIncEeprom,
              sizeof(struct _HourIncEepromInfo));
  }
}

//---------------------------Tick������-------------------------------
static  unsigned char _TickTimer;            //Tick����
//ÿTick(>= 0.5ms)����һ�Σ��������ʱ���ж���
void HourIncEeprom_TickTask(void)
{
  _TickTimer++;
  if(_TickTimer) return;
  //_TickTimer������0�� һ�����ڵ���
  HourIncEeprom.ToHourTimer++;
  if(HourIncEeprom.HourCalibrationOV) HourIncEeprom.HourCalibrationOV--;
}

//----------------------------128Tick������----------------------------
//����128Tick ,���ڱ���Flash������0��Сʱδ����1��Сʱ����
signed char HourIncEeprom_128TickTask(void)
{
  if(HourIncEeprom.ToHourTimer < HourIncEeprom.Info.ToHourCount) return 0;
  HourIncEeprom.ToHourTimer = 0;
  //1Сʱ���ˣ����洦��
  HourIncEeprom.Info.Hour++;
  Eeprom_Wr(HourIncEeprom_GetInfoBase(), &HourIncEeprom.Info.Hour, 4); //�洢ǰ4����
  return 1;
}

//---------------------------------СʱУ׼----------------------------------
//�Կ������ϴ�У׼��1Сʱʱ�����ô˺���У׼Сʱ
void HourIncEeprom_HourCalibration(void)
{
  unsigned short ToHourTimer  = 0xffff - HourIncEeprom.HourCalibrationOV;
  //ֵ̫С��ʱ�䲻��  
  if(ToHourTimer < (HOUR_INC_EEPROM_TO_HOUR_DEFAULT / 60)) return;
  HourIncEeprom.Info.ToHourCount = ToHourTimer;
  Eeprom_Wr(HourIncEeprom_GetInfoBase(),
           &HourIncEeprom,
           sizeof(struct _HourIncEepromInfo));
  HourIncEeprom.HourCalibrationOV = 0xffff;//������ִ��У׼
  HourIncEeprom.ToHourTimer = ToHourTimer;//����ʱִ�м�1Сʱ
}

//---------------------------------ģ��ָ���0ʱ��---------------------------
void HourIncEeprom_ResetToHour0(void)
{
  HourIncEeprom.Info.Hour = 0;
  Eeprom_Wr(HourIncEeprom_GetInfoBase(), &HourIncEeprom.Info.Hour, 4); //�洢ǰ4����
}

//-----------------------------�õ��ۼ�Сʱ��------------------------------
//AbsHourΪ����ľ���ʱ�䣬Ϊ0ʱ�ɻ�ô�ģ�鵱ǰСʱ��
//��ֵ��ʾ����ľ���ʱ��ȵ�ǰСʱ��Ҫ��
signed long HourIncEeprom_GetAddHour(unsigned long AbsHour)
{
  return HourIncEeprom.Info.Hour - AbsHour;
}

//----------------------------------�õ�Сʱ�е�����--------------------------
unsigned short HourIncEeprom_GetSecInHour(void)
{
  return ((unsigned long)HourIncEeprom.ToHourTimer * 3600) / 
          HourIncEeprom.Info.ToHourCount;
}


