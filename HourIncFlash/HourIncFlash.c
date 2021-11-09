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

//-----------------------------��������ͷ��С---------------------------------
#if HOUR_INC_FLASH_WR_BCELL < 64   //4�ֽڶ���ʱ��
  #define _HEADER_SIZE     4
#else
  #define _HEADER_SIZE    (HeaderSize_t / 8)
#endif 

//-----------------------------�õ�һҳ�ڿɴ洢��Сʱ��-------------------------
#if HOUR_INC_FLASH_WR_BCELL < 8 //�ֽ���ʱ
  #define _GetPageHourCount() (((HOUR_INC_FLASH_PAGE_SIZE - _HEADER_SIZE)) * \
                              (8 / HOUR_INC_FLASH_WR_BCELL))
#else //�ֽ�Ϊ��λʱ
  #define _GetPageHourCount() (((HOUR_INC_FLASH_PAGE_SIZE - _HEADER_SIZE)) / \
                              (HOUR_INC_FLASH_WR_BCELL / 8))
#endif

//------------------------------�õ���������Сʱ��------------------------------
//0����ֵ�����ˣ���ֵδ����
signed char _GetHourFromBuf(const unsigned char *pBuf){
  unsigned char Pos = 0;
  for(;Pos < HOUR_INC_FLASH_BUFFER_SIZE; Pos += (HOUR_INC_FLASH_WR_BCELL + 7) / 8){
    unsigned char Data = *(pBuf + Pos);   
    #if HOUR_INC_FLASH_WR_BCELL < 8 //�ֽ���ʱ
      if(Data == 0xff) //������
        return Pos * (8 / HOUR_INC_FLASH_WR_BCELL);
      else if(Data != 0){//д��һ���ֲ���,�ֽ��ڲ���
        unsigned char BitPos = 0;
        for(; BitPos < 8; BitPos++){
          if(Data & (1 << BitPos)) break;
        }
        return (BitPos / HOUR_INC_FLASH_WR_BCELL) + Pos * (8 / HOUR_INC_FLASH_WR_BCELL);
      }
      //else ==0ʱ����
    #else
      if(Data == 0xff) //������(����ֻ�����׸�δ��д0��)
        return Pos / (HOUR_INC_FLASH_WR_BCELL / 8);
    #endif
  }
  return -1;
}

