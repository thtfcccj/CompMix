/***************************************************************************

                    ����PWM�ź����������-��Picʹ��CCP�е�ʵ��

 ****************************************************************************/

#include <pic.h>
#include <string.h>
#include "PicBit.h"
#include "IoCtrl.h"

#include "PeriodPwmGen.h"
#include "string_P.h"

struct _PeriodPwmGen PeriodPwmGen;

/***************************************************************************
                            �ڲ�����
 ****************************************************************************/

//����PWMʱ��
#ifndef PERIOD_PWM_GEN_PS //PSֵ��������Ƶϵ��: 0:1, 1:4, 2:16, 3:64 
#define PERIOD_PWM_GEN_PS  0 
#endif

#ifndef PERIOD_PWM_GEN_PR //PRֵ������ϸ��Ƶ�� 
#define PERIOD_PWM_GEN_PR  5
#endif

#ifndef PERIOD_PWM_GEN_PL //PLֵ��������PERIOD_PWM_GEN_PR�ڵ�ռ�ձ�
#define PERIOD_PWM_GEN_PL   4
#endif

//����ʹ�õ�CCP,�޶���ΪCCP3
//#define PERIOD_PWM_GEN_USE_CCP_1 //ʹ��CCP1ʱ����
//#define PERIOD_PWM_GEN_USE_CCP_2 //ʹ��CCP1ʱ����
//#define PERIOD_PWM_GEN_USE_CCP_4 //ʹ��CCP1ʱ����

//����ʹ�õĶ�ʱ��,PWMģʽ��ʹ��(��8λģʽ)2/4/6��ʱ���е�һ��,������ʱʹ�ö�ʱ��4
//#define PERIOD_PWM_GEN_USE_TIMER_2 //ʹ�ö�ʱ��4ʱ����
//#define PERIOD_PWM_GEN_USE_TIMER_6 //ʹ�ö�ʱ��6ʱ����

/***************************************************************************
                        CCP���ú�ĺ�ת��
 ****************************************************************************/
//------------------------����CCP3�Ĵ�����ת��------------------------
#ifndef PERIOD_PWM_GEN_USE_CCP_1
#ifndef PERIOD_PWM_GEN_USE_CCP_2
#ifndef PERIOD_PWM_GEN_USE_CCP_4
#define CCPxCON       CCP3CON       //����
#define CCPRxL        CCPR3L       //��ʱ����8bit
#define CCPRxH        CCPR3H       //��ʱ����2bit
#define TMRS_SHIFT    4            //CCPTMRS��λֵ
#endif //#ifndef PERIOD_PWM_GEN_USE_CCP_4
#endif //#ifndef PERIOD_PWM_GEN_USE_CCP_2
#endif //#ifndef PERIOD_PWM_GEN_USE_CCP_1

//------------------------����CCP1�Ĵ�����ת��------------------------
#ifdef PERIOD_PWM_GEN_USE_CCP_1
#define CCPxCON       CCP1CON       //����
#define CCPRxL        CCPR1L       //��ʱ����8bit
#define CCPRxH        CCPR1H       //��ʱ����2bit
#define TMRS_SHIFT    0            //CCPTMRS��λֵ
#endif
//------------------------����CCP2�Ĵ�����ת��------------------------
#ifdef PERIOD_PWM_GEN_USE_CCP_2
#define CCPxCON       CCP2CON       //����
#define CCPRxL        CCPR2L       //��ʱ����8bit
#define CCPRxH        CCPR2H       //��ʱ����2bit
#define TMRS_SHIFT    2            //CCPTMRS��λֵ
#endif
//------------------------����CCP4�Ĵ�����ת��------------------------
#ifdef PERIOD_PWM_GEN_USE_CCP_4
#define CCPxCON       CCP4CON       //����
#define CCPRxL        CCPR4L       //��ʱ��
#define TMRS_SHIFT    6            //CCPTMRS��λֵ
#endif

/***************************************************************************
                        ��ʱ�����ú�ĺ�ת��
 ****************************************************************************/

//------------------------���ڶ�ʱ��4�Ĵ�����ת��------------------------
#ifndef PERIOD_PWM_GEN_USE_TIMER_2
#ifndef PERIOD_PWM_GEN_USE_TIMER_6
#define TxCON        T4CON          //����

#define PRx          PR4           //��Ƶ
//ѡ��ʱ��4ΪCCP1��CCP2��PWM��ʱ��(0b11��Ч)
#define _SelTimer()  do{CCPTMRS &= ~(3 << TMRS_SHIFT); CCPTMRS |=  2 << TMRS_SHIFT;}while(0)
#endif //#ifndef PERIOD_PWM_GEN_USE_TIMER_2
#endif //#ifndef PERIOD_PWM_GEN_USE_TIMER_6

