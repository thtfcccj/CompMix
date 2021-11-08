/*****************************************************************************

		                    在Flash页内实现的小时累加模块
此模块主要用于没有RTC及掉电RAM或EEPROM时，系统还需要记录设备工作小时相关的信息,
  使用时保存小时点(需以unsigned long 为单位)，减此值即为实际时间
此模块使用一页Flash(即此模块专用)来保存自程序启用以来的工作小时数
保存精度以1小时为单位，如59分59秒999毫秒关机时，也将认为是0小时
可进行小时校正，将Tick任务放在中断中可提高精度，未对温度对时钟影响进行补偿

此模块独立与硬件(通过Flash模块实现)与应用
*****************************************************************************/
#ifndef _HOUR_INC_FLASH_H
#define	_HOUR_INC_FLASH_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif
/*****************************************************************************
		                      相关配置
*****************************************************************************/

//本模块可选时，建议的定义
//#define SUPPORT_HOUR_INC_FLASH  //支持Flash页内实现的小时计时时

//定义时否支持此模块信息结构恢复功能
//不启用时，此信息结构存储在专用单页的前几个字节上，启用后，复用EEPROM存储
//此信息，开机时两者比较，以正确的信息进行恢复
//#define SUPPORT_HOUR_INC_FLASH_RESTORE   //暂未实现此功能

//(整片擦除后)写Flash时，允许的最小写入bit单位
//针对Flash不同，有的允许Bit依次由1写0(如部分外置Flash)，有的允许1bit,2bit,4bit. 
//有的允许2Byte为单位,4Byte为单位...可先设成1，再依次测试是否可写入以获是实际精度
//此值必须为：2^n，即：1，2，4，8(1Byte)，16(2Byte), 32(4Byte) 64(8Byte)...
#ifndef HOUR_INC_FLASH_WR_BCELL
  #define HOUR_INC_FLASH_WR_BCELL    8  
#endif

//定义存存在Flash页中的起始位置，4的倍数且BCELL对齐
#ifndef HOUR_INC_FLASH_PAGE_BASE
  #define HOUR_INC_FLASH_PAGE_BASE    (0x8000-0x600)
#endif

//定义专用Flash页大小，必须为4的倍数且BCELL对齐
#ifndef HOUR_INC_FLASH_PAGE_SIZE
  #define HOUR_INC_FLASH_PAGE_SIZE    512  
#endif

                            
//读写一次Flash时缓冲大小，必须为4的倍数且BCELL对齐
//值越大调用Flash读写次数越小但占用栈空间越多,开机时调用
#ifndef HOUR_INC_FLASH_BUFFER_SIZE  
  #define HOUR_INC_FLASH_BUFFER_SIZE   (4 * 2)
#endif

//定时TICK周期调至至1小时时的周期数, 默认1ms 调用一次(建议tick中断中)
//定义此值》=1时，表示外部有RTC,1小时调用一次直接保存,此状态不可标定
#ifndef HOUR_INC_FLASH_TO_HOUR_OV  
  #define HOUR_INC_FLASH_TO_HOUR_OV  (3600 * 1000 / 1)//默认1ms调用一次
#else //这里为示例,注意校准时，此为误差值，如10s，则误差修正为10s
//  #define HOUR_INC_FLASH_TO_HOUR_OV  (3600 * 1000 / 16))//16ms一次示例
//  #define HOUR_INC_FLASH_TO_HOUR_OV  (3600)//1S定时，如有1s基准时基时 
//  #define HOUR_INC_FLASH_TO_HOUR_OV  (360)//10S定时，如有10s基准时基时 
//  #define HOUR_INC_FLASH_TO_HOUR_OV  1//1h定时1h表示启用了RTC模块  
#endif

//定义1h计数默认值(256Tick为单位 )，以提高小时精度
#ifndef HOUR_INC_FLASH_TO_HOUR_DEFAULT
  #if (HOUR_INC_FLASH_TO_HOUR_OV > 36000)//0.1秒以下还有计数时有细化
    #define HOUR_INC_FLASH_TO_HOUR_DEFAULT  (HOUR_INC_FLASH_TO_HOUR_OV / 256)
  #else
    #define HOUR_INC_FLASH_TO_HOUR_DEFAULT   HOUR_INC_FLASH_TO_HOUR_OV
  #endif
#endif

//定义此是否可标定,1h表示启用了RTC模块，直接调用，强制禁止标定以与其同步
#if HOUR_INC_FLASH_TO_HOUR_OV <= 1
  #ifdef SUPPORT_HOUR_INC_FLASH_CAL
    #undef SUPPORT_HOUR_INC_FLASH_CAL
  #endif
#endif

/***********************************************************************
		                      相关结构
***********************************************************************/


//信息结构
struct _HourIncFlashInfo{
  //到达1小时的Task计数值,用于校准小时精度,不允许校准时此值可其它INFO用
  unsigned short ToHourCount;    
};

