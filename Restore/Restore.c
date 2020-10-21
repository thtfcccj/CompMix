/*******************************************************************************

                           数据恢复模块
此模块为通用模块
*******************************************************************************/

#include "Eeprom.h"
#include "InfoBase.h"
#include "IoCtrl.h"
#include "Restore.h"

/*****************************************************************************
                          相关函数实现
****************************************************************************/

//----------------------------保存备份点函数----------------------------------
void Restore_SaveBak(void)
{
  Eeprom_Wr(Bak_GetBakInfoBase(), 
            (const InfoBaseL *)(pGetRdPoint(Bak_GetOrgInfoBase(), INFO_BASE_BAK_COUNT)),
            INFO_BASE_BAK_COUNT);
}

//----------------------------从备份点恢复并重启-------------------------------
void Restore_FromBak(void)
{
  Eeprom_Wr(Bak_GetOrgInfoBase(), 
            (const InfoBaseL *)(pGetRdPoint(Bak_GetBakInfoBase(), INFO_BASE_BAK_COUNT)),
            INFO_BASE_BAK_COUNT);
  Eeprom_ForceWrBufAndRestart();//强制回写并重启
}

//-----------------------------判断系统是否在配置模式---------------------------
unsigned char Restore_IsCfgMode(void)
{
  return SysFlag & SYS_FLAG_CFG_MODE;
}

//-------------------------------置为配置模式并重启-----------------------------
void Restore_SetCfgMode(void)
{
  //SysFlag |= SYS_FLAG_CFG_MODE;//置为配置模式
  unsigned char InitedFlag = 0x55; //配置模式标志
  Eeprom_Wr(InfoBase_GetInitedFlagBase(), &InitedFlag, 1);
  Eeprom_ForceWrBufAndRestart();//强制回写并重启
}

//-------------------------------取消配置模式并重启-----------------------------
void Restore_ClrCfgMode(void)
{
  //SysFlag &= ~SYS_FLAG_CFG_MODE;  //正常工作模式
  unsigned char InitedFlag = RESTORE_INITED_FLAG; //正常模式标志  
  Eeprom_Wr(InfoBase_GetInitedFlagBase(), &InitedFlag, 1);
  Eeprom_ForceWrBufAndRestart();//强制回写并重启
}