//------------------------���ڶ�ʱ��2�Ĵ�����ת��------------------------
#ifdef PERIOD_PWM_GEN_USE_TIMER_2
#define TxCON        T2CON       //����
#define PRx          PR2         //��Ƶ
//ѡ��ʱ��2ΪCCP1��CCP2��PWM��ʱ��(0b11��Ч)
#define _SelTimer()  do{CCPTMRS &= ~(3 << TMRS_SHIFT); CCPTMRS |=  1 << TMRS_SHIFT;}while(0)
#endif //#ifndef PERIOD_PWM_GEN_USE_TIMER_2

//------------------------���ڶ�ʱ��6�Ĵ�����ת��------------------------
#ifdef PERIOD_PWM_GEN_USE_TIMER_6
#define TxCON        T6CON       //����
#define PRx          PR6         //��Ƶ
//ѡ��ʱ��4ΪCCP1��CCP2��PWM��ʱ��(0b11��Ч)
#define _SelTimer()  do{CCPTMRS &= ~(3 << TMRS_SHIFT); CCPTMRS |=  3 << TMRS_SHIFT;}while(0)
#endif //#ifndef PERIOD_PWM_GEN_USE_TIMER_6

/***************************************************************************
                          ��غ���ʵ��
 ****************************************************************************/

//------------------------------��ʼ������---------------------------------
//��ʼ����Ĭ��Ϊֹͣ״̬

void PeriodPwmGen_Init(void) {
  TxCON = 0; //�ȹر�
  CCPxCON = 0; //
  _SelTimer();
  CfgPeriodPwmGenIo(); //PWM��Ӧ��������Ϊ���
  PeriodPwmGen_Stop(); //��ֹͣ
}
//---------------------------����PWM����-------------------------------------

static void _StartPWM(void) {
  //���Ƶ�� = _MCU_HZ / ((Ԥ��Ƶֵ(4bit) * (��Ƶ��ֵ(8bit))
  PRx = PERIOD_PWM_GEN_PR; //��ʱ����
  CCPRxH = 0;
  CCPRxL = PERIOD_PWM_GEN_PL; //PWMֵ
  CCPxCON = 0x0f; //1111 = PWM mode, PWMģʽP2A�����ߵ�ƽ��Ч��ռ�ձȸ���λ
  TxCON = PICB_TMR4ON | PERIOD_PWM_GEN_PS;
  CCPxCON |= 0x80; //CCP is enabled
}
//---------------------------ֹͣPWM����-------------------------------------

static void _StopPWM(void) {
  //�رն�ʱ��  
  TxCON = 0;
  CCPxCON = 0;
  ClrPeriodPwmGenIo(); //����Ϊ�͵�ƽ�޵������
}

//----------------------------������-------------------------------------
//����128ms��ʱ����
void PeriodPwmGen_Task(void) {
  if (PeriodPwmGen.ContinuityTimer == 255) return; //�˳�

  if (PeriodPwmGen.PeriodTimer == 0) {//ʱ�䵽��
    PeriodPwmGen.PeriodTimer = PERIOD_PWM_GEN_PERIOD_OV;

    if (PeriodPwmGen.ContinuityTimer) //�����м�ʱ
      PeriodPwmGen.ContinuityTimer--;
    else//�������������,���¿�������
      _StartPWM();
  } else {//��ʱ��
    PeriodPwmGen.PeriodTimer--;
    if ((PeriodPwmGen.ContinuityTimer == 0) && //������ʱ,ֹͣ���ڽ�ֹ���
      (PeriodPwmGen.PeriodTimer == PERIOD_PWM_GEN_PERIOD_DIS))
      _StopPWM();
  }
}

//---------------------------��������-------------------------------------
//��ʼ����������������
void PeriodPwmGen_Start(void) {
  PeriodPwmGen.ContinuityTimer = PERIOD_PWM_GEN_CONTINUITY_COUNT; //������
  PeriodPwmGen.PeriodTimer = PERIOD_PWM_GEN_PERIOD_OV; //�����
  _StartPWM();
}

//-----------------------------ֹͣ����-------------------------------------
//ֹͣ���

void PeriodPwmGen_Stop(void) {
  _StopPWM();
  PeriodPwmGen.ContinuityTimer = 255; //��ʾֹͣ״̬
}