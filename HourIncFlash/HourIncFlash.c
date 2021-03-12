/*****************************************************************************

		            在Flash页内实现的小时累加模块实现

*****************************************************************************/

#include "HourIncFlash.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include "Flash.h"
#include <string.h>

struct _HourIncFlash HourIncFlash;          //直接实例化

static const struct _HourIncFlashInfo _InfoDefault = {
  HOUR_INC_FLASH_TO_HOUR_DEFAULT,    //到达1小时的Task计数值,用于校准小时精度
};

/*******************************************************************************
		                      相关函数实现
*******************************************************************************/

//---------------------------得到缓冲区内小时数---------------------------
//0或正值找完了，负值未找完
signed char _GetHourFromBuf(const unsigned char *pBuf){
  unsigned char Hour = 0;
  for(;Hour < HOUR_INC_FLASH_BUFFER_SIZE; Hour += (HOUR_INC_FLASH_WR_BCELL + 7) / 8){
      unsigned char Data = *pBuf;
      if(Data == 0xff) return Hour;//找完了   
    #if HOUR_INC_FLASH_WR_BCELL <= 8 //字节内时
      //(暂不支持)
    #endif
  }
  return -1;
};

//----------------------------初始化函数-------------------------------
void HourIncFlash_Init(unsigned char IsInited)
{
  memset(&HourIncFlash, 0, sizeof(struct _HourIncFlash));
  HourIncFlash.HourCalibrationOV = 0xffff;//校准用
  //=========================Info初始化=========================
  if(!IsInited){//装载默认
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
  //读取当前位置等信息
  //=========================读取擦除次数===========================
  struct _HourIncFlashHeader Header;
  Flash_Read(HOUR_INC_FLASH_PAGE_BASE, &Header,  sizeof(struct _HourIncFlashHeader));
  //首次启用初始化
  if((!IsInited) || (Header.EreaseCountAnti == 0xffff)){
    Header.EreaseCount = 0;
    Header.EreaseCountAnti = 0xffff; 
    Flash_Unlock();
    if(!IsInited) Flash_ErasePage(HOUR_INC_FLASH_PAGE_BASE); //先擦除
    Flash_Write(HOUR_INC_FLASH_PAGE_BASE,  
                &Header,  
                sizeof(struct _HourIncFlashHeader));
    Flash_Lock(); 
    return;
  }
  //读取后数据头错误及恢复
  if(Header.EreaseCount != (0xffff - Header.EreaseCountAnti)){
    HourIncFlash.Flag |= HOUR_INC_FLASH_HEADER_ERR;
    if(Header.EreaseCount >= 0x8000) return;//默认全为0
  }
  HourIncFlash.EreaseCount = Header.EreaseCount;
    
  //=========================获取写位置===========================
  unsigned char Buf[HOUR_INC_FLASH_BUFFER_SIZE]; //使用栈缓冲
  unsigned short Pos = sizeof(struct _HourIncFlashHeader);
  for(; Pos < (HOUR_INC_FLASH_PAGE_SIZE + HOUR_INC_FLASH_BUFFER_SIZE - 1); 
        Pos += HOUR_INC_FLASH_BUFFER_SIZE){
    Flash_Read(HOUR_INC_FLASH_PAGE_BASE + Pos, Buf, HOUR_INC_FLASH_BUFFER_SIZE);
    signed char InPos = _GetHourFromBuf(Buf);
    if(InPos >= 0){//找完了
      HourIncFlash.WrPos = Pos + InPos;
      break;
    }
    //else 未找完继续
  }
}

//---------------------------Tick任务函数-------------------------------
//每Tick(>= 0.5ms)调用一次，建议放在时基中断里
void HourIncFlash_TickTask(void)
{
  HourIncFlash.TickTimer++;
  if(HourIncFlash.TickTimer) return;
  //一个周期到了
  HourIncFlash.ToHourTimer++;
  if(HourIncFlash.HourCalibrationOV) HourIncFlash.HourCalibrationOV--;
}

//----------------------------128Tick任务函数----------------------------
//放入128Tick ,用于保存Flash
void HourIncFlash_128TickTask(void)
{
  if(HourIncFlash.ToHourTimer < HourIncFlash.Info.ToHourCount) return;
  HourIncFlash.ToHourTimer = 0;
  //1小时到了，保存处理
  HourIncFlash.WrPos += (HOUR_INC_FLASH_WR_BCELL + 7) / 8;//下一位置了
  //=========================一页写完了===========================
  if(HourIncFlash.WrPos >= HOUR_INC_FLASH_PAGE_SIZE){
    Flash_Unlock();
    Flash_ErasePage(HOUR_INC_FLASH_PAGE_BASE); //先擦除
    Flash_Lock(); 
    HourIncFlash.EreaseCount++; //增加了
    //重写数据头
    struct _HourIncFlashHeader Header;
    Header.EreaseCount = HourIncFlash.EreaseCount;
    Header.EreaseCountAnti = 0xffff - HourIncFlash.EreaseCount;
    Flash_Write(HOUR_INC_FLASH_PAGE_BASE,  
                &Header,  
                sizeof(struct _HourIncFlashHeader)); 
    HourIncFlash.WrPos = sizeof(struct _HourIncFlashHeader); //复原位置
    return;
  }
  //=========================一页继续写===========================
  unsigned char Buf[(HOUR_INC_FLASH_WR_BCELL + 7) / 8];
  #if HOUR_INC_FLASH_WR_BCELL < 8 //字节内时
    Buf = 0;//暂不支持
  #else
    memcpy(Buf, 0, sizeof(Buf)); //暂先全部写为0
  #endif
  Flash_Write(HOUR_INC_FLASH_PAGE_BASE + HourIncFlash.WrPos,  
              &Buf,  
              sizeof(Buf));
}

//---------------------------------小时校准----------------------------------
//自开机或上次校准到1小时时，调用此函数校准小时
void HourIncFlash_HourCalibration(void)
{
  unsigned short ToHourTimer  = 0xffff - HourIncFlash.HourCalibrationOV;
  //值太小，时间不够  
  if(ToHourTimer < (HOUR_INC_FLASH_TO_HOUR_DEFAULT / 60)) return;
  HourIncFlash.Info.ToHourCount = ToHourTimer;
  Eeprom_Wr(HourIncFlash_GetInfoBase(),
           &HourIncFlash,
           sizeof(struct _HourIncFlashInfo));
  HourIncFlash.HourCalibrationOV = 0xffff;//可重新执行校准
  HourIncFlash.ToHourTimer = ToHourTimer;//任务时执行加1小时
}

//-----------------------------得到一页内存储天数-----------------------------
#define _GetPosHour() (((HOUR_INC_FLASH_PAGE_SIZE - \
           sizeof(struct _HourIncFlashHeader)) * 8) / HOUR_INC_FLASH_WR_BCELL)

//-----------------------------得到累加小时数-----------------------------
unsigned long HourIncFlash_GetAddHour(void)
{
  //assert(HourIncFlash.WrPos >= sizeof(struct _HourIncFlashHeader));
  return (unsigned long)_GetPosHour() * HourIncFlash.EreaseCount + 
          (HourIncFlash.WrPos - sizeof(struct _HourIncFlashHeader));
}

//----------------------------------得到小时中的秒数--------------------------
unsigned short HourIncFlash_GetSecInHour(void)
{
  return ((unsigned long)HourIncFlash.ToHourTimer * 3600) / 
          HourIncFlash.Info.ToHourCount;
}


