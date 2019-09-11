/***********************************************************

                标准延时模块
此模块与应用无关
************************************************************/
#ifndef  _DELAY_H
#define  _DELAY_H  


//--------------------Us延时函数----------------------------
void Delay_Us(unsigned short DelayUs);
#define vDelayUs(delayus) Delay_Us(delayus) //为保持兼容性
#define DelayUs(delayus) Delay_Us(delayus) //为保持兼容性

//--------------------Ms延时函数----------------------------
void Delay_Ms(unsigned short DelayMs);
#define vDelayMs(delayms) Delay_Ms(delayms) //为保持兼容性
#define DelayMs(delayms) Delay_Ms(delayms) //为保持兼容性

#endif
