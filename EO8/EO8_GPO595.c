/***************************************************************************

        扩展输出8位()接口=>在HC595芯片上，通过EO8驱动程序实现

***************************************************************************/
#include "EO8.h"
#include "Delay.h"
#include "IoCtrl.h"  //IO控制
#include <string.h>

struct _EO8  EO8;//直接单例化

//---------------------------IoCtrl需实现的回调----------------------------
/*/时钟IO:
#define EO8_cbSetClk() do{}while(0)
#define EO8_cbClrClk() do{}while(0)
//数据IO:
#define EO8_cbSetDio() do{}while(0)
#define EO8_cbClrDio() do{}while(0)
//锁存信号:
#define EO8_cbSetLe() do{}while(0)
#define EO8_cbClrLe() do{}while(0)
//延时时间，us为单位
#define SEO8_cbBitDelay()      1 //数据保持时间
#define SEO8_cbKeepLeDelay()   1 //锁存信号保持时间
#define SEO8_cbCancelLeDelay() 0 //锁存信号取消时保持时间

//IO初始化
#define EO8_cbIoInit()  do{}while(0)
//IO准备,将在发送前调用 (CLK,DI未复用其它功能时可实现为空)
#define EO8_cbIoStart() do{}while(0)
//IO结束,将在结束时调用 (CLK,DI未复用,或不在中断里调用时可实现为空)
#define EO8_cbIoEnd() do{}while(0) 

*/

/***************************************************************************
                              相关函数
***************************************************************************/

//------------------------------初始化函数------------------------------
void EO8_Init(void)
{
  EO8_cbIoInit();//IO初始化
  EO8.O = EO8_DEFAULT;
  //EO8驱动可能为非独占式，不在此初始化
  EO8_FaskTask();//开机立即更新一次
}

//-----------------------------快速任务函数-----------------------------
//最长调用时间建议<=10ms
void EO8_FaskTask(void)
{
  if(EO8.PrvO == EO8.O) return;
  EO8_cbIoStart(); //IO准备(CLK,DI未复用其它功能时可实现为空)
  //配合时钟写数据:
  unsigned char Data = EO8.O;
  for(unsigned char Shift = 0x80; Shift > 0; Shift >>= 1){
    //准备好需移出的数据
    if(Data & Shift) EO8_cbSetDio();
    else EO8_cbClrDio();   
    EO8_cbClrClk();//时钟起始沿    
    DelayUs(SEO8_cbBitDelay());  //数据保持
    EO8_cbSetClk(); //时钟结束沿
    DelayUs(SEO8_cbBitDelay());  //数据保持    
  }
  //给LE信号锁存数据:
  EO8_cbSetLe(); //高电平锁存
  DelayUs(SEO8_cbKeepLeDelay());  //锁存保持
  EO8_cbClrLe(); //低电平取消  
  DelayUs(SEO8_cbCancelLeDelay());  //取消时保持
  EO8_cbIoEnd(); //IO结束(CLK,DI未复用,或不在中断里调用时可实现为空)
  EO8.PrvO = EO8.O;
}

