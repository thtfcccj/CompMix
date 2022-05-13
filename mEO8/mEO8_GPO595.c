/***************************************************************************

        扩展输出8位()接口=>在HC595芯片上，通过mEO8驱动程序实现

***************************************************************************/
#include "mEO8.h"
#include "Delay.h"
#include "IoCtrl.h"  //IO控制
#include <string.h>

//---------------------------IoCtrl需实现的回调----------------------------
/*/时钟IO:
#define mEO8_cbSetClk() do{}while(0)
#define mEO8_cbClrClk() do{}while(0)
//数据IO:
#define mEO8_cbSetDio() do{}while(0)
#define mEO8_cbClrDio() do{}while(0)

//锁存信号(多例化时，应实现为函数, 单例化可直接实现)
#define mEO8_cbSetLe(eo) do{}while(0)
#define mEO8_cbClrLe(eo) do{}while(0)

//延时时间，us为单位
#define mEO8_cbBitDelay()      1 //数据保持时间
#define mEO8_cbKeepLeDelay()   1 //锁存信号保持时间
#define mEO8_cbCancelLeDelay() 0 //锁存信号取消时保持时间

//IO统一初始化：不在此调用
  
//IO准备,将在发送前调用 (CLK,DI未复用其它功能时可实现为空)
#define mEO8_cbIoStart(pEo) do{}while(0)
//IO结束,将在结束时调用 (CLK,DI未复用,或不在中断里调用时可实现为空)
#define mEO8_cbIoEnd(pEo) do{}while(0) 
*/

/***************************************************************************
                              相关函数
***************************************************************************/

//------------------------------初始化函数------------------------------
void mEO8_Init(struct _mEO8 *pEo,
                unsigned char DefaultO) //默认输出
{
  pEo->O = DefaultO;
  pEo->PrvO = DefaultO + 1;  
  mEO8_FaskTask(pEo);//开机立即更新一次
}

//-----------------------------快速任务函数-----------------------------
//最长调用时间建议<=10ms
void mEO8_FaskTask(struct _mEO8 *pEo)
{
  if(pEo->PrvO == pEo->O) return;
  mEO8_cbIoStart(pEo); //IO准备(CLK,DI未复用其它功能时可实现为空,也可用于临界区指示)
  //配合时钟写数据:
  unsigned char Data = pEo->O;
  for(unsigned char Shift = 0x80; Shift > 0; Shift >>= 1){
    //准备好需移出的数据
    if(Data & Shift) mEO8_cbSetDio();
    else mEO8_cbClrDio();   
    mEO8_cbClrClk();//时钟起始沿    
    DelayUs(mEO8_cbBitDelay());  //数据保持
    mEO8_cbSetClk(); //时钟结束沿
    DelayUs(mEO8_cbBitDelay());  //数据保持    
  }
  //给LE信号锁存数据:
  mEO8_cbSetLe(pEo); //高电平锁存
  DelayUs(mEO8_cbKeepLeDelay());  //锁存保持
  mEO8_cbClrLe(pEo); //低电平取消  
  DelayUs(mEO8_cbCancelLeDelay());  //取消时保持
  mEO8_cbIoEnd(pEo); //IO结束(CLK,DI未复用,或不在中断里调用时可实现为空,也可用于临界区指示)
  pEo->PrvO = pEo->O;
}

