/*******************************************************************************

                         温度设备模块-实现
此实现独立于硬件
*******************************************************************************/
#include "Temp.h"
#include <string.h>

#include "Eeprom.h"
#include "InfoBase.h"

struct _Temp Temp;

/*******************************************************************************
                           相关行为函数实现
*******************************************************************************/

//------------------------------初始化函数-------------------------------------
void Temp_Init(unsigned char IsInited) //是否已初始化
{
  memset(&Temp, 0, sizeof(struct _Temp));
  if(!IsInited){//装载默认
    Temp.Info.Zero = TEMP_DEFAULT_ZERO;
    Temp.Info.Gain = TEMP_DEFAULT_GAIN; 
    Eeprom_Wr(Temp_GetInfoBase(),
              &Temp.Info,
              sizeof(struct _TempInfo));
  }
  else{
    Eeprom_Rd(Temp_GetInfoBase(),
                &Temp.Info,
                sizeof(struct _TempInfo));
  }
}

//---------------------------------更新温度值-----------------------------------
//同期调用以更新温度,建议在非中断中周期调用
void Temp_Update(void) //原始信号(为负时需上浮固定值为正)
{
  unsigned short OrgSignal = Temp_cbGetCurTempSignal();
  //========================由原始信号得到温度值============================
  unsigned char CurTemp;
  if(OrgSignal >= Temp.Info.Zero){//只处理正常温度
    //原始信号->减零点
    unsigned long Data = OrgSignal - Temp.Info.Zero;
    //->X增益
    Data *= Temp.Info.Gain;
    Data >>= (TEMP_GAIN_Q - 8); //Q8数以在附加校正时提高校准精度
    if(Data > 0xffff){//正超限时
      CurTemp = 255;
      Temp.Flag |= TEMP_HI_OV_ERR;
    }
    else{
      CurTemp = Temp_cbAppendPro(Data); //附加校正
      Temp.Flag &= ~(TEMP_HI_OV_ERR | TEMP_LOW_OV_ERR);
    }
  }
  else{//负超限了
    CurTemp = 0;
    Temp.Flag |= TEMP_LOW_OV_ERR;
  }
  //==========================滤波预处理==============================
  #ifdef SUPPORT_TEMP_FILETER //支持滤波时
    unsigned char *pFilterBuf = Temp.FilterBuf;
    //(缓冲区无效)首次使用时,先用首个有效值填充
    if(*pFilterBuf == 0){
      unsigned char *pEndFilterBuf = pFilterBuf + SUPPORT_TEMP_FILETER;
      for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
        *pFilterBuf = CurTemp;
      return;
    }
    //需滤波处理时，先追加到最后
   if(Temp.FilterPos >= SUPPORT_TEMP_FILETER)
      Temp.FilterPos = 0;
    else Temp.FilterPos++;
    *(pFilterBuf + Temp.FilterPos) = CurTemp;
    Temp.FilterPos = 0; //TEMP_FILETER_AVERVGE_MID使用
    Temp.Flag |= TEMP_UPDATE_LATER;//稍后处理
  #else
    Temp.CurTemp = CurTemp; //直接结果
  #endif
}

//---------------------------稍后更新任务-------------------------------
//放入Temp_Update()快一倍的进程中调用(建议32ms进程)
#ifdef SUPPORT_TEMP_FILETER //支持滤波时
void Temp_UpdateLater(void)
{
  if(!(Temp.Flag & TEMP_UPDATE_LATER)) return;//无任务
  
  unsigned char *pFilterBuf = Temp.FilterBuf;  
  
  #ifdef SUPPORT_TEMP_FILETER_AVERVGE //简单平均时: 累加出结果
    unsigned char *pEndFilterBuf = pFilterBuf + SUPPORT_TEMP_FILETER;
    unsigned short AddTemp = 0;
    for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
      AddTemp += *pFilterBuf;
    Temp.CurTemp = AddTemp / SUPPORT_TEMP_FILETER;
    Temp.Flag &= ~TEMP_UPDATE_LATER;//一次处理完成
    return;
  #endif //SUPPORT_TEMP_FILETER_AVERVGE
  
  #ifdef SUPPORT_TEMP_FILETER_AVERVGE_MID  //支持平均但去除最大最小值时
    if(Temp.FilterPos == 0){//首次初始化
      memcpy(pFilterBuf, pFilterBuf + SUPPORT_TEMP_FILETER, SUPPORT_TEMP_FILETER * 2);
      Temp.FilterPos = SUPPORT_TEMP_FILETER;
    }
    //由小到大冒泡排序,因最后Lost个数不用,故其后不用排序 
    pFilterBuf += SUPPORT_TEMP_FILETER;//指向排序缓冲    
    unsigned char End = Temp.FilterPos;  //将执行周期分成多份处理
    unsigned char Lost = SUPPORT_TEMP_FILETER_AVERVGE_MID;//前后丢弃数
    if(End <= Lost){//已结束了,更新至当前温度
      unsigned char *pEndFilterBuf = pFilterBuf + SUPPORT_TEMP_FILETER - Lost;      
      pFilterBuf += Lost;
      unsigned short AddTemp = 0;
      for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
        AddTemp += *pFilterBuf;
      Temp.CurTemp = AddTemp / (SUPPORT_TEMP_FILETER - Lost * 2);
      Temp.Flag &= ~TEMP_UPDATE_LATER;//一次处理完成
      return;
    }
    unsigned char Start = End - ((SUPPORT_TEMP_FILETER - Lost * 2) / 4); //4一次任务排序组数
    if(Start < Lost) Start = Lost;  
    //由后向前，找到当前最大值后移到最后面
    for(; End >= Start; End--){
      unsigned char MaxPos = 0;       //记住本次最大值及其位置
      unsigned char Max = *pFilterBuf;    
      for(unsigned char i = 1; i < End; i++){
        unsigned char Cur = *(pFilterBuf + i); 
        if(Cur > Max){//比最大值大了,重新记住最大值及其位置
           Max = Cur;
           MaxPos = i;
        }
      }
      //将最后一个值与最大值位置数交换
      *(pFilterBuf + MaxPos) = *(pFilterBuf + End - 1);
      *(pFilterBuf + End - 1) = Max;
    }
    Temp.FilterPos = End; //下次结束位置
  #endif //SUPPORT_TEMP_FILETER_AVERVGE_MID
}
#endif //SUPPORT_TEMP_FILETER //支持滤波时