//Flash头(支持备份时此头更新时，将一并存储在Info中)
struct _HourIncFlashHeader{
  unsigned short EreaseCount;     //擦除计数，一次为一轮回，即对应天数
  unsigned short EreaseCountAnti; //擦除计数反码，用于校验
};

//主结构
struct _HourIncFlash{
  struct _HourIncFlashInfo Info;
  //小时FLASH计数
  unsigned short EreaseCount; //擦除计数，一次为一轮回，即对应天数
  unsigned short InPageHour;  //一页内经过的小时数（注意字单位是否超限）
  //开机小时数
  unsigned short OnHour;      
  
  //小时内RAM计数:
  #if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1小时以下计数时
  unsigned short ToHourTimer; //到1小时时计数值  
  #endif  
  
  #ifdef SUPPORT_HOUR_INC_FLASH_CAL //允许校准时
  unsigned short HourCalibrationOV;//1小时校准专用(因ToHourTimer会提前结束故不能用)
  #endif
  
  #if (HOUR_INC_FLASH_TO_HOUR_OV > 36000)//0.1秒以下还有计数时,U16装不下了。
  unsigned char TickTimer;    //Tick计数
  #endif
  
  unsigned char Flag;         //相关标志，见定义
};

//相关标志定义为
#define HOUR_INC_FLASH_HEADER_ERR 0x80  //数据头有误


extern struct _HourIncFlash HourIncFlash;          //直接实例化

/*******************************************************************************
		                      相关行为函数
*******************************************************************************/

//------------------------------初始化函数---------------------------------
void HourIncFlash_Init(unsigned char IsInited);

//------------------------------Tick任务函数-------------------------------
//if(HOUR_INC_FLASH_TO_HOUR_OV > 1)时需调用，放在与基对应的时基内周期调用
void HourIncFlash_TickTask(void);

//----------------------------128Tick任务函数------------------------------
//if(HOUR_INC_FLASH_TO_HOUR_OV <= 1时) 1小时到调用一次，即直接保存小时
//if(HOUR_INC_FLASH_TO_HOUR_OV < 36000) 时HourIncFlash_TickTask后调用
//否则放在HOUR_INC_FLASH_TO_HOUR_OV * 128的时基内周期调用
signed char HourIncFlash_128TickTask(void);

//-------------------------是否可校准函数-------------------------------------
#ifdef SUPPORT_HOUR_INC_FLASH_CAL
  #define HourIncFlash_HaveCalibration() (HourIncFlash.HourCalibrationOV > 0)
#elif !defined(HourIncFlash_HaveCalibration) //允许外部实现
  #define HourIncFlash_HaveCalibration() (0)
#endif

//------------------------------校准函数-------------------------------------
//HourIncFlash_HaveCalibration()时可调用, 默认一般1小时左右
#ifdef SUPPORT_HOUR_INC_FLASH_CAL
void HourIncFlash_Calibration(unsigned short Sec); //秒为单位
#elif !defined(HourIncFlash_Calibration) //允许外部实现
  #define HourIncFlash_Calibration(sec) do{}while(0)
#endif

//----------------------------得到小时溢出值----------------------------
#ifdef SUPPORT_HOUR_INC_FLASH_CAL //允许校准时
  #define HourIncFlash_GetHourCount() (HourIncFlash.Info.ToHourCount)
#else //不允许时，使用用户定义值
  #define HourIncFlash_GetHourCount() HOUR_INC_FLASH_TO_HOUR_DEFAULT
#endif

//---------------------------------模块恢复至0时间---------------------------
void HourIncFlash_ResetToHour0(void);

/*******************************************************************************
		                      相关属性函数
*******************************************************************************/

//-----------------------------得到累加小时数------------------------------
//AbsHour为保存的绝对时间，为0时可获得此模块当前小时点
//负值表示保存的绝对时间比当前小时点要早
signed long HourIncFlash_GetAddHour(unsigned long AbsHour);

//----------------------------得到累加小时中天数------------------------------
#define HourIncFlash_GetDay(absHour)  (HourIncFlash_GetAddHour(absHour) / 24)

//----------------------------得到累加小时中的小时数---------------------------
#define HourIncFlash_GetHour(absHour)  (HourIncFlash_GetAddHour(absHour) % 24)

//----------------------------------得到小时中的秒数--------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1小时以下计数时
unsigned short HourIncFlash_GetSecInHour(void);
#endif

//---------------------------得到小时中的分钟数-----------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1小时以下计数时
#define HourIncFlash_GetMinue()  (HourIncFlash_GetSecInHour() / 60)
#endif

//----------------------------------得到分中的秒数--------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 1)//1小时以下计数时
#define HourIncFlash_GetSec()  (HourIncFlash_GetSecInHour() % 60)
#endif

//----------------------------------得到Tick计数----------------------------
#if (HOUR_INC_FLASH_TO_HOUR_OV > 36000)//0.1秒以下还有计数时,U16装不下了。
#define HourIncFlash_GetTick()   (HourIncFlash.TickTimer)
#endif

#endif
