/*******************************************************************************

                         温度设备模块实现

*******************************************************************************/
#include "TempDev.h"
#include "Eeprom.h"
#include "InfoBase.h"
#include "MemMng.h"
#include <string.h>

/*******************************************************************************
                           相关行为函数实现
*******************************************************************************/

//------------------------------初始化函数-------------------------------------
void TempDev_Init(struct _TempDev *pDev,
                  unsigned char IsInited, //是否已初始化
                  unsigned char Ch)      //分配的通道
{
  memset(pDev, 0, sizeof(struct _TempDev));
  pDev->Ch = Ch;
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(Ch);
  if(!IsInited){//装载默认
    pDesc->InfoToDefault(pDev);
    Eeprom_Wr(TempDev_GetInfoBase(Ch),
              &pDev->Info,
              sizeof(struct _TempDevInfo));
  }
  else{
    Eeprom_Rd(TempDev_GetInfoBase(Ch),
                &pDev->Info,
                sizeof(struct _TempDevInfo));
  }
  
  #ifdef SUPPORT_TEMP_DEV_FILETER //支持滤波时,分配内存
  unsigned char FilterType = pDesc->FilterCfg >> TEMP_DEV_FILETER_TYPE_SHIFT;
  if(FilterType){
    unsigned short BufCapability = pDesc->LutAryCapability * 2;
    #ifdef SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID //排序需增加1倍缓冲
      if(FilterType == TEMP_DEV_FILETER_AVERVGE_MID) BufCapability *= 2;
    #endif
    pDev->pFilterBuf = MemMng_pvMalloc(BufCapability);
    memset(pDev->pFilterBuf, 0, BufCapability);//初始化为无效值
  }
  #endif //SUPPORT_TEMP_DEV_FILETER
  
  //其它非0初始化
  //(无)
}

//---------------------------------更新温度值-----------------------------------
//同期调用以更新温度,建议在非中断中周期调用
void TempDev_Update(struct _TempDev *pDev,
                    unsigned short OrgSignal) //原始信号(为负时需上浮固定值为正)
{
  //========================由原始信号得到温度值============================
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  unsigned short Temp;
  if(OrgSignal >= pDev->Info.Zero){//只处理正常温度
    //原始信号->减零点
    unsigned long Data = OrgSignal - pDev->Info.Zero;
    //->X增益
    Data *= pDev->Info.Gain;
    Temp = Data >> pDesc->GainQ;
    //->(可选)曲线校正
    if(pDesc->pLutAry != NULL){
      Temp = NolinearConvert(Temp, //数据源为当前温度
                             pDesc->pLutAry,  //已上升排序查找表阵列入口
                             0);//自动大小
    }
    //->(可选)附加校正
    if(pDesc->ExVolPro != NULL){
      Temp = pDesc->ExVolPro(pDev, Temp);
      if(Temp == 0) return;//无效值丢弃
    }
  }
  else Temp = 1;//负值为绝对温度，无意义,(0用于表示未初始化了)
  //==========================滤波预处理==============================
  #ifdef SUPPORT_TEMP_DEV_FILETER //支持滤波时
    unsigned short *pFilterBuf = pDev->pFilterBuf;
    //直接结果
    if(pFilterBuf == NULL){
      pDev->CurTemp = Temp;
      return;
    }
    //(缓冲区无效)首次使用时,先用首个有效值填充
    if(*pFilterBuf == 0){
      unsigned short *pEndFilterBuf = pFilterBuf + pDesc->LutAryCapability;
      for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
        *pFilterBuf = Temp;
      return;
    }
    //需滤波处理时，先追加到最后
   if(pDev->FilterPara >= pDesc->LutAryCapability)
      pDev->FilterPara = 0;
    else pDev->FilterPara++;
    *(pFilterBuf + pDev->FilterPara) = Temp;
    pDev->FilterPara = 0; //TEMP_DEV_FILETER_AVERVGE_MID使用
    pDev->Flag |= TEMP_DEV_UPDATE_LATER;//稍后处理
  #else
    pDev->CurTemp = Temp; //直接结果
  #endif
}

//---------------------------稍后更新任务-------------------------------
//放入TempDev_Update()快一倍的进程中调用(建议32ms进程)
#ifdef SUPPORT_TEMP_DEV_FILETER //支持滤波时
void TempDev_UpdateLater(struct _TempDev *pDev)
{
  if(pDev->Flag & TEMP_DEV_UPDATE_LATER) return;//无任务
  
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  unsigned char Capability = pDesc->LutAryCapability;
  unsigned char FilterType = pDesc->FilterCfg >> TEMP_DEV_FILETER_TYPE_SHIFT;
  unsigned short *pFilterBuf = pDev->pFilterBuf;  
  
  #ifdef SUPPORT_TEMP_DEV_FILETER_AVERVGE //简单平均时: 累加出结果
  if(FilterType == TEMP_DEV_FILETER_AVERVGE){
    unsigned short *pEndFilterBuf = pFilterBuf + Capability;
    unsigned long AddTemp = 0;
    for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
      AddTemp += *pFilterBuf;
    pDev->CurTemp = AddTemp / Capability;
    pDev->Flag &= ~TEMP_DEV_UPDATE_LATER;//一次处理完成
    return;
  }
  #endif //SUPPORT_TEMP_DEV_FILETER_AVERVGE
  
  #ifdef SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID  //支持平均但去除最大最小值时
  if(FilterType == TEMP_DEV_FILETER_AVERVGE_MID){
    if(pDev->FilterPara == 0){//首次初始化
      memcpy(pFilterBuf, pFilterBuf + Capability, Capability * 2);
      pDev->FilterPara = Capability;
    }
    //由小到大冒泡排序,因最后Lost个数不用,故其后不用排序 
    pFilterBuf += Capability;//指向排序缓冲    
    unsigned char End = pDev->FilterPara;  //将执行周期分成多份处理
    unsigned char Lost = pDesc->FilterCfg & TEMP_DEV_FILETER_PARA_MASK;//前后丢弃数
    if(End <= Lost){//已结束了,更新至当前温度
      unsigned short *pEndFilterBuf = pFilterBuf + Capability - Lost;      
      pFilterBuf += Lost;
      unsigned long AddTemp = 0;
      for(; pFilterBuf < pEndFilterBuf; pFilterBuf++)
        AddTemp += *pFilterBuf;
      pDev->CurTemp = AddTemp / (Capability - Lost * 2);
      pDev->Flag &= ~TEMP_DEV_UPDATE_LATER;//一次处理完成
      return;
    }
    unsigned char Start = End - ((Capability - Lost * 2) / 4); //4一次任务排序组数
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
    pDev->FilterPara = End; //下次结束位置
  }
  #endif //SUPPORT_TEMP_DEV_FILETER_AVERVGE_MID
}
#endif //SUPPORT_TEMP_DEV_FILETER //支持滤波时

