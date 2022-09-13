/***********************************************************************

		                   ��ʾ��ַ����ʱ��
��ģ�������ά����ʾ��ַ��ʱ���������������ʾ��Ӧ���޹�
***********************************************************************/
#ifndef _DISP_ADR_TIMER_H
#define	_DISP_ADR_TIMER_H
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif

/***********************************************************************
		                      �������
***********************************************************************/

//�Ƿ�֧�ִ�ģ��
//#define SUPPORT_DISP_ADR_TIMER   

//�������ʱ��,256msΪ��λ
#ifndef DISP_ADR_TIMER_COUNT
  #define	DISP_ADR_TIMER_COUNT  (4 * 10)     
#endif
                            
/***********************************************************************
		                      ��ؽṹ
***********************************************************************/

extern unsigned char DispAdrTimer; 

//����main������
#define DECLEAR_DISP_ADR_TIMER   unsigned char DispAdrTimer = 0;

/***********************************************************************
		                      ��غ���
***********************************************************************/

//----------------------��ͨ������-------------------------------
//����256ms������ɨ��
#define DispAdrTimer_Task()  do{if(DispAdrTimer) DispAdrTimer--;}while(0)

//-----------------------------��������----------------------------
#define DispAdrTimer_Start()  do{DispAdrTimer = DISP_ADR_TIMER_COUNT;}while(0)
//ָ��������
#define DispAdrTimer_StartS(sec)  do{DispAdrTimer = (sec) * 4;}while(0)

//-------------------------�Ƿ�����ʾ״̬---------------------------
#define DispAdrTimer_IsDisp()   (DispAdrTimer)

//-------------------------����������ʾ---------------------------
#define DispAdrTimer_EndDisp()    do{DispAdrTimer = 0;}while(0)

#endif
