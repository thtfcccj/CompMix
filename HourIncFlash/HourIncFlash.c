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

//-----------------------------定义数据头大小---------------------------------
#if HOUR_INC_FLASH_WR_BCELL < 64   //4字节对齐时，
  #define _HEADER_SIZE     4
#else
  #define _HEADER_SIZE    (HeaderSize_t / 8)
#endif 

//-----------------------------得到一页内可存储的小时数-------------------------
#if HOUR_INC_FLASH_WR_BCELL < 8 //字节内时
  #define _GetPageHourCount() (((HOUR_INC_FLASH_PAGE_SIZE - _HEADER_SIZE)) * \
                              (8 / HOUR_INC_FLASH_WR_BCELL))
#else //字节为单位时
  #define _GetPageHourCount() (((HOUR_INC_FLASH_PAGE_SIZE - _HEADER_SIZE)) / \
                              (HOUR_INC_FLASH_WR_BCELL / 8))
#endif

//------------------------------得到缓冲区内小时数------------------------------
//0或正值找完了，负值未找完
signed char _GetHourFromBuf(const unsigned char *pBuf){
  unsigned char Pos = 0;
  for(;Pos < HOUR_INC_FLASH_BUFFER_SIZE; Pos += (HOUR_INC_FLASH_WR_BCELL + 7) / 8){
    unsigned char Data = *(pBuf + Pos);   
    #if HOUR_INC_FLASH_WR_BCELL < 8 //字节内时
      if(Data == 0xff) //找完了
        return Pos * (8 / HOUR_INC_FLASH_WR_BCELL);
      else if(Data != 0){//写了一部分才完,字节内查找
        unsigned char BitPos = 0;
        for(; BitPos < 8; BitPos++){
          if(Data & (1 << BitPos)) break;
        }
        return (BitPos / HOUR_INC_FLASH_WR_BCELL) + Pos * (8 / HOUR_INC_FLASH_WR_BCELL);
      }
      //else ==0时继续
    #else
      if(Data == 0xff) //找完了(这里只找了首个未被写0的)
        return Pos / (HOUR_INC_FLASH_WR_BCELL / 8);
    #endif
  }
  return -1;
}

//---------------------------------初始化函数-----------------------------------
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
  if((!IsInited) || (Header.EreaseCount == 0xffff)){
    HourIncFlash_ResetToHour0();
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
  unsigned short Pos = 0;
  for(; Pos < (HOUR_INC_FLASH_PAGE_SIZE - _HEADER_SIZE); 
        Pos += HOUR_INC_FLASH_BUFFER_SIZE){
    Flash_Read((HOUR_INC_FLASH_PAGE_BASE + _HEADER_SIZE) 
                 + Pos,Buf,HOUR_INC_FLASH_BUFFER_SIZE);
    signed char InPos = _GetHourFromBuf(Buf);
    if(InPos >= 0){//找完了
      #if HOUR_INC_FLASH_WR_BCELL < 8 //字节内时
        HourIncFlash.InPageHour = (Pos * (8 / HOUR_INC_FLASH_WR_BCELL)) + InPos;
      #else
        HourIncFlash.InPageHour = (Pos / (HOUR_INC_FLASH_WR_BCELL / 8)) + InPos;
      #endif
      break;
    }
    //else 未找完继续
  }
}

//---------------------------已写0时的掩码-------------------------------
#if HOUR_INC_FLASH_WR_BCELL < 8 //字节内时
static const unsigned char _BitedPos0[] = {
  0xff, 0xfe, 0xfc, 0xf8, 0xf0,0xe0, 0xc0, 0x80
};
#endif

//---------------------------Tick任务函数-------------------------------
//每Tick(>= 0.5ms)调用一次，建议放在时基中断里
void HourIncFlash_TickTask(void)
{
  HourIncFlash.TickTimer++;
  if(HourIncFlash.TickTimer) return;
  //HourIncFlash.TickTimer计数到0， 一个周期到了
  HourIncFlash.ToHourTimer++;
  if(HourIncFlash.HourCalibrationOV) HourIncFlash.HourCalibrationOV--;
}

