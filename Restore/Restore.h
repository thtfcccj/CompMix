/*******************************************************************************

                           数据恢复模块
此模块为通用模块
*******************************************************************************/

#ifndef __RESTORE_H 
#define __RESTORE_H

/*****************************************************************************
                          相关函数
****************************************************************************/

//----------------------------保存备份点函数----------------------------------
void Restore_SaveBak(void);

//----------------------------从备份点恢复并重启-------------------------------
void Restore_FromBak(void);

//-----------------------------判断系统是否在配置模式---------------------------
unsigned char Restore_IsCfgMode(void);

//-------------------------------置为配置模式并重启-----------------------------
void Restore_SetCfgMode(void);

//-------------------------------取消配置模式并重启-----------------------------
void Restore_ClrCfgMode(void);

#endif