//---------------------------------��ʼ������-----------------------------------
void HourIncFlash_Init(unsigned char IsInited)
{
  memset(&HourIncFlash, 0, sizeof(struct _HourIncFlash));
  #ifdef SUPPORT_HOUR_INC_FLASH_CAL //����У׼ʱ
    HourIncFlash.HourCalibrationOV = 0xffff;
  #endif

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
  if((!IsInited) || (Header.EreaseCount == 0xffff)){
    HourIncFlash_ResetToHour0();
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
  unsigned short Pos = 0;
  for(; Pos < (HOUR_INC_FLASH_PAGE_SIZE - _HEADER_SIZE); 
        Pos += HOUR_INC_FLASH_BUFFER_SIZE){
    Flash_Read((HOUR_INC_FLASH_PAGE_BASE + _HEADER_SIZE) 
                 + Pos,Buf,HOUR_INC_FLASH_BUFFER_SIZE);
    signed char InPos = _GetHourFromBuf(Buf);
    if(InPos >= 0){//������
      #if HOUR_INC_FLASH_WR_BCELL < 8 //�ֽ���ʱ
        HourIncFlash.InPageHour = (Pos * (8 / HOUR_INC_FLASH_WR_BCELL)) + InPos;
      #else
        HourIncFlash.InPageHour = (Pos / (HOUR_INC_FLASH_WR_BCELL / 8)) + InPos;
      #endif
      break;
    }
    //else δ�������
  }
}

//---------------------------��д0ʱ������-------------------------------
#if HOUR_INC_FLASH_WR_BCELL < 8 //�ֽ���ʱ
static const unsigned char _BitedPos0[] = {
  0xff, 0xfe, 0xfc, 0xf8, 0xf0,0xe0, 0xc0, 0x80
};
#endif

//---------------------------Tick������-------------------------------
//if(HOUR_INC_FLASH_TO_HOUR_OV > 1)ʱ����ã����������Ӧ��ʱ�������ڵ���
void HourIncFlash_TickTask(void)
{
  #if (HOUR_INC_FLASH_TO_HOUR_OV > 36000)//0.1�����»��м���ʱ,U16װ�����ˡ�
    HourIncFlash.TickTimer++;
    if(HourIncFlash.TickTimer) return;
    //HourIncFlash.TickTimer������0�� һ�����ڵ���
  #endif 

  #if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1Сʱ���¼���ʱ
    HourIncFlash.ToHourTimer++;
  #endif

  #ifdef SUPPORT_HOUR_INC_FLASH_CAL //����У׼ʱ
  if(HourIncFlash.HourCalibrationOV) HourIncFlash.HourCalibrationOV--;
  #endif
}

//----------------------------128Tick������----------------------------
//if(HOUR_INC_FLASH_TO_HOUR_OV <= 1ʱ) 1Сʱ������һ�Σ���ֱ�ӱ���Сʱ
//if(HOUR_INC_FLASH_TO_HOUR_OV < 36000) ʱHourIncFlash_TickTask�����
//�������HOUR_INC_FLASH_TO_HOUR_OV *128��ʱ�������ڵ���
signed char HourIncFlash_128TickTask(void)
{
  #if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1Сʱ���¼���ʱ
    if(HourIncFlash.ToHourTimer < HourIncFlash_GetHourCount()) return 0;
    HourIncFlash.ToHourTimer = 0;
  #endif
  //1Сʱ���ˣ����洦��
  HourIncFlash.InPageHour++;
  if(HourIncFlash.OnHour < 0xffff) HourIncFlash.OnHour++;
  //=========================һҳд����===========================
  if(HourIncFlash.InPageHour > _GetPageHourCount()){//��GetPageHourCount+1�ػ�
    Flash_Unlock();
    Flash_ErasePage(HOUR_INC_FLASH_PAGE_BASE); //�Ȳ���
    Flash_Lock(); 
    HourIncFlash.EreaseCount++; //������
    HourIncFlash.InPageHour = 0; //��ԭλ��    
    //��д����ͷ
    struct _HourIncFlashHeader Header;
    Header.EreaseCount = HourIncFlash.EreaseCount;
    Header.EreaseCountAnti = 0xffff - HourIncFlash.EreaseCount;
    Flash_Write(HOUR_INC_FLASH_PAGE_BASE,  
                &Header,  
                sizeof(struct _HourIncFlashHeader)); 
    //HourIncFlash.Flag = 0;
    //����У��д���Ƿ���ȷ(��)

    return -1;
  }
  //=========================һҳ�ڼ���д===========================
  unsigned char Buf[(HOUR_INC_FLASH_WR_BCELL + 7) / 8];
  unsigned short WrPos;
  #if HOUR_INC_FLASH_WR_BCELL < 8 //�ֽ���ʱ
    WrPos = (HourIncFlash.InPageHour - 1) / (8 / HOUR_INC_FLASH_WR_BCELL);
    unsigned char BitPos = (HourIncFlash.InPageHour % 
      (8 / HOUR_INC_FLASH_WR_BCELL)) * HOUR_INC_FLASH_WR_BCELL;
    if(BitPos == 0) Buf[0] = 0;//���ֽ�����
    else Buf[0] = _BitedPos0[BitPos];//��λ������
  #else
    WrPos = (HourIncFlash.InPageHour - 1) * (HOUR_INC_FLASH_WR_BCELL / 8); //-1��
    memset(Buf, 0, sizeof(Buf)); //����ȫ��дΪ0
  #endif
  Flash_Write((HOUR_INC_FLASH_PAGE_BASE + _HEADER_SIZE) +  
              WrPos, Buf, sizeof(Buf));
  //����У��д���Ƿ���ȷ(��)
  return 1;
}

//------------------------------У׼����-------------------------------------
//HourIncFlash_HaveCalibration()ʱ�ɵ���, Ĭ��һ��1Сʱ����
#ifdef SUPPORT_HOUR_INC_FLASH_CAL
void HourIncFlash_Calibration(unsigned short Sec) //��Ϊ��λ
{
  if(HourIncFlash.HourCalibrationOV == 0) return; //����ʱ�����
  //��Sec��һ����1Сʱ�������Ӧ
  unsigned short ToHourTimer  = 0xffff - HourIncFlash.HourCalibrationOV;
  unsigned long Data = ((unsigned  long)3600 << 10) / Sec;
  ToHourTimer = (Data * ToHourTimer) >> 10;
  //ֵ̫С��ʱ�䲻��  
  if(ToHourTimer < (HOUR_INC_FLASH_TO_HOUR_DEFAULT / 60)) return;
  HourIncFlash.Info.ToHourCount = ToHourTimer;
  Eeprom_Wr(HourIncFlash_GetInfoBase(),
           &HourIncFlash,
           sizeof(struct _HourIncFlashInfo));
  HourIncFlash.HourCalibrationOV = 0xffff;//������ִ��У׼
  HourIncFlash.ToHourTimer = ToHourTimer;//����ʱִ�м�1Сʱ
}
#endif

//---------------------------------ģ��ָ���0ʱ��---------------------------
void HourIncFlash_ResetToHour0(void)
{
  Flash_Unlock();
  Flash_ErasePage(HOUR_INC_FLASH_PAGE_BASE); //�Ȳ���
  struct _HourIncFlashHeader Header;
  Header.EreaseCount = 0;
  Header.EreaseCountAnti = 0xffff; 
  Flash_Write(HOUR_INC_FLASH_PAGE_BASE,      //��д��ͷ
              &Header,  
              sizeof(struct _HourIncFlashHeader));
  Flash_Lock(); 
  //����У��д���Ƿ���ȷ(��)

  //����ʼ���ڲ���ص�Ӱ��
  HourIncFlash.EreaseCount = 0;
  HourIncFlash.InPageHour = 0;
  //HourIncFlash.Flag = 0;
}

//-----------------------------�õ��ۼ�Сʱ��------------------------------
//AbsHourΪ����ľ���ʱ�䣬Ϊ0ʱ�ɻ�ô�ģ�鵱ǰСʱ��
//��ֵ��ʾ����ľ���ʱ��ȵ�ǰСʱ��Ҫ��
signed long HourIncFlash_GetAddHour(unsigned long AbsHour)
{
  //��GetPageHourCount+1�ػ�
  unsigned long CurAbsHour = HourIncFlash.InPageHour + 
    (unsigned long)(_GetPageHourCount() + 1) * HourIncFlash.EreaseCount;
  return CurAbsHour - AbsHour;
}

//----------------------------------�õ�Сʱ�е�����--------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1Сʱ���¼���ʱ
unsigned short HourIncFlash_GetSecInHour(void)
{
  return ((unsigned long)HourIncFlash.ToHourTimer * 3600) / HourIncFlash_GetHourCount();
}
#endif


