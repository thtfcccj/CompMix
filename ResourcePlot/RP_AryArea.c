/***********************************************************************

                 阵列区域绘图模块实现
本模块独立于应用
***********************************************************************/

#include "RP_AryArea.h"
#include "ePic.h"
#include "Plot.h"
#include "RP_FixArea.h"
#include <string.h>

/***********************************************************************
                             相关函数实现
***********************************************************************/

//---------------------------绘制项数据------------------------------
static void _PotItem(struct _RP_AryArea *pArea,
                      unsigned long Handle,
                      unsigned short x,
                      unsigned short y,
                      unsigned char Pos,//本页更新位置
                      unsigned char PlotMask)
{
  pArea->FDesc.Rect.x = x;
  pArea->FDesc.Rect.y = y;
  pArea->FDesc.IconBase = pArea->STypeInfo.IconBase;
  pArea->FDesc.IconCount = pArea->STypeInfo.IconCount;
  pArea->FDesc.ParaBase = pArea->STypeInfo.ParaBase;
  pArea->FDesc.ParaCount = pArea->STypeInfo.ParaCount;
  //调用固定区域绘图
  const struct _RpAryAreaDesc *pAryDesc = pArea->pAryDesc;   //项描述
  if(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN){//背景图变化焦点时替换背景
    if(Pos == pArea->Focus) pArea->FDesc.cBg = pAryDesc->cFocus;
    else pArea->FDesc.cBg = pAryDesc->cBg;
  }
  RP_FixArea(Handle, PlotMask, &pArea->FDesc);
}

//--------------------------更新当前项数据------------------------------
//返回0结束,否则未结束
static signed char  _UpdateSTypeInfo(struct _RP_AryArea *pArea,
                                       unsigned long Handle,
                                       unsigned short AryId)
{
  signed char Resumme = RP_AryArea_cbGetSTypeInfo(Handle, AryId,
                                                  &pArea->STypeInfo);
  //结束了
  if(!Resumme){
    pArea->AryCount = AryId;//动态更新
  }
  return Resumme;
}

