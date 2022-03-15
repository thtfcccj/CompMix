/*******************************************************************************

                         压缩时间设置-在TMenu系统中的菜单实现
此模块通过回调独立于应用
*******************************************************************************/

#include "ZipTime_TMenu.h"
#include "ZipTime.h"
#include "TMenu_MNumAdj.h"

#include <string.h>

//描述
static const struct _MNumDesc _TimeSetDesc[] = {
  {MNUM_TYPE_DEC, ZipTime_cbGetStartYear(), ZipTime_cbGetStartYear() + 63},//年
  {MNUM_TYPE_DEC, 1, 12},//月
  {MNUM_TYPE_DEC, 1, 31},//日
  {MNUM_TYPE_DEC, 0, 23},//时
  {MNUM_TYPE_DEC, 0, 59},//分  
};

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
    unsigned long ZipTime = 
      ZipTime_FromTime(pUser->Value[0] - ZipTime_cbGetStartYear(),
                       pUser->Value[1],
                       pUser->Value[2],
                       pUser->Value[3],
                       pUser->Value[4],
                       0);               //固定为0
    Menu_ZipTime_cbSet(ZipTime);
    TMenu_Note(NULL); //提示成功
    break; 
  }
  case TM_NOTIFY_MNUM_GET_DESC:{ //得到数值描述
    memcpy(&pUser->Desc, 
           &_TimeSetDesc[pUser->CurItem], 
           sizeof(struct _MNumDesc));
    break;
  }
  default:break;
  }
}

//--------------------------顶层菜单结构----------------------------
#ifdef TM_EN_MUTI_LAN            //常量选择型多国语言时
  //暂不支持常量选择型字符串
#elif defined(TM_RESOURCE_LAN)  //由资源文件获取多国语言时
  static const LanCode_t  _TimeSet[]= {ZIP_TIME_YMENU_PID, 0};
  static const LanCode_t _Year[] = {ZIP_TIME_YMENU_PID, 1};
  static const LanCode_t _Month[] = {ZIP_TIME_YMENU_PID, 2};
  static const LanCode_t _Day[] = {ZIP_TIME_YMENU_PID, 3};
  static const LanCode_t _Hour[] = {ZIP_TIME_YMENU_PID, 4};
  static const LanCode_t _Minute[] = {ZIP_TIME_YMENU_PID, 5};
#else  //单一语言字符串时
  static const char _TimeSet[]= {"时间设置"};
  static const char _Year[] = {"年"};
  static const char _Month[] = {"月"};
  static const char _Day[] = {"日"};
  static const char _Hour[] = {"时"};
  static const char _Minute[] = {"分"};
#endif

static const char * const _TimeAry[] = {
  _Year, _Month, _Day, _Hour, _Minute
};

const TMenu_t Menu_ZipTime = {//菜单结构
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE, //菜单类型为多值调整模式模式与用户区标志
  5,                          //由菜单类型决定的相关数据大小
  _TimeSet,                //菜单头,为NULL时从回调里读取
  &ZIP_TIME_TMENU_PARENT,      //自已的父菜单
  _TimeAry,                //存放自已的子菜单阵列连接头
  _Notify,                    //与用户空间交互的通报函数
};

