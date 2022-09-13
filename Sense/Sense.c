/*******************************************************************************

                         感知端信号处理模块实现

*******************************************************************************/
#include "Sense.h"
#include <string.h>

#include "Eeprom.h"
#include "math_3.h"

struct _Sense Sense;

/*******************************************************************************
                           相关函数实现
*******************************************************************************/

//------------------------------初始化函数-------------------------------------
void Sense_Init(struct _Sense *pSense,
                const struct _SenseDesc *pDesc,//静态描述结构
                unsigned char IsInited) //是否已初始化
{
  memset(pSense, 0, sizeof(struct _Sense));
  pSense->pDesc = pDesc;
  if(!IsInited){//装载默认
    pSense->Info.Zero = pDesc->DefaultZero;
    pSense->Info.Gain = pDesc->DefaultGain; 
    Eeprom_Wr(pDesc->InfoBase,
              &pSense->Info,
              sizeof(struct _SenseInfo));
  }
  else{
    Eeprom_Rd(pDesc->InfoBase,
                &pSense->Info,
                sizeof(struct _SenseInfo));
  }
}

//------------------------------更新滤波算法--------------------------------
//可在初始化后调用
void Sense_UpdateFilter(struct _Sense *pSense,
                        unsigned char FilterType, //滤波算法类型,见定义
                        signed short *pFilterBuf, //滤波缓冲区
                        unsigned short FilterSize,//滤波缓冲个数
                        unsigned short FilterPara)//不同滤波算法时，带入的参数
{
  if(FilterType >= SENSE_FILTER_TYPE_COUNT) FilterType = 0;
  pSense->FilterType = FilterType;
  pSense->pFilterBuf = pFilterBuf;
  pSense->FilterSize = FilterSize;
  pSense->FilterPara = FilterPara; 
  pSense->FilterNewPos = 0;    //从0开始了
  pSense->Flag &= ~SENSE_VOL_VALID;//数据无效
}

//-----------------------------更新目标值-----------------------------------
//周期调用以更新温度,
//若采用了去极值等需要排序的滤波算法时，应在非中断中调用
void Sense_Update(struct _Sense *pSense,
                  signed short OrgSignal) //原始信号(上升沿)
{
  const struct _SenseDesc *pDesc = pSense->pDesc;
  //得浓度值，转换为正后参与运算
  signed short CurVol = Line_GetYInZeroMutiSS(pSense->Info.Zero + 32767, //直线穿过y轴的x值
                                     pSense->Info.Gain,//斜率
                                     pDesc->GainQ,//斜率的Q值
                                     OrgSignal + 32767);  //未知y点的x坐标
  if(pDesc->NlLut != NULL) //线性转换
    CurVol = pDesc->NlLut(CurVol);
  
  //无滤波, 直接得结果
  if(pSense->FilterType == SENSE_FILTER_TYPE_NONE){
    pSense->Vol = CurVol;
    return;
  }
  signed short *pFilterBuf = pSense->pFilterBuf;  
  unsigned short FilterSize = pSense->FilterSize;
  //数据无效时，用其初始化
  if(!(pSense->Flag & SENSE_VOL_VALID)){
    memset2((unsigned short*)pFilterBuf, 
            (unsigned short)CurVol, 
            FilterSize);
    pSense->Flag |= SENSE_VOL_VALID;
    pSense->Vol = CurVol;
    return;
  }
  //需滤波处理时，数据有效先追加到最后
  pSense->FilterNewPos++;
  if(pSense->FilterNewPos >= FilterSize)
    pSense->FilterNewPos = 0;
  *(pFilterBuf + pSense->FilterNewPos) = CurVol;
  //滤波处理->平均
  //if(pSense->FilterType == SENSE_FILTER_TYPE_AVERVGE){
    pSense->Vol = AverageS2(pFilterBuf, FilterSize);
    return;
  //}
}

//----------------------------校准处理--------------------------------------
//0标定成功，否则失败!
signed char Sense_Calibration(struct _Sense *pSense,
                              signed short TargetVol, //目标浓度值
                              unsigned char IsGain)//标增益(需提前标零点)，否则标零点
{
  //当前数值有误时
  if(pSense->Flag & (SENSE_LOW_OV_ERR | SENSE_HI_OV_ERR)) return -1;  
  //两点标定时，只有一点
  if(IsGain && !(pSense->Flag & SENSE_ADJ_1ST_FINAL)) return -1; 
 
  const struct _SenseDesc *pDesc = pSense->pDesc;  
  //以目标浓度值，反向计算源信号值(直接用信号源，可能值稳)
  signed short OrgData;
  if(pDesc->AntiNlLut != NULL) //反线性转换
    OrgData = pDesc->AntiNlLut(pSense->Vol);
  else OrgData = pSense->Vol;
  
  OrgData = Line_GetXInZeroMutiU(pSense->Info.Zero, //直线穿过y轴的x值
                                 pSense->Info.Gain,//斜率
                                 pDesc->GainQ,//斜率的Q值
                                 OrgData);  //未知x点的y坐标
  
  //增益标定模式时，两个点计算增益值
  if(IsGain){
    unsigned short Diff;//检查目标差，相近会影响精度，禁止标定。
    if(pSense->PrvVol >= TargetVol) Diff = pSense->PrvVol - TargetVol;
    else Diff =  TargetVol - pSense->PrvVol;
    if(Diff < pDesc->DiffAdjVol) return -1;
    
    unsigned short Gain = Line_GetMuti(pSense->PrvSignal + 32767,  //点0的x坐标
                            pSense->PrvVol + 32767,  //点0的y坐标
                            OrgData +  32767,  //点1的x坐标
                            TargetVol + 32767,  //点1的y坐标                      
                            pDesc->GainQ); //斜率的Q值
    if(Gain <= 0) return -1;//异常
    pSense->Info.Gain = Gain;
  }
  //两点标定或单点标定，计算零点值
  pSense->Info.Zero = Line_GetXzInZeroMutiU(OrgData,  //x值
                                            pSense->Info.Gain,//斜率
                                            pDesc->GainQ,//斜率的Q值
                                            TargetVol);  //x对应的点y坐标
  
  //替换上次为当前以便下次使用
  pSense->PrvSignal = OrgData;
  pSense->PrvVol = TargetVol;
  pSense->Flag |= SENSE_ADJ_1ST_FINAL; //只要成功，就有一个点了  
  //最后保存
  Eeprom_Wr(pDesc->InfoBase,
            &pSense->Info,
            sizeof(struct _SenseInfo));
  //标定成功后，将目标浓度值给与当前以立即显示更新结果
  pSense->Vol = TargetVol;
  memset2((unsigned short*)pSense->pFilterBuf, 
          (unsigned short)TargetVol, 
          pSense->FilterSize);
  return 0;
}

//--------------------------------得到在范围目标值------------------------------
signed short Sense_GetVolInScope(const struct _Sense *pSense)
{
  const struct _SenseDesc *pDesc = pSense->pDesc;
  signed short Vol = pSense->Vol;
  if(Vol < pDesc->Min) return pDesc->Min;
  if(Vol > pDesc->Max) return pDesc->Max;
  return Vol;
}




