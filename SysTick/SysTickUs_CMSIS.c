/******************************************************************
                  系统Us级Tick模块-在CMSIS中的实现
使用systimer硬件，及CMSIS标准库实现
********************************************************************/
#include "SysTickUs.h"
#include "CMSIS.h"

static volatile unsigned short _IntCount;//定时器中断计数

extern void SysTick_cbHandlerNotify(void); //定时器通报,外部实现

//---------------------时基定时器0初始化函数------------------------
void SysTickUs_Init()
{
  SysTick_Config(SystemCoreClock / 1000); //配置为1ms一次中断,形参为向下计时load值 
}

//---------------------------cm3中断实现--------------------------
void SysTick_Handler(void)
{
  _IntCount++;
  SysTick_cbHandlerNotify();
}

//------------------得到定时器的值--------------------------------
TickType_t SysTickUs_Get(void)
{
  //临界区操作：最小颗粒度控制
  //SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Usk; //Enter Critcal
  TickType_t Us =  SysTick->VAL;
  //SysTick->CTRL |= SysTick_CTRL_TICKINT_Usk; //Exit Critcal
  
  //“如果在上次读取本寄存器后， SysTick 已经计到了 0，则该位为 1,”这个读一次CTRL
  volatile uint32_t Dump = SysTick->CTRL;
  
  return ((TickType_t)_IntCount * 1000) + (Us / SYS_MHZ); 
}


//---------------------时基定时器查询方式------------------------
//放快速进程中，或长延时进程中防止错过时间
//在此函数中,硬件时基到时
void SysTickUs_FastTask(void)
{
  if(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) return;
  _IntCount++;
  //退出临界区,略
}

