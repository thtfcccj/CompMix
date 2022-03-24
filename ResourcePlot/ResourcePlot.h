/******************************************************************************

                         资源信息绘图模块

为简化相关操作，本资源前缀以Rp(Resource Plot)打头
******************************************************************************/
#ifndef __RESOURCE_PLOT_H 
#define __RESOURCE_PLOT_H 
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif
#include "Color.h"

/*****************************************************************************
                              句柄Handle定义与说明
*****************************************************************************/
//Handle作为唯一的ID号与应用层交互，按位分为以下几部分：

//实时刷新调用标志,不需要实时部分,在可取图片与字符资源时，可返回异常以停止刷新
#define RP_HANDLE_REAL_REFRESH 0x80000000
//主类，固定分配用于大类识别
#define RP_HANDLE_MTYPE_MASK   0x7f000000
//子类，在RP_AryArea回调获得(某类设备)
#define RP_HANDLE_STYPE_MASK   0x00ff0000
//阵列ID号(某个设备)
#define RP_HANDLE_ARY_MASK     0x00ffffff   

/******************************************************************************

                             资源描述结构

******************************************************************************/

//---------------------------------矩形区域结构-------------------------------
//以屏幕左上角为起点，全部以像素为单位
struct _RpRect{
  unsigned short x; //距屏左像素距离
  unsigned short y; //距屏上像素距离
  unsigned short w; //像素宽度
  unsigned short h; //像素高度
};

//--------------------------------主界面区域结构---------------------------------
//描述主界面区域与默认背景色
struct _RpMainAreaDesc{     //->此结构在资源定中的前缀标识  "RPM_"
  struct _RpRect Rect;      //位置范围信息
  Color_t cBg;               //默认背景色
};

//------------------------------固定区域描述结构--------------------------------
//如标题栏，信息栏，用此信息绘制一片区域，需要绘制的信息，含图标与变量
struct _RpFixAreaDesc{     //->此结构在资源定中的前缀标识"RPF_"
  struct _RpRect Rect;     //位置范围信息
  Color_t cBg;              //此区域的背景色
  unsigned char IconBase;  //图标信息ID查找表起始
  unsigned char IconCount; //图标信息ID查找表个数
  unsigned char ParaBase;  //变量信息ID查找表起始
  unsigned char ParaCount; //变量信息ID查找表个数  
};

//----------------------------阵列界面中的项区域描述----------------------------
//当主界内有多个相同阵列区域(如状态，菜单)的项时，用于描述此项同的结构以绘制该项
struct _RpAryAreaDesc{    //->此结构在资源定中的前缀标识  "RPX_"
  struct _RpRect Rect;     //位置范围信息，x,y为在主界面中的相对位置
  unsigned char  RowCount; //在主界面中，横向阵列排列个数
  unsigned char  ColCount; //在主界面中，纵向阵列排列个数
  union{//区域阵列信息与菜单：
    unsigned char IconBgId;  //背景图ID,若有且为焦点，则首图为未中，第二图为选中
    unsigned char IconBase;  //菜单图ID,若有且为焦点，则首图为未中，第二图为选中
  };
  unsigned char  BitInfo;  //位信息，见定义
  Color_t cBg;              //此区域的背景色
  Color_t cFocus;           //被选中时边界颜色  
};
//位信息定义为：
#define RP_ITEM_REAL_REFRESH       0x80  //项内有需要实时刷新的数据
#define RP_ITEM_DIS_FOCUS          0x40  //无焦点区，即不响应点击
#define RP_ITEM_ICON_BG_VALID      0x20  //背景图图片有效,即仅为背景色绘图
#define RP_ITEM_ICON_BG_FOCUS_EN   0x10  //背景图片焦点有效，即选中时使用图片
#define RP_ITEM_ID_MASK            0x0F  //应用层使用同一函数时，此结构的ID号

