/***********************************************************************

		                   ָʾ�ư�ͨ������ģ��
��ģ��Ӧ���޹�
***********************************************************************/

#include "LightCh.h"
/***********************************************************************
		                      ��غ���ʵ��
***********************************************************************/

//-------------------------------����----------------------------------------
void LightCh_On(unsigned char Ch)
{
  switch(Ch){
    case 0: OpenLight0(); break;
    case 1: OpenLight1(); break;
    case 2: OpenLight2(); break;
    case 3: OpenLight3(); break;
    case 4: OpenLight4(); break;
    case 5: OpenLight5(); break;
    case 6: OpenLight6(); break;
    case 7: OpenLight7(); break;
    default: break;
  }
}

//--------------------------------�ص�---------------------------------------
void LightCh_Off(unsigned char Ch)
{
  switch(Ch){
    case 0: CloseLight0(); break;
    case 1: CloseLight1(); break;
    case 2: CloseLight2(); break;
    case 3: CloseLight3(); break;
    case 4: CloseLight4(); break;
    case 5: CloseLight5(); break;
    case 6: CloseLight6(); break;
    case 7: CloseLight7(); break;
    default: break;
  }
}