/***********************************************************

                ��׼��ʱģ��-��AVR 8M�ٶ�ʱ��ʵ��
��ģ����Ӧ���޹�
************************************************************/
#include "Delay.h"
#include <intrinsics.h>
#include <comp_a90.h>  //_NOP()
//#define _NOP() __no_operation()

//--------------------Us��ʱ����----------------------------
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


//--------------------Ms��ʱ����----------------------------
void Delay_Ms(unsigned short DelayMs)
{
	for(; DelayMs>0; DelayMs--)
	{
		Delay_Us(600);
	}  
}


