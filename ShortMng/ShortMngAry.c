/********************************************************************************

                      短路管理模块阵列实现

*******************************************************************************/
#include "ShortMngAry.h"
#include "IoCtrl.h"

struct _ShortMngAry ShortMngAry;
static unsigned char _CurCh;//当前巡检通道
/*******************************************************************************
                               相关函数
*******************************************************************************/

//--------------------------------初始化-----------------------------
//形参为总的上电延时时间，用于分时上电
void ShortMngAry_Init(unsigned char ShutDelay)
{
  CfgPo();//电源管理IO初始化
  _CurCh = 0;
  for(unsigned char Ch = 0; Ch < SHORT_MNG_ARY_COUNT; Ch++){
    ShortMng_Init(&ShortMngAry.ShortMng[Ch], ShutDelay); 
    ShutDelay++;
  }
}

//-------------------------------置为掉电模式------------------------------
void ShortMngAry_ShutDown()//对应通道  
{
  for(unsigned char Ch = 0; Ch < SHORT_MNG_ARY_COUNT; Ch++){
    ShortMng_ShutDown(&ShortMngAry.ShortMng[Ch], Ch);
  }
}

//------------------------------任务函数------------------------------
//放入128msS附近任务中扫描
void ShortMngAry_Task(void) //对应通道
{
  ShortMng_Task(ShortMngAry_GetMng(_CurCh), _CurCh);
  _CurCh++;
  if(_CurCh >= SHORT_MNG_ARY_COUNT) _CurCh = 0;
}

//------------------------------对应通道是否接入------------------------------
signed char ShortMngAry_IsShut(unsigned char Ch)
{
  unsigned char State =  ShortMng_GetState(ShortMngAry_GetMng(Ch));
  if(ShortMng_IsShut(State)) return 1;
  return 0;
}