//----------------------------128Tick任务函数----------------------------
//放入128Tick ,用于保存Flash，返回0：小时未到，1：小时到了
signed char HourIncFlash_128TickTask(void)
{
  if(HourIncFlash.ToHourTimer < HourIncFlash.Info.ToHourCount) return 0;
  HourIncFlash.ToHourTimer = 0;
  //1小时到了，保存处理
  HourIncFlash.InPageHour++;
  //=========================一页写完了===========================
  if(HourIncFlash.InPageHour >= _GetPageHourCount()){
    Flash_Unlock();
    Flash_ErasePage(HOUR_INC_FLASH_PAGE_BASE); //先擦除
    Flash_Lock(); 
    HourIncFlash.EreaseCount++; //增加了
    HourIncFlash.InPageHour = 0; //复原位置    
    //重写数据头
    struct _HourIncFlashHeader Header;
    Header.EreaseCount = HourIncFlash.EreaseCount;
    Header.EreaseCountAnti = 0xffff - HourIncFlash.EreaseCount;
    Flash_Write(HOUR_INC_FLASH_PAGE_BASE,  
                &Header,  
                sizeof(struct _HourIncFlashHeader)); 
    //HourIncFlash.Flag = 0;
    //这里校验写入是否正确(略)

    return -1;
  }
  //=========================一页内继续写===========================
  unsigned char Buf[(HOUR_INC_FLASH_WR_BCELL + 7) / 8];
  unsigned short WrPos;
  #if HOUR_INC_FLASH_WR_BCELL < 8 //字节内时
    WrPos = (HourIncFlash.InPageHour - 1) / (8 / HOUR_INC_FLASH_WR_BCELL);
    unsigned char BitPos = (HourIncFlash.InPageHour % 
      (8 / HOUR_INC_FLASH_WR_BCELL)) * HOUR_INC_FLASH_WR_BCELL;
    if(BitPos == 0) Buf[0] = 0;//本字节满了
    else Buf[0] = _BitedPos0[BitPos];//高位的满了
  #else
    WrPos = (HourIncFlash.InPageHour - 1) * (HOUR_INC_FLASH_WR_BCELL / 8); 
    memset(Buf, 0, sizeof(Buf)); //暂先全部写为0
  #endif
  Flash_Write((HOUR_INC_FLASH_PAGE_BASE + _HEADER_SIZE) +  
              WrPos, Buf, sizeof(Buf));
  //这里校验写入是否正确(略)
  return 1;
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

//---------------------------------模块恢复至0时间---------------------------
void HourIncFlash_ResetToHour0(void)
{
  Flash_Unlock();
  Flash_ErasePage(HOUR_INC_FLASH_PAGE_BASE); //先擦除
  struct _HourIncFlashHeader Header;
  Header.EreaseCount = 0;
  Header.EreaseCountAnti = 0xffff; 
  Flash_Write(HOUR_INC_FLASH_PAGE_BASE,      //再写入头
              &Header,  
              sizeof(struct _HourIncFlashHeader));
  Flash_Lock(); 
  //这里校验写入是否正确(略)

  //最后初始化内部相关的影响
  HourIncFlash.EreaseCount = 0;
  HourIncFlash.InPageHour = 0;
  //HourIncFlash.Flag = 0;
}

//-----------------------------得到累加小时数------------------------------
//AbsHour为保存的绝对时间，为0时可获得此模块当前小时点
//负值表示保存的绝对时间比当前小时点要早
signed long HourIncFlash_GetAddHour(unsigned long AbsHour)
{
  unsigned long CurAbsHour = HourIncFlash.InPageHour + 
    (unsigned long)_GetPageHourCount() * HourIncFlash.EreaseCount;
  return CurAbsHour - AbsHour;
}

//----------------------------------得到小时中的秒数--------------------------
unsigned short HourIncFlash_GetSecInHour(void)
{
  return ((unsigned long)HourIncFlash.ToHourTimer * 3600) / 
          HourIncFlash.Info.ToHourCount;
}


