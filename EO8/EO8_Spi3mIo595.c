/***************************************************************************

        扩展输出8位()接口=>在HC595芯片上，通过Spi3mIo驱动程序实现

***************************************************************************/
#include "EO8.h"
#include "Spi3mIo.h" 
#include <string.h>

struct _EO8  EO8;//直接单例化

//需在全局里定义 其所在通道, 默认0
#ifndef EO8_SPI3_MIO_595_CH
  #define EO8_SPI3_MIO_595_CH  0 
#endif

/***************************************************************************
                              相关函数
***************************************************************************/

//------------------------------初始化函数------------------------------
void EO8_Init(void)
{
  EO8.O = EO8_DEFAULT;
  //Spi3mIo驱动可能为非独占式，不在此初始化
  EO8_FaskTask();//开机立即更新一次
}

//-----------------------------快速任务函数-----------------------------
//最长调用时间建议<=10ms
void EO8_FaskTask(void)
{
  if(EO8.PrvO == EO8.O) return;
  Spi3mIo_RW(EO8_SPI3_MIO_595_CH, 
             &EO8.O, 1, //写入数据，1个
             NULL, 0); //无需读回
  EO8.PrvO = EO8.O;
}