//-------------------------------刷新界面------------------------------
static void _Refresh(struct _RP_AryArea *pArea,
                      signed char IsRefreshFocus)//刷新焦点，否则为PlotMask
{
  unsigned char PlotMask;
  if(IsRefreshFocus) PlotMask = 0x0F | 0x10; //刷新焦点位置
  else{
    PlotMask = pArea->PlotMask;
    pArea->PlotMask = 0;
    PlotMask &= ~0x10;//防止异常置位
  }
  const struct _RpMainAreaDesc *pMainDesc = pArea->pMainDesc; //整体描述
  unsigned short x = pMainDesc->Rect.x;
  unsigned short y = pMainDesc->Rect.y;  
  if(PlotMask & 0x80){//绘制整体背景
    Plot_SetBrushColor(pMainDesc->cBg);
    Plot_FullRect(x, y,pMainDesc->Rect.w,pMainDesc->Rect.h);
    PlotMask = 0x0f;//需所有
  }
  
  const struct _RpAryAreaDesc *pAryDesc = pArea->pAryDesc;   //项描述
  unsigned char RowCount = pAryDesc->RowCount;
  //得到项宽度与项x起始
  unsigned short pw = pAryDesc->Rect.w; //所占宽度
  x += pAryDesc->Rect.x; //x起始
  
  //得到项高度与项y起始
  unsigned short ph = pAryDesc->Rect.h; //所占高度
  y += pAryDesc->Rect.y; //y起始  
  
  //初始化FDesc本次固定部分
  pArea->FDesc.Rect.w = pAryDesc->Rect.w;
  pArea->FDesc.Rect.h = pAryDesc->Rect.h;
  pArea->FDesc.cBg = pAryDesc->cBg;
  pArea->FDesc.IconBgId = pAryDesc->IconBgId;
  //初始化RP_FixArea需要的PlotMask与Handle;
  unsigned long Handle = pArea->Handle;
  if(PlotMask == 0){//仅刷新实时部分时
    Handle |= RP_HANDLE_REAL_REFRESH;
    PlotMask = 0x0C; //仅更新项数据与项图
  }
  if(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN)
    PlotMask &= ~0x01;//若为背景图片，则用背景图片代替背景图
  
  unsigned short AryId = pArea->AryStart;
  unsigned short FocusAryId; //被选Aryid
  if(pAryDesc->BitInfo & RP_ITEM_DIS_FOCUS) FocusAryId = 0xffff;//无焦点
  else FocusAryId = AryId + pArea->Focus;
  unsigned char Pos;
  //========================单独更新焦点位置======================
  if(IsRefreshFocus){
    if(pAryDesc->BitInfo & RP_ITEM_DIS_FOCUS) return;//无焦点
    //更新失败时表示结束了，恢复
    if(!_UpdateSTypeInfo(pArea, Handle, FocusAryId)){
      pArea->Focus = pArea->PrvFocus;
      return;
    }
    Pos = pArea->Focus;
    //边框式焦点时
    if(!(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN)){
      Plot_SetPenColor(pAryDesc->cFocus);
      Plot_Rect(x + pw * (Pos % RowCount) - 1, 
                y + ph * (Pos / RowCount) - 1,
                pArea->FDesc.Rect.w + 2,
                pArea->FDesc.Rect.h + 2);
      if(Pos == pArea->PrvFocus) return; //不更新上一个
      //更新本页上次的焦点区域数据
      Pos = pArea->PrvFocus;
      pArea->PrvFocus = pArea->Focus; //更新了
      Plot_SetPenColor(pAryDesc->cBg); //用背景色清除
      Plot_Rect(x + pw * (Pos % RowCount) - 1, 
                y + ph * (Pos / RowCount) - 1,
                pArea->FDesc.Rect.w + 2,
                pArea->FDesc.Rect.h + 2);      
      return; //更新完在
    }
    _PotItem(pArea, Handle | FocusAryId | pArea->STypeInfo.SType,
             x + pw * (Pos % RowCount),
             y + ph * (Pos / RowCount),
             Pos,PlotMask); //0x10: 第二背景为选中
    if(Pos == pArea->PrvFocus) return; //不更新上一个
    //更新本页上次的焦点区域数据
    Pos = pArea->PrvFocus;
    pArea->PrvFocus = pArea->Focus; //更新了    
    AryId += Pos;   //上次焦点

    _UpdateSTypeInfo(pArea, Handle, AryId);
    _PotItem(pArea, Handle | AryId | pArea->STypeInfo.SType,
             x + pw * (Pos % RowCount),
             y + ph * (Pos / RowCount),
             Pos, PlotMask & 0x0f);  //未选中
    return;
  }
  //========================扫描更新内容=========================
  unsigned short Count = RowCount * pAryDesc->ColCount;//页内阵列总数
  if(Count > 255) return ;//异常
  
  Pos = 0; //从头开始
  for(; Pos < Count; Pos++, AryId++){
    if(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN){//背景图变化焦点时替换背景
      if(Pos == pArea->Focus) pArea->FDesc.cBg = pAryDesc->cFocus;
      else pArea->FDesc.cBg = pAryDesc->cBg;
    }
    //更新失败时表示结束了
    if(!_UpdateSTypeInfo(pArea, Handle, AryId)){
      break;
    }
    //组织FDesc的x,y轴等信息,_PotItem()直接展开以提高效率！
    pArea->FDesc.Rect.x = x + pw * (Pos % RowCount);
    pArea->FDesc.Rect.y = y + ph * (Pos / RowCount);
    pArea->FDesc.IconBase = pArea->STypeInfo.IconBase;
    pArea->FDesc.IconCount = pArea->STypeInfo.IconCount;
    pArea->FDesc.ParaBase = pArea->STypeInfo.ParaBase;
    pArea->FDesc.ParaCount = pArea->STypeInfo.ParaCount;
    //调用固定区域绘图
    unsigned char CurPlotMask = PlotMask;
    if((AryId == FocusAryId) && (PlotMask & 0x03))
      CurPlotMask |= 0x10;//绘制背景或所有时,第二背景为选中
    RP_FixArea(Handle | AryId | pArea->STypeInfo.SType, 
               CurPlotMask, &pArea->FDesc);
  }//end for
  if(Pos >= Count) return;
  if(PlotMask == 0xff) return;//已整屏清屏了
  //清除余下区域防止翻页时遗留(实际应刷新整页) 
  Plot_SetBrushColor(pMainDesc->cBg); 
  for(; Pos < Count; Pos++){
    pArea->FDesc.Rect.x = x + pw * (Pos % RowCount);
    pArea->FDesc.Rect.y = y + ph * (Pos / RowCount);
    Plot_FullRect(pArea->FDesc.Rect.x-1, 
                  pArea->FDesc.Rect.y-1,
                  pArea->FDesc.Rect.w+2,
                  pArea->FDesc.Rect.h+2);
  }
}

