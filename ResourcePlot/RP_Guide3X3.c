/***********************************************************************

                 资源文件型导航键模式3x3菜单实现
本模块独立于应用
***********************************************************************/

#include "RP_Guide3X3.h"
#include "ePic.h"
#include "Plot.h"
#include <string.h>

/***********************************************************************
                             相关函数实现
***********************************************************************/

//-----------------------------刷新界面----------------------------
static void _Refresh(const struct _RP_Guide3X3 *pGuide,
                      unsigned short Mask)
{
  unsigned char Focus = pGuide->Focus;
  for(unsigned char Pos = 0; Pos < 9; Pos++){
    if(!(Mask & (1 << Pos))) continue;//不刷新
    unsigned char IconId;
    if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_PIC2){//双图标模式时
      if(Pos == Focus){//焦点时
        if(Mask & 0x0800) IconId = 0;//焦点强制显示不选择中，用于动画
        else IconId = 1; //选中状态
      }
      else IconId = 0;//非焦点不选中
      IconId += pGuide->Desc.IconBase + Pos * 2;
    }
    else IconId = pGuide->Desc.IconBase + Pos;// 其它模式直接对应
    
    unsigned long Handle = pGuide->Desc.Handle;
    //由图标ID找到图数据
    const unsigned char *ePic = RP_Guide3X3_cbGetIcon(Handle, IconId);
    if((ePic == NULL) || (RP_Guide3X3_cbGetIconSize(Handle, IconId) == 0))
      return; //异常
      
    //==================画图片： 找到x,y及调色板信息==================
    Plot_SetPenStyle(pGuide->Desc.Bg);   
    ePic_ePicBuf(ePic, RP_cbGetIconSize(Handle,IconId));//初始化ePic    
    unsigned short pw = pGuide->Desc.Rect.w / 3; //所占宽度
    if(ePicBuf.Header.w >= pw) continue;//图片信息异常
    unsigned short x = (pw - ePicBuf.Header.w) / 2;
    x  += (Pos % 3) * pw + pGuide->Desc.Rect.x;
    unsigned short ph = pGuide->Desc.Rect.h / 3; //所占高度
    if(ePicBuf.Header.h >= ph) continue;//图片信息异常
    unsigned short y = (ph - ePicBuf.Header.h) / 2;
    y  += (Pos / 3) * ph + pGuide->Desc.Rect.y;
    if(Pos == Focus){//焦点时
      //吸附模式: 图标向左上移动2个像素
      if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_POP){
        x -= 2;
        y -= 2;
      }
      //按下模式: 图标向右下移动2个像素
      else if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_PUSH){
        x += 2;
        y += 2;
      }
    }//end 焦点时
    //设置图像调色板用于ePic模式
    Plot_SetPenColor(RP_Guide3X3_cbGetIconFg(Handle,IconId));
    ePic_Plot(x,y);//绘图 
    
    if(Pos != Focus) continue;
    
    //=====================焦点时部分样式后期处理=========================
    //描边模式: 即四周增加Fg[0]个像素Fg[1]色框
    if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_RECT){
      Plot_SetPenColor(pGuide->Desc.Fg[1]);
      Plot_Rect(x - 1,y - 1, ePicBuf.Header.w + 2, ePicBuf.Header.h + 2);
      if(pGuide->Desc.Fg[0])//双色框
        Plot_Rect(x - 2,y - 2, ePicBuf.Header.w + 4, ePicBuf.Header.h + 4);
      continue;
    }
    
    //吸附模式: 图标向左上移动2个像素，同时右侧与下侧增加共两个像素指定阴影
    if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_POP){
      unsigned short Offset = x + ePicBuf.Header.w;
      Plot_SetPenColor(pGuide->Desc.Fg[0]);
      Plot_LineV(Offset, y+1,ePicBuf.Header.h);
      Plot_SetPenColor(pGuide->Desc.Fg[1]);
      Plot_LineV(Offset + 1,y + 2,ePicBuf.Header.h);
      Offset = y + ePicBuf.Header.h;
      Plot_SetPenColor(pGuide->Desc.Fg[0]);      
      Plot_LineH(x + 1, Offset, ePicBuf.Header.w);
      Plot_SetPenColor(pGuide->Desc.Fg[1]);         
      Plot_LineH(x + 2, Offset + 1, ePicBuf.Header.w);
      continue;
    }
    //按下模式: 图标向右下移动2个像素，同时左侧与上侧增加共两个像素指定阴影
    if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_PUSH){
      Plot_SetPenColor(pGuide->Desc.Fg[0]);
      Plot_LineV(x - 1, y - 1,ePicBuf.Header.h);
      Plot_SetPenColor(pGuide->Desc.Fg[1]);
      Plot_LineV(x - 2, y - 2,ePicBuf.Header.h);
      Plot_SetPenColor(pGuide->Desc.Fg[0]);      
      Plot_LineH(x - 1, y - 1, ePicBuf.Header.w);
      Plot_SetPenColor(pGuide->Desc.Fg[1]);         
      Plot_LineH(x - 2, y - 2, ePicBuf.Header.w);
    }
  }//end for
}

