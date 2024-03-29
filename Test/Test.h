/***********************************************************************

		                    测试子模块
此模块应用无关
***********************************************************************/
#ifndef _TEST_H
#define	_TEST_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/***********************************************************************
		                      相关配置
***********************************************************************/

//是否支持测试起始回调，全局里定义
//#define SUPPORT_TEST_NOTIFY    


#ifndef TEST_COUNT
  #define	TEST_COUNT          8      //定义测试持续时间,128ms为单位
#endif
                            

/***********************************************************************
		                      相关结构
***********************************************************************/

struct _Test{
  unsigned char Index;        //测试时间计数
};

extern struct _Test Test;          //直接实例化

/***********************************************************************
		                      相关函数
***********************************************************************/

//-----------------------初始化函数-------------------------------
//void Test_Init(void);
#define Test_Init() do{Test.Index = 0;}while(0)

//----------------------普通任务函数-------------------------------
//放入256ms进程中扫描
void Test_Task(void);

//-------------------------开始测试函数---------------------------
void Test_Start(void);

//----------------------强制清测试状态函数------------------------
void Test_Clr(void);

//----------------------判断测试状态函数---------------------------
#define Test_IsDoing() (Test.Index)

/***********************************************************************
		                     回调函数
***********************************************************************/

//----------------------测试开始通报函数-------------------------------
#ifndef SUPPORT_TEST_NOTIFY
  #define Test_cbStartNotify() do{}while(0)
#else
  void Test_cbStartNotify(void);
#endif

//----------------------测试结束通报函数-------------------------------
#ifndef SUPPORT_TEST_NOTIFY
  #define Test_cbExitNotify() do{}while(0)  
#else 
  #ifdef SUPPORT_LED //直接实现以加快效率
    #include "Led.h"
    #define Test_cbExitNotify() Led_ClrTest()   //清指示灯
  #else
    void Test_cbExitNotify(void);
  #endif
#endif

#endif
