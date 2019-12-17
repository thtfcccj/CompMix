/***************************************************************************

                    周期PWM信号输出产生器-在Pic使用CCP中的实现

 ****************************************************************************/

#include <pic.h>
#include <string.h>
#include "PicBit.h"
#include "IoCtrl.h"

#include "PeriodPwmGen.h"
#include "string_P.h"

struct _PeriodPwmGen PeriodPwmGen;

/***************************************************************************
                            内部配置
 ****************************************************************************/

//定义PWM时钟
#ifndef PERIOD_PWM_GEN_PS //PS值，决定分频系数: 0:1, 1:4, 2:16, 3:64 
#define PERIOD_PWM_GEN_PS  0 
#endif

#ifndef PERIOD_PWM_GEN_PR //PR值，决定细分频率 
#define PERIOD_PWM_GEN_PR  5
#endif

#ifndef PERIOD_PWM_GEN_PL //PL值，决定在PERIOD_PWM_GEN_PR内的占空比
#define PERIOD_PWM_GEN_PL   4
#endif

//定义使用的CCP,无定义为CCP3
//#define PERIOD_PWM_GEN_USE_CCP_1 //使用CCP1时定义
//#define PERIOD_PWM_GEN_USE_CCP_2 //使用CCP1时定义
//#define PERIOD_PWM_GEN_USE_CCP_4 //使用CCP1时定义

//定义使用的定时器,PWM模式可使用(用8位模式)2/4/6定时器中的一个,不定义时使用定时器4
//#define PERIOD_PWM_GEN_USE_TIMER_2 //使用定时器4时定义
//#define PERIOD_PWM_GEN_USE_TIMER_6 //使用定时器6时定义

/***************************************************************************
                        CCP配置后的宏转义
 ****************************************************************************/
//------------------------关于CCP3寄存器的转义------------------------
#ifndef PERIOD_PWM_GEN_USE_CCP_1
#ifndef PERIOD_PWM_GEN_USE_CCP_2
#ifndef PERIOD_PWM_GEN_USE_CCP_4
#define CCPxCON       CCP3CON       //控制
#define CCPRxL        CCPR3L       //定时器低8bit
#define CCPRxH        CCPR3H       //定时器高2bit
#define TMRS_SHIFT    4            //CCPTMRS移位值
#endif //#ifndef PERIOD_PWM_GEN_USE_CCP_4
#endif //#ifndef PERIOD_PWM_GEN_USE_CCP_2
#endif //#ifndef PERIOD_PWM_GEN_USE_CCP_1

//------------------------关于CCP1寄存器的转义------------------------
#ifdef PERIOD_PWM_GEN_USE_CCP_1
#define CCPxCON       CCP1CON       //控制
#define CCPRxL        CCPR1L       //定时器低8bit
#define CCPRxH        CCPR1H       //定时器高2bit
#define TMRS_SHIFT    0            //CCPTMRS移位值
#endif
//------------------------关于CCP2寄存器的转义------------------------
#ifdef PERIOD_PWM_GEN_USE_CCP_2
#define CCPxCON       CCP2CON       //控制
#define CCPRxL        CCPR2L       //定时器低8bit
#define CCPRxH        CCPR2H       //定时器高2bit
#define TMRS_SHIFT    2            //CCPTMRS移位值
#endif
//------------------------关于CCP4寄存器的转义------------------------
#ifdef PERIOD_PWM_GEN_USE_CCP_4
#define CCPxCON       CCP4CON       //控制
#define CCPRxL        CCPR4L       //定时器
#define TMRS_SHIFT    6            //CCPTMRS移位值
#endif

/***************************************************************************
                        定时器配置后的宏转义
 ****************************************************************************/

//------------------------关于定时器4寄存器的转义------------------------
#ifndef PERIOD_PWM_GEN_USE_TIMER_2
#ifndef PERIOD_PWM_GEN_USE_TIMER_6
#define TxCON        T4CON          //控制

