/******************************************************************
                  系统Us级Tick模块-在CMSIS中直接用24位SysTickTimer实现
使用systimer硬件，及CMSIS标准库实现
********************************************************************/
#include "SysTickUs.h"
#include "CMSIS.h"
//注：SYS_MHZ为全局定义
//    与SysTimer配合时，需定义SYSTICK_T24值

//---------------------时基定时器0初始化函数------------------------
void SysTickUs_Init()
{
  SysTick_Config(0x1000000); //24bit满
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;//关中断 
}

//---------------------快速任务函数------------------------
//可放快速进程中，或长延时进程中防止错过时间
void SysTickUs_FastTask(void)
{

}

//------------------得到定时器的值--------------------------------
TickType_t SysTickUs_Get(void)
{
  return (0x1000000 - SysTick->VAL) / SYS_MHZ; 
}