//-------------------------------得到原始温度值---------------------------------
//即由目标温度得到增益前未处理的温度值,为更新温度的反运算，主要用于标定
static unsigned short _GetOrgTemp(struct _TempDev *pDev,
                                  unsigned short TargetTemp) //目标温度
{
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  //->(可选)附加反校正
  if(pDesc->AntiExVolPro != NULL){
    TargetTemp = pDesc->AntiExVolPro(pDev, TargetTemp);
  }
  //->(可选)反曲线校正
  if(pDesc->pLutAry != NULL){
    struct _NolinearConvertTbl *pDeLut = NolinearConvert_pGetBuf();
    NolinearConvert_AntiCopy(pDeLut, //表反向到缓冲区
                             pDesc->pLutAry, 
                             0);
    TargetTemp = NolinearConvert(TargetTemp, //数据源
                                 pDeLut,  //已上升排序查找表阵列入口
                                 0); 
  }
  return TargetTemp;
}

//------------------------由原始温度值得到原始信号值----------------------------
//为更新温度时，增益与零点的反运算，主要用于标定
static unsigned short  _GetOrgSignal(struct _TempDev *pDev,
                                     unsigned short OrgTemp) //原始温度
{
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  //->反X增益
  unsigned long Data = (unsigned long )OrgTemp << pDesc->GainQ;
  Data /= pDev->Info.Gain;
  //原始信号->反减零点得信号
  return Data + pDev->Info.Zero;
}

//----------------------------温度校准处理--------------------------------------
void TempDev_Calibration(struct _TempDev *pDev,
                         unsigned short TargetTemp) //目标浓度值
{
  unsigned short OrgTemp = _GetOrgTemp(pDev, TargetTemp);//当前真实温度对应原温度
  unsigned short CurSignal = _GetOrgSignal(pDev, ///当前真实温度对应采样信号
                                           _GetOrgTemp(pDev, pDev->CurTemp));
  const struct _TempDevDesc *pDesc = TempDev_cbpGetDesc(pDev->Ch);
  //增益标定模式时，需两个点
  if(pDev->Flag &TEMP_DEV_ADJ_MODE){
    if(!(pDev->Flag &TEMP_DEV_ADJ_1ST_FINAL)){//两点记录只有一点
      pDev->PrvAdjTemp = OrgTemp;
      pDev->PrvAdjSignal = CurSignal;
      pDev->Flag |= TEMP_DEV_ADJ_1ST_FINAL;
      return;
    }
    //两点有了，计算增益值;
    unsigned short TempDiff;
    unsigned short PrvAdjSignal = pDev->PrvAdjSignal;
    if(pDev->PrvAdjTemp > OrgTemp){//先高温后低温
      TempDiff = pDev->PrvAdjTemp - OrgTemp;
      if(PrvAdjSignal < CurSignal) return; //用户设置数据有误
      PrvAdjSignal = PrvAdjSignal - CurSignal;
    }
    else{//先低温后高温
      TempDiff =  OrgTemp - pDev->PrvAdjTemp;
      if(CurSignal < PrvAdjSignal) return; //用户设置数据有误
      PrvAdjSignal = CurSignal - PrvAdjSignal;
    }
    if(TempDiff < pDesc->CalibrationTempDiffMin)  return; //温差太小,不能标校
    if(PrvAdjSignal < pDesc->CalibrationOrgSignalDiffMin) return; //原始信号相差太小,不能标校
    //得到增益  
    unsigned long Data = (unsigned long)TempDiff << pDesc->GainQ;
    Data /= PrvAdjSignal;
    if(Data > 0xffff) pDev->Info.Gain = 0xffff; //最大放大倍数了
    else if(Data < 0x3f) pDev->Info.Gain = 0x3f; //最小放大倍数了
    else pDev->Info.Gain = Data;
  }
  //两点标定或单点标定，计算零点值
  unsigned long Data = (unsigned long)OrgTemp << pDesc->GainQ;
  Data /= pDev->Info.Gain;//此为目标温度需要的零点
  pDev->Info.Zero = (signed short)CurSignal - (signed short)Data;
  //替换上次为当前以便下次使用
  pDev->PrvAdjTemp = OrgTemp;
  pDev->PrvAdjSignal = CurSignal;
  //最后保存
  Eeprom_Wr(TempDev_GetInfoBase(pDev->Ch),
              &pDev->Info,
              sizeof(struct _TempDevInfo));
}



