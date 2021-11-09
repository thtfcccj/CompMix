/******************************************************************************

                          �¶�(������)ģ��-SMenu�ӿ�ʵ��

*******************************************************************************/

#include "Temp_SMenu.h"
#include "Temp.h"
#include "LedSign.h"

//----------------------------�¶���ʾ��궨------------------------------
//����ʱ��ʾ��ǰ�������¶�ֵ, ����궨ģʽ��
//ֱ������Ŀ���¶�ֵ�� Ϊ����ǰ�¶����㵽��ֵ, ������20��ʾ���㵽20��
//����Ŀ���¶�ֵ+ 500��Ϊ����ǰ�¶ȱ궨����ֵ(����궨)  �磺
//   ����460: ��ʾ�궨��-40��
//   ����570: ��ʾ�궨��70��
static void _GetTemp(struct _SMenuUser *pUser, unsigned char Type)
{
  if(SMenuUser_IsMoreAdmin())
    pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD;//�ɱ궨
  else pUser->Cfg = SMENU_USR_ADJ_BIT | SMENU_USR_REAL_RD | SMENU_USR_RD;//ֻ��
  pUser->Cfg2 = SMENU_USR2_NEGATIVE; //��Ϊ��ֵ 
  pUser->Adj = Temp_GetTemp() - TEMP_BASE; 
  pUser->Min = -TEMP_BASE;
  pUser->Max = (500 + 256) - TEMP_BASE; 
}
static void _SetTemp(struct _SMenuUser *pUser, unsigned char Type)
{
  if(pUser->Adj > (500 - 100))//������ֵ
    Temp_Calibration(pUser->Adj - 500 + TEMP_BASE, 1);
  else//�����ֵ
    Temp_Calibration(pUser->Adj + TEMP_BASE, 0);
}

//����ṹ
const struct _SMenuFun SiSMenu_Temp = {
  {LED_SIGN_C, LED_SIGN_P | LED_SIGN_DOT, LED_SIGN_E, LED_SIGN_T,}, //TEP.C
  _GetTemp, _SetTemp,
};