#define PRx          PR4           //分频
//选择定时器4为CCP1，CCP2的PWM定时器(0b11无效)
#define _SelTimer()  do{CCPTMRS &= ~(3 << TMRS_SHIFT); CCPTMRS |=  2 << TMRS_SHIFT;}while(0)
#endif //#ifndef PERIOD_PWM_GEN_USE_TIMER_2
#endif //#ifndef PERIOD_PWM_GEN_USE_TIMER_6

//------------------------关于定时器2寄存器的转义------------------------
#ifdef PERIOD_PWM_GEN_USE_TIMER_2
#define TxCON        T2CON       //控制
#define PRx          PR2         //分频
//选择定时器2为CCP1，CCP2的PWM定时器(0b11无效)
#define _SelTimer()  do{CCPTMRS &= ~(3 << TMRS_SHIFT); CCPTMRS |=  1 << TMRS_SHIFT;}while(0)
#endif //#ifndef PERIOD_PWM_GEN_USE_TIMER_2

//------------------------关于定时器6寄存器的转义------------------------
#ifdef PERIOD_PWM_GEN_USE_TIMER_6
#define TxCON        T6CON       //控制
#define PRx          PR6         //分频
//选择定时器4为CCP1，CCP2的PWM定时器(0b11无效)
#define _SelTimer()  do{CCPTMRS &= ~(3 << TMRS_SHIFT); CCPTMRS |=  3 << TMRS_SHIFT;}while(0)
#endif //#ifndef PERIOD_PWM_GEN_USE_TIMER_6

/***************************************************************************
                          相关函数实现
 ****************************************************************************/

//------------------------------初始化函数---------------------------------
//初始化后默认为停止状态

void PeriodPwmGen_Init(void) {
  TxCON = 0; //先关闭
  CCPxCON = 0; //
  _SelTimer();
  CfgPeriodPwmGenIo(); //PWM对应引脚设置为输出
  PeriodPwmGen_Stop(); //先停止
}
//---------------------------启动PWM函数-------------------------------------

static void _StartPWM(void) {
  //输出频率 = _MCU_HZ / ((预分频值(4bit) * (分频器值(8bit))
  PRx = PERIOD_PWM_GEN_PR; //定时总数
  CCPRxH = 0;
  CCPRxL = PERIOD_PWM_GEN_PL; //PWM值
  CCPxCON = 0x0f; //1111 = PWM mode, PWM模式P2A出，高电平有效。占空比高两位
  TxCON = PICB_TMR4ON | PERIOD_PWM_GEN_PS;
  CCPxCON |= 0x80; //CCP is enabled
}
//---------------------------停止PWM函数-------------------------------------

static void _StopPWM(void) {
  //关闭定时器  
  TxCON = 0;
  CCPxCON = 0;
  ClrPeriodPwmGenIo(); //驱动为低电平无电流输出
}

//----------------------------任务函数-------------------------------------
//放在128ms定时器中
void PeriodPwmGen_Task(void) {
  if (PeriodPwmGen.ContinuityTimer == 255) return; //退出

  if (PeriodPwmGen.PeriodTimer == 0) {//时间到了
    PeriodPwmGen.PeriodTimer = PERIOD_PWM_GEN_PERIOD_OV;

    if (PeriodPwmGen.ContinuityTimer) //启动中计时
      PeriodPwmGen.ContinuityTimer--;
    else//周期脉冲产生中,重新开启脉冲
      _StartPWM();
  } else {//计时中
    PeriodPwmGen.PeriodTimer--;
    if ((PeriodPwmGen.ContinuityTimer == 0) && //非连续时,停止周期禁止输出
      (PeriodPwmGen.PeriodTimer == PERIOD_PWM_GEN_PERIOD_DIS))
      _StopPWM();
  }
}

//---------------------------启动函数-------------------------------------
//开始连续输出至周期输出
void PeriodPwmGen_Start(void) {
  PeriodPwmGen.ContinuityTimer = PERIOD_PWM_GEN_CONTINUITY_COUNT; //启动了
  PeriodPwmGen.PeriodTimer = PERIOD_PWM_GEN_PERIOD_OV; //输出中
  _StartPWM();
}

//-----------------------------停止函数-------------------------------------
//停止输出

void PeriodPwmGen_Stop(void) {
  _StopPWM();
  PeriodPwmGen.ContinuityTimer = 255; //表示停止状态
}