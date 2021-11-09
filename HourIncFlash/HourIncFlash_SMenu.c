/******************************************************************************

            ��Flashҳ��ʵ�ֵ�Сʱ�ۼ�ģ��-SMenu�ӿ�ʵ��

*******************************************************************************/

#include "HourIncFlash_SMenu.h"
#include "HourIncFlash.h"
#include "LedSign.h"

//-----------------------��������Сʱ����СʱУ׼------------------------------
//����Сʱ������9hʱ����ʾ"����Сʱ��"����У��
//����Сʱ��<=9hʱ,��ʾ"Сʱ��.����.10��.��ʱֵ(xxx3.6sΪ��λ)
static void _GetHourIncFlash(struct _SMenuUser *pUser, unsigned char Type)
{
  if(HourIncFlash.OnHour > 9){//����9hʱ��ʾ���£���ʾ"����Сʱ��"����У��
    pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_RD | SMENU_USR_REAL_RD;
    pUser->Adj = HourIncFlash.OnHour;
  }
  else{//��ʾСʱ.����.10��ֵ
    //Ȩ�޹�������У��ʱ
    if(SMenuUser_IsMoreAdmin() && HourIncFlash_HaveCalibration())
      pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD;
    else pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_RD | SMENU_USR_REAL_RD;

    pUser->Cfg2 = SMENU_USR2_DOT3 | SMENU_USR2_DOT1;  //��ֵ��С����
    if(Type == SMENU_LAYER_WR)
      pUser->Adj = 1000;//��ʾ1.00.0��ʾУ����1h
    else{//��ʾ"Сʱ��.xxx��ʱֵ
      pUser->Adj = HourIncFlash.OnHour * 1000; //Сʱ
      unsigned short Sec = HourIncFlash_GetSecInHour();
      pUser->Adj += (Sec / 60) * 10;//����
      pUser->Adj += (Sec % 60) / 10;//10��Ϊ��λ
    }
  }
}
static void _SetHourIncFlash(struct _SMenuUser *pUser, unsigned char Type)
{
  if(HourIncFlash_HaveCalibration()){
    HourIncFlash_Calibration((pUser->Adj % 1000) * 10);
  }
}

//����ṹ
const struct _SMenuFun SiSMenu_HourIncFlash = {
  {LED_SIGN_R, LED_SIGN_U, LED_SIGN_O, LED_SIGN_H,}, //HOUR
  _GetHourIncFlash, _SetHourIncFlash,
};

//-----------------------�豸���������븴λ------------------------------
//�����û�����ʱ�临λ��0
static void _GetWorkDay(struct _SMenuUser *pUser, unsigned char Type)
{
  if(SMenuUser_IsSuper()) //�����û�����
    pUser->Cfg = SMENU_USR_ADJ_CMD | SMENU_USR_REAL_RD;//������
  else pUser->Cfg = SMENU_USR_ADJ_CMD | SMENU_USR_REAL_RD | SMENU_USR_RD;//ֻ��
  if(Type == SMENU_LAYER_WR) pUser->Adj = 0;//��ʾ����
  else{
    unsigned long Day = HourIncFlash_GetAddHour(0) / 24;
    if(Day > 9999) pUser->Adj = 9999;//�쳣����
    else pUser->Adj = Day;
  } 
}
static void _SetWorkDay(struct _SMenuUser *pUser, unsigned char Type)
{
  HourIncFlash_ResetToHour0();
}

//����ṹ
const struct _SMenuFun SiSMenu_WorkDay = {
  {LED_SIGN_Y, LED_SIGN_A, LED_SIGN_D, 0,}, //DAY
  _GetWorkDay, _SetWorkDay,
};