//------------------------刷新焦点区函数----------------------------
//Refresh()后需调用
void _RefreshFocus(struct _RP_AryArea *pArea)
{
  //图片做背景时，已经更新过了。
  if(pArea->pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN){
    pArea->PrvFocus = pArea->Focus; //更新了 
    return;
  }
  _Refresh(pArea, 1); //需单独更新
}

//-----------------------------复位相关定时器----------------------------
void _ResetTimer(struct _RP_AryArea *pArea)
{
  pArea->AutoPageTimer = RP_ARY_AREA_KEY_KEEP_OV + 
                         RP_ARY_AREA_AUTO_SCREEN_OV;
  pArea->RefreshTimer = RP_ARY_AREA_REFRESH_PERIOD;  
}


//-----------------------------初始化函数----------------------------
void RP_AryArea_Init(struct _RP_AryArea *pArea,    //主结构
                     const struct _RpMainAreaDesc *pMainDesc,//整体描述
                     const struct _RpAryAreaDesc *pAryDesc, //项描述                     
                     unsigned long Handle, //主句柄 
                     unsigned short AryCount)   //初始化时阵列大小
{
  memset(pArea, 0, sizeof(struct _RP_AryArea));
  pArea->pMainDesc = pMainDesc;
  pArea->pAryDesc = pAryDesc;  
  pArea->Handle = Handle;  
  pArea->AryCount = AryCount;
  _ResetTimer(pArea);
  RP_AryArea_RefreshAll(pArea);//刷新所有
}

//--------------------------上翻页处理----------------------------
//返回是否响应
static signed char _PageUp(struct _RP_AryArea *pArea,
                           unsigned short Count)
{
  if(pArea->AryCount < Count) return 0;//只有一页不处理
  
  if(pArea->AryStart >= Count) pArea->AryStart -= Count;
  else//回环到最后一页
    pArea->AryStart = ((pArea->AryCount - 1) / Count) * Count;
  return 1;
}               

//--------------------------下翻页处理----------------------------
//返回是否响应
static signed char _PageDown(struct _RP_AryArea *pArea,
                              unsigned short Count)
{
  if(pArea->AryCount < Count) return 0;//只有一页不处理
  if((pArea->AryStart + Count) < pArea->AryCount)//有下页时
    pArea->AryStart += Count;
  else pArea->AryStart = 0;//最后一页时回到第1页
  
  return 1;
}

