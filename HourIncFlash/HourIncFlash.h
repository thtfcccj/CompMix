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

/*****************************************************************************
		                      相关配置
*****************************************************************************/

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

//定义1h计数默认值(256Tick为单位 )，以提高小时精度
#ifndef HOUR_INC_FLASH_TO_HOUR_DEFAULT  
  #define HOUR_INC_FLASH_TO_HOUR_DEFAULT   14062  //无误差时为14062    
#endif
                            
//读写一次Flash时缓冲大小，必须为4的倍数且BCELL对齐
//值越大调用Flash读写次数越小但占用栈空间越多,开机时调用
#ifndef HOUR_INC_FLASH_BUFFER_SIZE  
  #define HOUR_INC_FLASH_BUFFER_SIZE   (4 * 2)
#endif

/***********************************************************************
		                      相关结构
***********************************************************************/


//信息结构
struct _HourIncFlashInfo{
  unsigned short ToHourCount;    //到达1小时的Task计数值,用于校准小时精度
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
  
  //小时内RAM计数:
  unsigned short ToHourTimer; //到1小时时计数值
  unsigned short HourCalibrationOV;//1小时校准专用(因ToHourTimer会提前结束故不能用)
  unsigned char TickTimer;    //Tick计数
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
//每Tick(>= 0.5ms)调用一次，建议放在时基中断里
void HourIncFlash_TickTask(void);

//----------------------------128Tick任务函数------------------------------
//放入128Tick ,用于保存Flash，返回0：小时未到，1：小时到了
signed char HourIncFlash_128TickTask(void);

//---------------------------------小时校准----------------------------------
//自开机或上次校准到1小时，调用此函数校准小时
void HourIncFlash_HourCalibration(void);

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
unsigned short HourIncFlash_GetSecInHour(void);

//---------------------------得到小时中的分钟数-----------------------------
#define HourIncFlash_GetMinue()  (HourIncFlash_GetSecInHour() / 60)

//----------------------------------得到分中的秒数--------------------------
#define HourIncFlash_GetSec()  (HourIncFlash_GetSecInHour() % 60)

//----------------------------------得到Tick计数----------------------------
#define HourIncFlash_GetTick()   (HourIncFlash.TickTimer)

#endif
