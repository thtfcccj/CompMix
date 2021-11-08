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
  if(!HourIncFlash_HaveCalibration() || 
    (HourIncFlash.OnHour > 9)){//����Сʱ������9hʱ����ʾ"����Сʱ��"����У��
    pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_RD| SMENU_USR_REAL_RD;
    pUser->Adj = HourIncFlash.OnHour;
  }
  else{//����У��,��ʾ0.xxx��1.xxx
    pUser->Cfg = SMENU_USR_ADJ_CMD | SMENU_USR_REAL_RD;
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