//------------------------由原始温度值得到原始信号值----------------------------
//为更新温度时，增益与零点的反运算，主要用于标定
static unsigned char  _GetOrgSignal(unsigned char OrgTemp) //原始温度
{
  //->反X增益
  unsigned long Data = (unsigned long )OrgTemp << TEMP_GAIN_Q;
  Data /= Temp.Info.Gain;
  //原始信号->反减零点得信号
  return Data + Temp.Info.Zero;
}

//----------------------------温度校准处理--------------------------------------
//0标定成功，否则失败!
signed char Temp_Calibration(unsigned char TargetTemp, //目标浓度值
                              unsigned char IsGain)//标增益(需提前标零点)，否则标零点
{
  //两点标定时，只有一点
  if(IsGain && !(Temp.Flag &TEMP_ADJ_1ST_FINAL)) return -1; 
  unsigned char OrgTemp = Temp_cbAntiAppendPro(TargetTemp);//当前真实温度对应原温度
  unsigned short CurSignal;
  if(Temp.Flag & (TEMP_LOW_OV_ERR | TEMP_HI_OV_ERR))//以稳定的温度反AD为准
    CurSignal = _GetOrgSignal(Temp_cbAntiAppendPro(Temp.CurTemp));
  else CurSignal = Temp_cbGetCurTempSignal();//以当前AD为准(AD不稳时，可能要标两次)

  if(IsGain){ //增益标定模式时，两个点计算增益值
    unsigned short TempDiff;
    unsigned short PrvAdjSignal = Temp.PrvAdjSignal;
    if(Temp.PrvAdjTemp > OrgTemp){//先高温后低温
      TempDiff = Temp.PrvAdjTemp - OrgTemp;
      if(PrvAdjSignal < CurSignal) return -2; //用户设置数据有误
      PrvAdjSignal = PrvAdjSignal - CurSignal;
    }
    else{//先低温后高温
      TempDiff =  OrgTemp - Temp.PrvAdjTemp;
      if(CurSignal < PrvAdjSignal) return -3; //用户设置数据有误
      PrvAdjSignal = CurSignal - PrvAdjSignal;
    }
    if(TempDiff < TEMP_DIFF_TEPM_MIN)  return - 4; //温差太小,不能标校
    if(PrvAdjSignal < TEMP_DIFF_SIGAL_MIN) return -5; //原始信号相差太小,不能标校
    
    //得到增益  
    unsigned long Data = (unsigned long)TempDiff << TEMP_GAIN_Q;
    Data /= PrvAdjSignal;
    if(Data > 0xffff) Temp.Info.Gain = 0xffff; //最大放大倍数了
    else if(Data < 0x3f) Temp.Info.Gain = 0x3f; //最小放大倍数了
    else Temp.Info.Gain = Data;
  }
  //两点标定或单点标定，计算零点值
  unsigned long Data = (unsigned long)OrgTemp << TEMP_GAIN_Q;
  Data /= Temp.Info.Gain;//此为目标温度需要的零点
  Temp.Info.Zero = (signed short)CurSignal - (signed short)Data;
  //替换上次为当前以便下次使用
  Temp.PrvAdjTemp = OrgTemp;
  Temp.PrvAdjSignal = CurSignal;
  Temp.Flag |= TEMP_ADJ_1ST_FINAL; //只要成功，就有一个点了  
  //最后保存
  Eeprom_Wr(Temp_GetInfoBase(),
              &Temp.Info,
              sizeof(struct _TempInfo));
  return 0;
}





