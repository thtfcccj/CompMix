/*******************************************************************************

                         压缩时间设置-在TMenu系统中的菜单实现
此模块通过回调独立于应用
*******************************************************************************/

#include "ZipTime_TMenu.h"
#include "TMenu_MNumAdj.h"
#include "UiTips.h"

#include <string.h>

#include "ZipTime.h"
#include "RTC.h" //RTC_SetRTC

//----------------------------内部字符资源------------------------------- 
#ifdef TM_EN_MUTI_LAN            //常量选择型多国语言时
  //暂不支持常量选择型字符串
#elif defined(TM_RESOURCE_LAN)  //由资源文件获取多国语言时
  //菜单
  #include "lsAry.h"
  //提示
  #include "Rid.h"
  #include "RidNote.h"
  #define ls_OpFinal   RID_pGeNote(RID_NOTE_OP_FINAL)
#else  //单一语言字符串时
  //菜单
  static const char ls_Year[] = {"年"};
  static const char ls_Month[] = {"月"};
  static const char ls_Day[] = {"日"};
  static const char ls_Hour[] = {"时"};
  static const char ls_Minute[] = {"分"};
  static const char ls_TimeSet[]= {"时间设置"};
  //提示
  extern const char ls_OpFinal[]= {"操作成功!"};
  
#endif

//描述
const struct _MNumDesc TimeSetDesc[] = {
  {MNUM_TYPE_DEC, ZipTime_cbGetStartYear(), ZipTime_cbGetStartYear() + 63},//年
  {MNUM_TYPE_DEC, 1, 12},//月
  {MNUM_TYPE_DEC, 1, 31},//日
  {MNUM_TYPE_DEC, 0, 23},//时
  {MNUM_TYPE_DEC, 0, 59},//分  
};

//时间对应的字符
const LanCode_t* const lsAry_Time[] = {
  ls_Year, ls_Month, ls_Day, ls_Hour, ls_Minute};

/*******************************************************************************
                           集中调整实现
设置好后集中保存并提示
*******************************************************************************/

//----------------------------------回调函数------------------------------- 
static void _Notify(unsigned char Type,//通报类型
                    //根据菜单类型输入的输入输出数据区
                    //该区域结构在控件里定义,直接指针转换即可
                    void *pv)
{
  struct _MNumAdjUser *pUser = (struct _MNumAdjUser *)pv;
  switch(Type){
  case TM_NOTIFY_GET_DATA:{ //将当前值装入
    unsigned long ZipTime = ZipTime_GetRTC();
    pUser->Value[0] = ZipTime_GetInfo(0, ZipTime) + ZipTime_cbGetStartYear();
    pUser->Value[1] = ZipTime_GetInfo(1, ZipTime);
    pUser->Value[2] = ZipTime_GetInfo(2, ZipTime);
    pUser->Value[3] = ZipTime_GetInfo(3, ZipTime);
    pUser->Value[4] = ZipTime_GetInfo(4, ZipTime);
    
    break;
  }
  case TM_NOTIFY_SET_DATA:{ //保存设定值
    RTC_SetRTC(pUser->Value[0],
               pUser->Value[1],
               pUser->Value[2],
               pUser->Value[3],
               pUser->Value[4],
               0);
    UiTips_UpdateS(ls_OpFinal); //提示成功
    break; 
  }
  case TM_NOTIFY_MNUM_GET_DESC:{ //得到数值描述
    memcpy(&pUser->Desc, 
           &TimeSetDesc[pUser->CurItem], 
           sizeof(struct _MNumDesc));
    break;
  }
  default:break;
  }
}


//--------------------------顶层菜单结构----------------------------

const TMenu_t ZipTime_TMenu = {//菜单结构
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE, //菜单类型为多值调整模式模式与用户区标志
  5,                          //由菜单类型决定的相关数据大小
  ls_TimeSet,                //菜单头,为NULL时从回调里读取
  ZIP_TIME_TMENU_PARENT,      //自已的父菜单
  lsAry_Time,                //存放自已的子菜单阵列连接头
  _Notify,                    //与用户空间交互的通报函数
};

/*******************************************************************************
                    每项设置完即保存实现
*******************************************************************************/

//----------------------------------回调函数------------------------------- 
static void _TimeSetNotify(unsigned char Type,//通报类型
                    //根据菜单类型输入的输入输出数据区
                    //该区域结构在控件里定义,直接指针转换即可
                    void *pv)
{
  struct _MNumAdjUser *pUser = (struct _MNumAdjUser *)pv;
  unsigned long ZipTime = ZipTime_GetRTC();
  switch(Type){
  case TM_NOTIFY_GET_DATA:{ //将当前值装入
    pUser->Value[0] = ZipTime_GetInfo(0, ZipTime) + ZipTime_cbGetStartYear();
    pUser->Value[1] = ZipTime_GetInfo(1, ZipTime);
    pUser->Value[2] = ZipTime_GetInfo(2, ZipTime);
    pUser->Value[3] = ZipTime_GetInfo(3, ZipTime);
    pUser->Value[4] = ZipTime_GetInfo(4, ZipTime);
    break;
  }
  case TM_NOTIFY_CHANGED:{ //数值被改变
    RTC_SetRTC(pUser->Value[0],
               pUser->Value[1],
               pUser->Value[2],
               pUser->Value[3],
               pUser->Value[4],
               ZipTime_GetInfo(5, ZipTime));
    break; 
  }
  case TM_NOTIFY_MNUM_GET_DESC:{ //得到数值描述
    memcpy(&pUser->Desc, 
           &TimeSetDesc[pUser->CurItem], 
           sizeof(struct _MNumDesc));
    break;
  }
  default:break;
  }
}

//--------------------------顶层菜单结构----------------------------
const TMenu_t ZipTimeS_TMenu = {//菜单结构
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE, //菜单类型为多值调整模式模式与用户区标志
  5,                          //由菜单类型决定的相关数据大小
  ls_TimeSet,                //菜单头,为NULL时从回调里读取
  ZIP_TIME_TMENU_PARENT2,   //自已的父菜单
  lsAry_Time,                //存放自已的子菜单阵列连接头
  _TimeSetNotify,            //与用户空间交互的通报函数
};


