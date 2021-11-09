/******************************************************************************

                          温度(单例化)模块-SMenu接口实现

*******************************************************************************/

#include "Temp_SMenu.h"
#include "Temp.h"
#include "LedSign.h"

//----------------------------温度显示与标定------------------------------
//正常时显示当前带符号温度值, 进入标定模式后：
//直接输入目标温度值： 为将当前温度清零到此值, 如输入20表示清零到20度
//输入目标温度值+ 500：为将当前温度标定到此值(两点标定)  如：
//   输入460: 表示标定至-40度
//   输入570: 表示标定至70度
static void _GetTemp(struct _SMenuUser *pUser, unsigned char Type)
{
  if(SMenuUser_IsMoreAdmin())
    pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD;//可标定
  else pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD | SMENU_USR_RD;//只读
  pUser->Cfg2 = SMENU_USR2_NEGATIVE; //可为负值 
  pUser->Adj = Temp_GetTemp() - TEMP_BASE; 
  pUser->Min = -TEMP_BASE;
  pUser->Max = (500 + 256) - TEMP_BASE; 
}
static void _SetTemp(struct _SMenuUser *pUser, unsigned char Type)
{
  if(pUser->Adj > (500 - 100))//标增益值
    Temp_Calibration(pUser->Adj - 500 + TEMP_BASE, 1);
  else//标零点值
    Temp_Calibration(pUser->Adj + TEMP_BASE, 0);
}

//主结结构
const struct _SMenuFun SiSMenu_Temp = {
  {LED_SIGN_C, LED_SIGN_P | LED_SIGN_DOT, LED_SIGN_E, LED_SIGN_T,}, //TEP.C
  _GetTemp, _SetTemp,
};












