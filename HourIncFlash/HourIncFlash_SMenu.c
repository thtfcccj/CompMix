/******************************************************************************

            在Flash页内实现的小时累加模块-SMenu接口实现

*******************************************************************************/

#include "HourIncFlash_SMenu.h"
#include "HourIncFlash.h"
#include "LedSign.h"

//-----------------------开机工作小时数与小时校准------------------------------
//开机小时数超过9h时，显示"开机小时数"不可校正
//开机小时数<=9h时,显示"小时数.分钟.10秒.计时值(xxx3.6s为单位)
static void _GetHourIncFlash(struct _SMenuUser *pUser, unsigned char Type)
{
  if(HourIncFlash.OnHour > 9){//超过9h时显示不下，显示"开机小时数"不可校正
    pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_RD | SMENU_USR_REAL_RD;
    pUser->Adj = HourIncFlash.OnHour;
  }
  else{//显示小时.分钟.10秒值
    //权限够且允许校正时
    if(SMenuUser_IsMoreAdmin() && HourIncFlash_HaveCalibration())
      pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD;
    else pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_RD | SMENU_USR_REAL_RD;

    pUser->Cfg2 = SMENU_USR2_DOT3 | SMENU_USR2_DOT1;  //负值与小数点
    if(Type == SMENU_LAYER_WR)
      pUser->Adj = 1000;//提示1.00.0表示校正到1h
    else{//显示"小时数.xxx计时值
      pUser->Adj = HourIncFlash.OnHour * 1000; //小时
      unsigned short Sec = HourIncFlash_GetSecInHour();
      pUser->Adj += (Sec / 60) * 10;//分钟
      pUser->Adj += (Sec % 60) / 10;//10秒为单位
    }
  }
}
static void _SetHourIncFlash(struct _SMenuUser *pUser, unsigned char Type)
{
  if(HourIncFlash_HaveCalibration()){
    HourIncFlash_Calibration((pUser->Adj % 1000) * 10);
  }
}

//主结结构
const struct _SMenuFun SiSMenu_HourIncFlash = {
  {LED_SIGN_R, LED_SIGN_U, LED_SIGN_O, LED_SIGN_H,}, //HOUR
  _GetHourIncFlash, _SetHourIncFlash,
};

//-----------------------设备工作天数与复位------------------------------
//超级用户允许将时间复位于0
static void _GetWorkDay(struct _SMenuUser *pUser, unsigned char Type)
{
  if(SMenuUser_IsSuper()) //超级用户允许
    pUser->Cfg = SMENU_USR_ADJ_CMD | SMENU_USR_REAL_RD;//可清零
  else pUser->Cfg = SMENU_USR_ADJ_CMD | SMENU_USR_REAL_RD | SMENU_USR_RD;//只读
  if(Type == SMENU_LAYER_WR) pUser->Adj = 0;//提示归零
  else{
    unsigned long Day = HourIncFlash_GetAddHour(0) / 24;
    if(Day > 9999) pUser->Adj = 9999;//异常超限
    else pUser->Adj = Day;
  } 
}
static void _SetWorkDay(struct _SMenuUser *pUser, unsigned char Type)
{
  HourIncFlash_ResetToHour0();
}

//主结结构
const struct _SMenuFun SiSMenu_WorkDay = {
  {LED_SIGN_Y, LED_SIGN_A, LED_SIGN_D, 0,}, //DAY
  _GetWorkDay, _SetWorkDay,
};













