/***********************************************************

                标准延时模块-在AVR 8M速度时的实现
此模块与应用无关
************************************************************/
#include "Delay.h"
#include <intrinsics.h>
#include <comp_a90.h>  //_NOP()
//#define _NOP() __no_operation()

//--------------------Us延时函数----------------------------
void Delay_Us(unsigned short DelayUs)
{
	for(; DelayUs>0; DelayUs--){
		_NOP();
		_NOP();
		//_NOP();
		//_NOP();
		///_NOP();
	}  
}


//--------------------Ms延时函数----------------------------
void Delay_Ms(unsigned short DelayMs)
{
	for(; DelayMs>0; DelayMs--)
	{
		Delay_Us(600);
	}  
}


