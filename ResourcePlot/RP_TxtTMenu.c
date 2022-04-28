/***************************************************************************

           资源文件型文本文件-在使用TMenuBuf时的实现
                         
****************************************************************************/

#include "RP_TxtTMenu.h"

#include "TMenu_Buf.h"
#include "TMenuBuf.h"
#include "Rid.h"

#include "stringEx.h"
#include <string.h>
  
//-------------------------------初始化-------------------------------------
//返回负异常
signed char RP_TxtTMenuInit(unsigned char Rid)
{
  const unsigned char *pBase = RID_pGetData(Rid);
  TMenuBuf.Para = (unsigned long)pBase; //存放基址
  TMenuBuf.Lut[0] = Rid; //RID  
  TMenuBuf.Lut[1] = 0;   //总页未更新
  TMenuBuf.Lut[2] = 0;   //当前页
  if(pBase == RI_NULL) return -1;//异常
  if(RID_GetIdCount(pBase) == 0) return -1;//无字符异常
  return 0;
}

//------------------------------回调函数实现---------------------------------
void RP_TxtTMenuNotify(unsigned char Type, void *pv)
{
  struct _BufUser *pUser = pv;

  if(TMenuBuf.Lut[1] == 0){//总页未更新时
    const unsigned char *pBase = (const unsigned char *)TMenuBuf.Para; 
    unsigned short LineCount = RID_GetIdCount(pBase);
    TMenuBuf.Lut[1] = (LineCount + pUser->h - 1) / pUser->h;//更新总页
  }
  //按键响应
  if(Type == TM_NOTIFY_BUF_KEY){
    switch(pUser->Key){
      case TGUI_KEY_ESCAPE: //退出键
        pUser->Notify = TM_BUF_NOTIFY_EXIT_FLAG; 
        return;
      case TGUI_KEY_UP:
      case TGUI_KEY_LEFT: 
      case TGUI_KEY_PAGEUP: //均为上一页
        if(TMenuBuf.Lut[2]) TMenuBuf.Lut[2]--;
        break;
      case TGUI_KEY_DOWN:
      case TGUI_KEY_RIGHT: 
      case TGUI_KEY_PAGEDOWN: //均为下一页        
        if(TMenuBuf.Lut[2] < (TMenuBuf.Lut[1] - 1)) TMenuBuf.Lut[2]++;
        break;        
    }
    pUser->Notify = TM_BUF_NOTIFY_DISP; //更新整屏显示
    return;
  }
  //更新显示缓冲区
  if(Type == TM_NOTIFY_GET_DATA){
    char *pBuf = pUser->pBuf;
    unsigned char w = pUser->w;
    unsigned short CurLine = TMenuBuf.Lut[2] * pUser->h;
    const unsigned char *pBase = (const unsigned char *)TMenuBuf.Para;     
    unsigned short LineCount = RID_GetIdCount(pBase);
    for(unsigned char i = 0; i < pUser->h; i++, CurLine++){
      char *pEnd;
      if(CurLine >= LineCount) pEnd = pBuf; //用于清空后部字符
      else pEnd= strcpyEx(pBuf, (const char *)RI_pGetData(pBase, CurLine), w);
      pBuf += w; //下行起始
      memset(pEnd, ' ', pBuf - pEnd);//未满一行填充后继空格
    }
  }
}

