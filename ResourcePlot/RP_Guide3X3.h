/***********************************************************************

                 资源文件型导航键模式3x3菜单接口
本模块独立于应用
***********************************************************************/
#ifndef __RP_GUIDE3X3_H 
#define __RP_GUIDE3X3_H 
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif
/***********************************************************************
                             说明
***********************************************************************/

//支持两种模式：
//  普通模式: 以九宫格方式排列：此模式下需按“确认”键进行选择
//              0   □ □ □   2
//                  □ ■ □ 
//              6   □ □ □   8
//  快速模式: 只有确认，左上右下5个图标，此模式下直接按对应导航键进入
//                     □   
//                  □ ■ □ 
//                     □   
//两种模式说明：
//  可开启进入时，自动选择模式：即一段时间不按键，自动进入被选择功能。
//  可指定进入时焦点(此模式建议去掉自动选择模式)，以实现返回
//  默认进入后焦点位于中间，以按最少的键进行选择。
//  可选焦区图标动画功能
//  图标使能功能：可用于权限控制，如：不需要左上或右上功能时：
//                     □    
//                  □ ■ □  除中部功能外，其它功能均可禁止。
//                  □ □ □ 
//  位置：从左到右，从上到下排列, 每个图标有选中与不选中两个
//  

/***********************************************************************
                           相关配置
***********************************************************************/

#ifndef RP_GUIDE3X3_AUTO_SEL_OV    
  #define RP_GUIDE3X3_AUTO_SEL_OV   10//自动选择打开时，进入时间
#endif

//Rp_Guide3X3显示样式,选中与不选中支持,有需要支持的样式时需开启:
//#define SUPPORT_RP_GUIDE3X3_PIC2    //双图标模式: 即选中为第二图标
//#define SUPPORT_RP_GUIDE3X3_REPLACE //替换模式: 将图标中的背景色改为指定色
//#define SUPPORT_RP_GUIDE3X3_RECT    //描边模式: 即四周增加指定色色框
//#define SUPPORT_RP_GUIDE3X3_POP     //吸附模式: 图标向左上移动2个像素并增加指定阴影
//#define SUPPORT_RP_GUIDE3X3_PUSH    //按下模式: 图标向右下移动2个像素并增加指定阴影
//#define SUPPORT_RP_GUIDE3X3_INC     //加色模式: 图标的每个像素RGB增加指定色值以改变颜色
//#define SUPPORT_RP_GUIDE3X3_DEC     //减色模式: 图标的每个像素RGB减去指定色值以改变颜色


/***********************************************************************
                             相关结构
***********************************************************************/
//依赖
#include "ResourcePlot.h"
#include "Color.h"

//-------------------------描述结构----------------------------------
//需外部初始化的区域：
struct _RP_Guide3X3_Desc{
  struct _RpRect Rect;      //所在区域
  unsigned long Handle;    //用于查找绘图资源及回调
  unsigned short Cfg;      //相关配置，见定义  
  unsigned char IconBase;  //资源图标起始
  unsigned char Style;     //选中时显示样式，见定义
  Color_t Bg;               //背景色
  Color_t Fg[2];            //见显示样式对应说明
};

//相关配置定义为
#define RP_GUIDE3X3_FAST_MODE  0x8000 //快速模式，否则为普通模式
#define RP_GUIDE3X3_AUTO_ENTER 0x4000 //开启进入时，自动选择模式
#define RP_GUIDE3X3_EN_DYNC    0x2000 //开启焦区图标动画功能

#define RP_GUIDE3X3_EN_MASK    0x01FF //允许对应图标显示及选择，否则不显示

//选中时显示样式定义为：
//双图标模式: 即选中为第二图标, 即：IconId = 位置 * 2 + IconBase,Fg[0]Fg[1]无效
#define RP_GUIDE3X3_STYLE_PIC2    0  
//替换模式: 将图标中的调色板Fg[0]色，替换为调色板Fg[1]色(需底层调色板支持,暂未实现)
#define RP_GUIDE3X3_STYLE_REPLACE 1
//描边模式: 即四周增加Fg[0]个像素Fg[1]色框
#define RP_GUIDE3X3_STYLE_RECT    2 
//吸附模式: 图标向左上移动2个像素，同时右侧与下侧增加Fg[0]Fg[1]色共两个像素指定阴影
#define RP_GUIDE3X3_STYLE_POP     3 
//按下模式: 图标向右下移动2个像素，同时左侧与上侧增加Fg[0]Fg[1]色共两个像素指定阴影
#define RP_GUIDE3X3_STYLE_PUSH    4
//加色模式: 图标的每个像素RGB分别减去Fg[0]色(需底层绘图支持,暂未实现)
#define RP_GUIDE3X3_STYLE_INC     5
//减色模式: 图标的每个像素RGB分别加上Fg[0]色(需底层绘图支持,暂未实现)
#define RP_GUIDE3X3_STYLE_DEC     6

//---------------------------主结构----------------------------------
struct _RP_Guide3X3{
  struct _RP_Guide3X3_Desc  Desc; //初始化函数前需全部设置
  //内部管理
  unsigned char DyncTimer;   //动画定时器  
  unsigned char Countdown;   //自动选择倒计定时器
  unsigned char Focus;      //焦点，即选择的IconId
  unsigned char PrvFocus;   //上次焦点位置,用于清除
};

/************************************************************************
                           相关函数
*************************************************************************/

//-----------------------------初始化函数----------------------------
void RP_Guide3X3_Init(struct _RP_Guide3X3 *pGuide,//确保Desc初始化
                      signed char DefaultFocus); //默认焦点,负在中间

//----------------------------任务处理函数----------------------------
//1s左右调用一次
void RP_Guide3X3_Task(struct _RP_Guide3X3 *pGuide); 

//-------------------------------刷新所有函数----------------------------
//用于立即重建所有数据
void RP_Guide3X3_RefreshAll(struct _RP_Guide3X3 *pGuide); 

//-----------------------------按键处理函数----------------------------
//GuideKey定义为: 确认0左1上2右3下4,及数字键'1'-'9'
void RP_Guide3X3_Key(struct _RP_Guide3X3 *pGuide,
                      unsigned char GuideKey); 

/******************************************************************************
                               回调函数
*******************************************************************************/

//------------------------------图标被选择------------------------------------
//即执行图标指定的功能，实现跳转
void RP_Guide3X3_cbSelFinal(unsigned long Handle, unsigned char Focus); //0-8

//-----------------------------倒计时通报------------------------------------
//用于用户在提示区等显示倒计时信息
void RP_Guide3X3_cbCountdownNotify(unsigned char Countdown); 

//----------------------------由图标ID找到图数据------------------------------
const unsigned char *RP_Guide3X3_cbGetIcon(unsigned long Handle,
                                           unsigned char IconId);

//----------------------------由图标ID找到图数据大小--------------------------
unsigned long RP_Guide3X3_cbGetIconSize(unsigned long Handle,
                                        unsigned char IconId);

//----------------------------由图标ID找图标前景色----------------------------
//用于wbm格式时着色
Color_t RP_Guide3X3_cbGetIconFg(unsigned long Handle,
                                unsigned char IconId);

#endif