//-----------------------------初始化函数----------------------------
void RP_Guide3X3_Init(struct _RP_Guide3X3 *pGuide,//确保Desc初始化
                      signed char DefaultFocus) //默认焦点,负在中间
{
  //倒计时器
  if(pGuide->Desc.Cfg & RP_GUIDE3X3_AUTO_ENTER)
    pGuide->Countdown = RP_GUIDE3X3_AUTO_SEL_OV;
  else pGuide->Countdown = 0;
  pGuide->DyncTimer = 0;
  //快速模式不允许边角功能
  if(pGuide->Desc.Cfg & RP_GUIDE3X3_FAST_MODE) 
    pGuide->Desc.Cfg &= ~((1 << 0)|(1 << 2)|(1 << 6)|(1 << 8));
  
  //最后Focus初始化，防止并初始化时纠错
  if((DefaultFocus < 0) || (DefaultFocus >= 9))//焦点在中间
    DefaultFocus = 5; 
  unsigned short Cfg = pGuide->Desc.Cfg;
  signed char Focus = DefaultFocus;
  for(; Focus >= 0; Focus--){//用前向纠错
    if(Cfg & (1 << Focus)){//找到了
      pGuide->Focus = Focus;
      _Refresh(pGuide,pGuide->Desc.Cfg);//全部刷新
      return; 
    }
  }
  for(; Focus < 9; Focus++){//用后向纠错
    if(Cfg & (1 << Focus)){//找到了
      pGuide->Focus = Focus;
      _Refresh(pGuide,pGuide->Desc.Cfg);//全部刷新
      return; 
    }
  }
  //全部不可选择,强制开启中间
  pGuide->Desc.Cfg |= (1 << 5);
  pGuide->Focus = 5;
  return;
}

//-----------------------------按键处理函数----------------------------
//GuideKey定义为: 确认0左1上2右3下4,
void RP_Guide3X3_Key(struct _RP_Guide3X3 *pGuide,
                      unsigned char GuideKey)
{
  unsigned char Focus = pGuide->Focus;
  unsigned char OrgFocus = Focus;  
  unsigned short Cfg = pGuide->Desc.Cfg;
  switch(GuideKey){
    case 0: //确认键了
      RP_Guide3X3_cbSelFinal(pGuide->Desc.Handle, Focus);
      return;
    case 1: //左键向左1格
      if(Focus % 3) Focus--; //向左1格
      else break; //已在最左边了
      //允许选择时完成
      if(Cfg & (1 << Focus)) break; 
      //不允许选择时，再向左
      if(Focus % 3) Focus--;
      else Focus = OrgFocus; //已在最左不允许选择需还原
      if(Cfg & (1 << Focus)) break; //最左允许选择了
      else Focus = OrgFocus; //最左不允许选择，还原
      break;
    case 2: //上键向上1格
      if(Focus >= 3) Focus -= 3; //向上1格
      else break; //已在最上边了
      //允许选择时完成
      if(Cfg & (1 << Focus)) break; 
      //不允许选择时，再向上
      if(Focus >= 3) Focus -= 3; //向上1格
      else Focus = OrgFocus; //已在最上不允许选择需还原
      if(Cfg & (1 << Focus)) break; //最上允许选择了
      else Focus = OrgFocus; //最上不允许选择，还原
      break;    
    case 3: //右键向右1格
      if((Focus % 3) != 2) Focus++; //向右1格
      else break; //已在最右边了
      //允许选择时完成
      if(Cfg & (1 << Focus)) break; 
      //不允许选择时，再向右
      if((Focus % 3) != 2) Focus++; //向右1格
      else Focus = OrgFocus; //已在最左不允许选择需还原
      if(Cfg & (1 << Focus)) break; //最左允许选择了
      else Focus = OrgFocus; //最左不允许选择，还原
      break;
    case 4: //下键向下1格
      if(Focus < 6) Focus += 3; //向下1格
      else break; //已在最下边了
      //允许选择时完成
      if(Cfg & (1 << Focus)) break; 
      //不允许选择时，再向向
      if(Focus < 6) Focus += 3; //向下1格
      else Focus = OrgFocus; //已在最下不允许选择需还原
      if(Cfg & (1 << Focus)) break; //最上允许选择了
      else Focus = OrgFocus; //最上不允许选择，还原
      break;
    default: return; //其它键不支持 
  }
  
  if(Focus == OrgFocus) return;//选择未改变
    pGuide->Focus = Focus;
    
  if(Cfg & RP_GUIDE3X3_AUTO_ENTER) //自动选择时
    RP_Guide3X3_cbSelFinal(pGuide->Desc.Handle, Focus);
  else _Refresh(pGuide, (1 << Focus) | (1 << OrgFocus));//刷新变动前后图标
}

//----------------------------任务处理函数----------------------------
//1s左右调用一次
void RP_Guide3X3_Task(struct _RP_Guide3X3 *pGuide)
{
  //自动选择倒计时及处理
  if(pGuide->Countdown){
    pGuide->Countdown--;
    if(!pGuide->Countdown){//时间到了
      RP_Guide3X3_cbSelFinal(pGuide->Desc.Handle, pGuide->Focus);
      return;
    }
  }
  //动画处理: 周期性刷新动画
  if(pGuide->Desc.Cfg & RP_GUIDE3X3_EN_DYNC){
    pGuide->DyncTimer++;
    unsigned short Mask = (1 << pGuide->Focus);
    if(pGuide->DyncTimer & 1) Mask |= 0x0800; //用非选中图标刷新
    _Refresh(pGuide, Mask);//刷新显示
  }
}


