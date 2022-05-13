/******************************************************************************

                ѹ��ʱ��ģ��-SMenu�����ӿ�ʵ��

*******************************************************************************/

#include "ZipTime_SMenu.h"
#include "LedSign.h"

#include "ZipTime.h"
#include "RTC.h" //������

//----------------------------���޸�--------------------------------
static void _GetYear(struct _SMenuUser *pUser, unsigned char Type)
{
  pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD; //ʵʱ����
  pUser->Adj = ZipTime_cbGetStartYear() + ZipTime_GetInfo(0, ZipTime_GetRTC());
  pUser->Min = ZipTime_cbGetStartYear();
  pUser->Max = ZipTime_cbGetStartYear() + ZIP_TIME_YEAR_MAX;  
}
static void _SetYear(struct _SMenuUser *pUser, unsigned char Type)
{
  RTC_SetInfo(0,  pUser->Adj - 2000);
}

//����ṹ
const struct _SMenuFun SMenu_ZipTime_Year = {
  {LED_SIGN_N, LED_SIGN_A,LED_SIGN_I,LED_SIGN_N}, //NIAN
   _GetYear, _SetYear,
};

//----------------------------���޸�--------------------------------
static void _GetMouth(struct _SMenuUser *pUser, unsigned char Type)
{
  pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD; //ʵʱ����
  pUser->Adj = ZipTime_GetInfo(1, ZipTime_GetRTC());
  pUser->Min = 1;
  pUser->Max = 12;  
  
}
static void _SetMouth(struct _SMenuUser *pUser, unsigned char Type)
{
  RTC_SetInfo(1,  pUser->Adj);
}

//����ṹ
const struct _SMenuFun SMenu_ZipTime_Mouth = {
  {0, LED_SIGN_E,LED_SIGN_U,LED_SIGN_Y}, //YUE
   _GetMouth, _SetMouth,
};

//----------------------------���޸�--------------------------------
static void _GetDay(struct _SMenuUser *pUser, unsigned char Type)
{
  pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD; //ʵʱ����
  pUser->Adj = ZipTime_GetInfo(2, ZipTime_GetRTC());
  //pUser->Min = 0;
  pUser->Max = 31;
}

static void _SetDay(struct _SMenuUser *pUser, unsigned char Type)
{
  RTC_SetInfo(2,  pUser->Adj);
}

//����ṹ
const struct _SMenuFun SMenu_ZipTime_Day = {
  {0, 0,LED_SIGN_1, LED_SIGN_R}, //RI
   _GetDay, _SetDay,
};

//----------------------------ʱ�޸�--------------------------------
static void _GetHour(struct _SMenuUser *pUser, unsigned char Type)
{
  pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD; //ʵʱ����
  pUser->Adj = ZipTime_GetInfo(3, ZipTime_GetRTC());
  //pUser->Min = 0;
  pUser->Max = 23;  
  
}
static void _SetHour(struct _SMenuUser *pUser, unsigned char Type)
{
  RTC_SetInfo(3,  pUser->Adj);
}

//����ṹ
const struct _SMenuFun SMenu_ZipTime_Hour = {
  {0, LED_SIGN_I,LED_SIGN_H,LED_SIGN_S}, //SHI
   _GetHour, _SetHour,
};

//---------------------------���޸�--------------------------------
//ͬʱ�����
static void _GetMinte(struct _SMenuUser *pUser, unsigned char Type)
{
  pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD; //ʵʱ����
  pUser->Adj = ZipTime_GetInfo(4, ZipTime_GetRTC());
  //pUser->Min = 0;
  pUser->Max = 59;  
  
}
static void _SetMinte(struct _SMenuUser *pUser, unsigned char Type)
{
  RTC_SetInfo(4+2,  pUser->Adj);//ͬʱ����� 
}

//����ṹ
const struct _SMenuFun SMenu_ZipTime_Minte = {
  {0, LED_SIGN_N,LED_SIGN_E,LED_SIGN_F}, //FEN
   _GetMinte, _SetMinte,
};






