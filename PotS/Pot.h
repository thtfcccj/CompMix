/***************************************************************************

                   单例化 数字电位器(POT)标准化接口

****************************************************************************/
#ifndef _POT_H
#define _POT_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/*******************************************************************************
                               相关定义
********************************************************************************/
//#define SUPPORT_POT      //此模块外部引用建议可编译项


//支持硬件实时延时检测时，全局定义,同时定义延时检测周期 Pot_Task()调用周期为单位
//#define SUPPORT_POT_DELAY_HW_CHECK      255            

//数字电位器支持最高位数(最低位数固定为0)
#ifndef POT_UP
  #define POT_UP    255
#endif

//数字电位器单位
#if POT_UP <= 255
  #define PotLen_t  unsigned char
#else
  #define PotLen_t  unsigned short
#endif
  
//默认位置,仅外部使用
#ifndef POT_DEFAULT_POS
  #define POT_DEFAULT_POS   128  //在中间
#endif
  
/*******************************************************************************
                             相关结构
********************************************************************************/
struct _PotInfo{
  PotLen_t Pos;   //数字电位器位置
};

struct _Pot{
  struct _PotInfo Info;
  PotLen_t CurPos;   //当前调整的位置
  unsigned char Id; //分配的ID号(支持多个POT)
  unsigned char Flag;//相关标志，见定义
  #ifdef SUPPORT_POT_DELAY_HW_CHECK   //支持硬件实时同步检测时
    unsigned short Delay;     //硬件检测延时时间
  #endif
};

//相关标志定义为:
#define POT_HW_COMM_COUNT_MASK 0x0f  //硬件通讯次数计数,具体值定义为:
#define POT_HW_COMM_ERR_HW     0x0f  //硬件未通讯上
#define POT_HW_COMM_SYNC_FINAL 0     //硬件同步上了

#define POT_HW_COMM_BLOCK      0x10    //硬件通讯阻塞
#define POT_EX_MASK            0xE0    //余下留给派生类等

extern struct _Pot Pot;//直接单例化
/***************************************************************************
                          对外接口部分
***************************************************************************/

//-------------------------------初始化函数---------------------------------
void Pot_Init(unsigned char Id,  //分配的通道标识
              signed char IsInited);//初始化标志

//-----------------------------任务函数---------------------------------
//<8ms调用一次
void Pot_Task(void);

//---------------------------得到电位器当前位置-----------------------------
#define Pot_GetCurPos()  (Pot.CurPos)

//------------------------更新电位器位置---------------------------------
void Pot_UpdatePos(PotLen_t NewPos);

//---------------------------恢复电位器位置---------------------------------
#define Pot_RestorePos() do{Pot_UpdatePos(Pot.Info.Pos); }while(0)

//---------------------------保存电位器当前位置-----------------------------
void Pot_SavePos(void);

//------------------------结束电位器位置到硬件POT-----------------------------
//POT通讯完成后可主动调用
void Pot_Pos2HwEnd(signed char Resume);//0:正常结束，负：未通讯上结束

//------------------------是否有故障(未通讯上)-----------------------------
#define Pos_IsErr() ((Pot.Flag & POT_HW_COMM_COUNT_MASK) == POT_HW_COMM_ERR_HW)

/***************************************************************************
                          回调函数
***************************************************************************/

//---------------------------得到默认POT位置-----------------------------
//未初始化时调用
PotLen_t Pot_cbGetDefaultPos(void);

//-----------------------------硬件初始化调用-----------------------------
void Pot_cbPos2HwInit(void);

//-------------------------开始电位器位置到硬件POT------------------------
//即将当前位置打到数字电位器上，根据不同通讯由硬件实现
//返回是否启动成功 0成功,非0失败
signed char Pot_cbPos2HwStart(void);

//-------------------------判断硬件是否通讯中----------------------------
//返回0:正常结束，负：未通讯上结束，正：通讯过程中
signed char Pot_cbIsHwCommDoing(void);


#endif
