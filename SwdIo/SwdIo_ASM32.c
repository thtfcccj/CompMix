/***********************************************************************

		                    SWD复用IO口切换模块-在ASM32中的实现
此模块可用于SWD与IO口之间的切换，也可用于通过关闭SWD来加解密
***********************************************************************/

#include "SwdIo.h"
#include "CMSIS.h"

#ifdef  SUPPORT_SWD_IO_UNLOCK //支持SWD通过IO解锁时
  unsigned char SwdIo_LockTimer = SWD_IO_UNLICK_TIME;
#endif

/***********************************************************************
		                      相关函数实现
***********************************************************************/

//---------------------SWD口配置为普通IO-------------------------------
#ifndef _APP_DEBUG   //正常工作时
void SwdIo_ToIo(void)
{
  RCC->REGLOCK = RCC_REGLOCKKEY;
  RCC->IOMUX = ((unsigned long)0x5A69 << 16) | (0 << 0);
  RCC->REGLOCK = RCC_RESGLOCKKEY;
}
#endif  

//---------------------SWD配置为普通IO后置输出0-------------------
#ifndef _APP_DEBUG   //正常工作时
//应在初始化时调用SwdIo_ToIo()后，若口不用,调用此函数配置回输出0模式
void SwdIo_IoOut0(void)
{
  //默认配置为输入高阻，这里配置为输出0以提高抗扰能力
  GPIOC->DOCL = (1 << 7);  GPIOC->DIR |= (1 << 7);
  GPIOD->DOCL = (1 << 1);  GPIOD->DIR |= (1 << 1);    
}
#endif

//-------------------------------配置回SWD模式---------------------------
#ifndef _APP_DEBUG   //正常工作时
//同时将SWD口配置为输入模式以用于SWD接入
void SwdIo_ToSwd(void)
{
  RCC->REGLOCK = RCC_REGLOCKKEY;
  RCC->IOMUX = ((unsigned long)0x5A69 << 16) | (1 << 0); 
  RCC->REGLOCK = RCC_RESGLOCKKEY;
  //配置回输入模式防止死锁
  GPIOC->DIR &= ~(1 << 7); GPIOD->DIR &= ~(1 << 1);
  GPIOC->AFR &= ~((unsigned long)0x0f << (7 * 4)); 
  GPIOD->AFR &= ~((unsigned long)0x0f << (1 * 4)); 
}
#endif

//----------------------普通任务函数-------------------------------
#ifdef  SUPPORT_SWD_IO_UNLOCK //支持SWD通过IO解锁时
//放入256ms进程中扫描
void SwdIo_Task(void)
{
  if(SwdIo_cb_IsInvalid()){//无效时
    SwdIo_LockTimer = SWD_IO_UNLICK_TIME;
    //注：端口可能其它模块在使用，故不能回锁

  }
  else{//低电平了
    if(!SwdIo_LockTimer){//解除锁定
      SwdIo_ToSwd();
    }
    else SwdIo_LockTimer--;
  }
}
#endif //SUPPORT_SWD_IO_UNLOCK