//-----------------------------图标信息结构------------------------------
//如状态栏中的时间图标，使用此结构时，要求所父区域宽度与高度,512个像素
struct _RpIconDesc{        //->此结构在资源定中的前缀标识  "RPI_ARY"
  unsigned char Info;     //信息位，见定义
  unsigned char xl;       //位置x轴低8位
  unsigned char yl;       //位置y轴低8位 
};
#define SIZEOF_RP_ICON_DESC   3//实际占位

//信息位定义为：
#define RP_ICON_ID_MASK      0x3F  //b0~b5图标ID
#define RP_ICON_X9           0x40  //b6=1时x轴+256
#define RP_ICON_Y9           0x80  //b7=1时y轴+256

//注1：系统将相同类型的图标统一管理，从中找出该图标绘制函数查找表进行绘制。
//注2：图标可能为wmp格式时,资源文件应提供图标ID对应的前景色阵列用于绘制。

//-----------------------------变量信息结构------------------------------
//如状态栏中的时间，使用此结构时，要求所父区域宽度与高度,512个像素
struct _RpParaDesc{        //->此结构在资源定中的前缀标识  "RPP_ARY"
  unsigned char Info;     //信息位，见定义
  unsigned char xl;       //相对位置x轴低8位
  unsigned char yl;       //位置y轴低8位
  unsigned char Para;     //其它参数，见定义 
};
#define SIZEOF_RP_PARA_DESC   4//实际占位

//信息位定义为：
#define RP_PARA_ID_MASK      0x3F  //b0~b5参数ID
#define RP_PARA_X9           0x40  //b6=1时x轴+256
#define RP_PARA_Y9           0x80  //b7=1时y轴+256
//其它参数,定义为：
#define RP_PARA_FONT_MASK    0x0F  //所示使用的字体ID号


//注1：系统将相同类型的绘图参数函数组成阵列，从中找出该函数进行给制。
//注2：资源文件应提供变量ID相关的颜色(可根据状态切换颜色)以用于绘制。

//-----------------------------其它结构或常量说明-----------------------
//资源定中的其它前缀标识定义为
//颜色阵列：统一以“C_”开头，后跟用途等信息
//表空间分配以2字节对齐，以对齐颜色等

/*****************************************************************************
                              通用绘图函数
*****************************************************************************/

//-------------------------指定位置图标绘图-----------------------------
//更新某个具体图标
void RP_PaintIcon(unsigned long Handle,const struct _RpIconDesc *pDesc,
                  unsigned short x, unsigned short y);//相对x,y

//-------------------------指定位置参数绘图-----------------------------
//更新某个具体参数
void RP_PaintPara(unsigned long Handle,const struct _RpParaDesc *pDesc,
                  unsigned short x, unsigned short y);//相对x,y

/********************************************************************************
                           回调函数
********************************************************************************/

//----------------------------由图标ID找到图数据------------------------------
const unsigned char *RP_cbGetIcon(unsigned long Handle,
                                   unsigned char IconId);

//----------------------------由图标ID找到图数据大小----------------------------
unsigned long RP_cbGetIconSize(unsigned long Handle,
                               unsigned char IconId);

//----------------------------由图标ID找图标前景色------------------------------
//用于wbm格式时着色
Color_t RP_cbGetIconFg(unsigned long Handle,
                       unsigned char IconId);

//----------------------------由参数AryID找参数信息----------------------------
const struct _RpParaDesc *RP_cbGetParaDesc(unsigned long Handle,
                                            unsigned char ParaAryId);

//----------------------------由参数AryID找字符串------------------------------
const char * RP_cbGetString(unsigned long Handle,
                             unsigned char ParaAryId);

//----------------------------由参数ID找图标前景色------------------------------
//由图标ID找到图
Color_t RP_cbGetParaFg(unsigned long Handle,
                       unsigned char ParaId);


#endif  //#ifndef __RESOURCE_PLOT_H 



