/***********************************************************

                标准延时模块-在PIC 8M速度时的实现
此模块与应用无关
************************************************************/
#include "Delay.h"

//--------------------Us延时函数----------------------------
void Delay_Us(unsigned short DelayUs)
{
	for(; DelayUs > 0; DelayUs--){
		//asm("NOP");
		//asm("NOP");
	}  
}

//--------------------Ms延时函数----------------------------
void Delay_Ms(unsigned short DelayMs)
{
	for(; DelayMs>0; DelayMs--)
	{
		Delay_Us(800);
	}  
}

