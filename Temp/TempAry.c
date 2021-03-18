/*******************************************************************************

                         温度设备-多例化实现
多例化时可使用此子模块实例化
*******************************************************************************/
#include "TempAry.h"

struct _TempDev TempDevAry[TEMP_ARY_COUNT];


/*******************************************************************************
                           相关行为函数
*******************************************************************************/

//------------------------------初始化函数-------------------------------------
void TempAry_Init(unsigned char IsInited) //是否已初始化
{
  for(unsigned char Ch = 0; Ch < TEMP_ARY_COUNT; Ch++){
    TempDev_Init(&TempDevAry[Ch], IsInited, Ch);
  }
}


//---------------------------------更新温度值-----------------------------------
//周期调用以更新温度,建议在非中断中周期调用
void TempAry_Update(void)
{
  for(unsigned char Ch = 0; Ch < TEMP_ARY_COUNT; Ch++){
    TempDev_Update(&TempDevAry[Ch], TempAry_cbGetOrgSignal(Ch));
  }
}

//---------------------------稍后更新任务-------------------------------
//放入TempAry_Update()快两倍的进程中调用(建议32ms进程)
#ifdef SUPPORT_TEMP_DEV_FILETER //支持滤波时
void TempAry_UpdateLater(void)
{
  for(unsigned char Ch = 0; Ch < TEMP_ARY_COUNT; Ch++){
    TempDev_UpdateLater(&TempDevAry[Ch]);
  }
}
#endif




