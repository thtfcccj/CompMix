/*******************************************************************************

                           ���ݻָ�ģ��
��ģ��Ϊͨ��ģ��
*******************************************************************************/

#include "Eeprom.h"
#include "InfoBase.h"
#include "IoCtrl.h"
#include "Restore.h"

/*****************************************************************************
                          ��غ���ʵ��
****************************************************************************/

//----------------------------���汸�ݵ㺯��----------------------------------
void Restore_SaveBak(void)
{
  Eeprom_Wr(Bak_GetBakInfoBase(), 
            (const InfoBaseL *)(pGetRdPoint(Bak_GetOrgInfoBase(), INFO_BASE_BAK_COUNT)),
            INFO_BASE_BAK_COUNT);
}

//----------------------------�ӱ��ݵ�ָ�������-------------------------------
void Restore_FromBak(void)
{
  Eeprom_Wr(Bak_GetOrgInfoBase(), 
            (const InfoBaseL *)(pGetRdPoint(Bak_GetBakInfoBase(), INFO_BASE_BAK_COUNT)),
            INFO_BASE_BAK_COUNT);
  Eeprom_ForceWrBufAndRestart();//ǿ�ƻ�д������
}

//-----------------------------�ж�ϵͳ�Ƿ�������ģʽ---------------------------
unsigned char Restore_IsCfgMode(void)
{
  return SysFlag & SYS_FLAG_CFG_MODE;
}

//-------------------------------��Ϊ����ģʽ������-----------------------------
void Restore_SetCfgMode(void)
{
  //SysFlag |= SYS_FLAG_CFG_MODE;//��Ϊ����ģʽ
  unsigned char InitedFlag = 0x55; //����ģʽ��־
  Eeprom_Wr(InfoBase_GetInitedFlagBase(), &InitedFlag, 1);
  Eeprom_ForceWrBufAndRestart();//ǿ�ƻ�д������
}

//-------------------------------ȡ������ģʽ������-----------------------------
void Restore_ClrCfgMode(void)
{
  //SysFlag &= ~SYS_FLAG_CFG_MODE;  //��������ģʽ
  unsigned char InitedFlag = RESTORE_INITED_FLAG; //����ģʽ��־  
  Eeprom_Wr(InfoBase_GetInitedFlagBase(), &InitedFlag, 1);
  Eeprom_ForceWrBufAndRestart();//ǿ�ƻ�д������
}
