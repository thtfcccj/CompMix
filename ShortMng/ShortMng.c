/********************************************************************************

                      短路管理模块实现

*******************************************************************************/

#include "ShortMng.h"

/*******************************************************************************
                        相关函数实现
*******************************************************************************/

//----------------------------任务函数------------------------------
//放入1S附近任务中扫描
void ShortMng_Task(struct _ShortMng *pPow, unsigned char Ch) //对应通道
{
  unsigned char State = pPow->Flag & SHORT_MNG_STATE_MASK;
  //闭合状态每个周期都检测
  if(State == SHORT_MNG_STATE_SHUT){
    if(ShortMng_cbIsShort(Ch)){//识别到短路了
      unsigned char ShortCount;
      if(pPow->Timer){//连续短路检测中
        ShortCount = (pPow->Flag >> SHORT_MNG_PROTECT_COUNT_SHIFT);
        if(ShortCount < (SHORT_MNG_PROTECT_COUNT_MASK >> SHORT_MNG_PROTECT_COUNT_SHIFT)) 
          ShortCount++;
      }
      else//首次短路了
        ShortCount = 1;
      pPow->Flag = (ShortCount << SHORT_MNG_PROTECT_COUNT_SHIFT) | 
                    SHORT_MNG_STATE_BREAK;
      ShortMng_cbBreak(Ch);//断开连接
      if(ShortCount >= 7) pPow->Timer = 255;//最长了
      else pPow->Timer = SHORT_MNG_PTC_WAIT + (1 << ShortCount);
      //状态改变通报
      ShortMng_cbStateNotify(Ch, SHORT_MNG_STATE_SHUT, SHORT_MNG_STATE_BREAK);
    }
    else{
      if(pPow->Timer) pPow->Timer--;
      if(pPow->Timer == 0) pPow->Flag &= ~SHORT_MNG_PROTECT_COUNT_MASK;
    }
    return;
  }
  
  //其它状态时间到后处理
  if(pPow->Timer) pPow->Timer--;
  if(pPow->Timer) return; //时间未到
  
  //时间到了
  unsigned char NextState;
  switch(State){
    case SHORT_MNG_STATE_BREAK:  //短路保护结束,开启供电
    case SHORT_MNG_STATE_IDIE:  //开启供电
      ShortMng_cbShut(Ch);
      NextState = SHORT_MNG_STATE_WAIT;
      pPow->Timer = SHORT_MNG_WAIT_OV;
      break;
    case SHORT_MNG_STATE_WAIT:  //已接入，转至正常
      NextState = SHORT_MNG_STATE_SHUT;
      pPow->Timer = SHORT_MNG_NOR_OV;
      break;
    default: 
      NextState = SHORT_MNG_STATE_IDIE; //异常
      break;
  }
  
  //状态改变了，更新并通报
  if(State != NextState){
    pPow->Flag &= ~SHORT_MNG_STATE_MASK;
    pPow->Flag |= NextState;
    ShortMng_cbStateNotify(Ch, State, NextState);
  }
}