//-----------------------------按键处理函数----------------------------
//GuideKey定义为: 确认0左1上2右3下4, 5上一页，6下一页,及数字键'1'-'9'
void RP_AryArea_Key(struct _RP_AryArea *pArea,
                      unsigned char GuideKey)
{
  const struct _RpAryAreaDesc *pAryDesc = pArea->pAryDesc;   //项描述
  unsigned char Focus = pArea->Focus; 
  unsigned char RowCount = pAryDesc->RowCount;
  unsigned char ColCount = pAryDesc->ColCount; 
  unsigned short Count = RowCount * ColCount;
  unsigned short PrvAryStart = pArea->AryStart;
  switch(GuideKey){
    case 0: //确认键了
      RP_AryArea_cbSelFinal(pArea->Handle, pArea->AryStart + Focus);
      _ResetTimer(pArea);
      return;
    case 1: //左键
      if(RowCount >= 2){ //多行时表示向左选择
        if(Focus % RowCount) Focus--; //向左1格
        else Focus += RowCount - 1;//回环到最右
        break;
      }
      //else 左键解释为上翻页键
    case 5: _PageUp(pArea, Count);break;//上翻页键    
    case 3: //右键
      if(RowCount >= 2){ //多行时表示向右选择
        if((Focus % RowCount) < (RowCount - 1)) Focus++; //向右1格
        else Focus -= Focus % RowCount;//在最右了，回环到最左
        break;
      }
      //else 右键解释为下翻页键
    case 6: _PageDown(pArea, Count);break;//下翻页键    
    case 2: //上键
      if(Focus >= RowCount) Focus -= RowCount; //向上1格
      else if(!pArea->AryStart) return;//不允许上键回环到最后(对应最后下键不到开始)
      else{//上一页最后一排对应位置
        if(_PageUp(pArea, Count)){//翻页成功了
          Focus += Count - RowCount;
        }
      }
      break; 
    case 4: //下键
      if((Focus + RowCount) < Count) Focus += RowCount; //向下1格
      else{//下一页第1排对应位置
        if(_PageDown(pArea, Count)){//翻页成功了
          Focus = Focus % RowCount;
        }
      }
      break;  
    default: //其它键
      //响应数字键
      if((GuideKey >= '1') && (GuideKey <= '9')){
        Focus = GuideKey - '1';
      }
      else return; //其它键不支持 
  }
  pArea->Focus = Focus;
  _ResetTimer(pArea);
  //页变化
  if(PrvAryStart != pArea->AryStart){
    pArea->PlotMask = 0x7f; //刷新所有
    _Refresh(pArea, 0); //更新页面
    //焦点(最末页)超限时修正
    unsigned short EndPos = pArea->AryCount - pArea->AryStart;
    if(Focus >= EndPos){
      pArea->Focus = EndPos - 1; //最后一个
      pArea->PrvFocus = pArea->Focus;//不更新上次
      _Refresh(pArea, 1); //单独更新焦点
      return;
    }
    _RefreshFocus(pArea); //更新焦点
    return;
  }
  
  //仅焦点变化了
  if(Focus != pArea->PrvFocus){
    _Refresh(pArea, 1); //单独更新焦点
  }
}

//----------------------------任务处理函数----------------------------
//1s左右调用一次
void RP_AryArea_Task(struct _RP_AryArea *pArea)
{
  //自动翻页处理
  if(pArea->AutoPageTimer){
    pArea->AutoPageTimer--;
    if(!pArea->AutoPageTimer){//时间到了,更新所有
      RP_AryArea_Key(pArea, 6);
      pArea->AutoPageTimer = RP_ARY_AREA_AUTO_SCREEN_OV;//从滚动点计时
      return;
    }
  }
  //普通数据自动更新
  if(pArea->RefreshTimer){
    pArea->RefreshTimer--;
    if(!pArea->RefreshTimer){//时间到了,更新普通数据 
      pArea->RefreshTimer = RP_ARY_AREA_REFRESH_PERIOD;
      pArea->PlotMask = 0x0f; //刷新普通数据
      _Refresh(pArea, 0);     //需单独更新
      return;
    }
  }
  //实时数据更新
  if(pArea->pAryDesc->BitInfo & RP_ITEM_REAL_REFRESH){
    pArea->PlotMask = 0; //实时数据
    _Refresh(pArea, 0);  //需单独更新  
  }
}

//-------------------------------刷新所有函数----------------------------
//用于立即重建所有数据
void RP_AryArea_RefreshAll(struct _RP_AryArea *pArea)
{
  pArea->PlotMask = 0xff; 
  _Refresh(pArea, 0); //更新页面
  _RefreshFocus(pArea); //更新焦点
}


