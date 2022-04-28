/***************************************************************************

           ��Դ�ļ����ı��ļ�-��ʹ��TMenuBufʱ��ʵ��
                         
****************************************************************************/

#include "RP_TxtTMenu.h"

#include "TMenu_Buf.h"
#include "TMenuBuf.h"
#include "Rid.h"

#include "stringEx.h"
#include <string.h>
  
//-------------------------------��ʼ��-------------------------------------
//���ظ��쳣
signed char RP_TxtTMenuInit(unsigned char Rid)
{
  const unsigned char *pBase = RID_pGetData(Rid);
  TMenuBuf.Para = (unsigned long)pBase; //��Ż�ַ
  TMenuBuf.Lut[0] = Rid; //RID  
  TMenuBuf.Lut[1] = 0;   //��ҳδ����
  TMenuBuf.Lut[2] = 0;   //��ǰҳ
  if(pBase == RI_NULL) return -1;//�쳣
  if(RID_GetIdCount(pBase) == 0) return -1;//���ַ��쳣
  return 0;
}

//------------------------------�ص�����ʵ��---------------------------------
void RP_TxtTMenuNotify(unsigned char Type, void *pv)
{
  struct _BufUser *pUser = pv;

  if(TMenuBuf.Lut[1] == 0){//��ҳδ����ʱ
    const unsigned char *pBase = (const unsigned char *)TMenuBuf.Para; 
    unsigned short LineCount = RID_GetIdCount(pBase);
    TMenuBuf.Lut[1] = (LineCount + pUser->h - 1) / pUser->h;//������ҳ
  }
  //������Ӧ
  if(Type == TM_NOTIFY_BUF_KEY){
    switch(pUser->Key){
      case TGUI_KEY_ESCAPE: //�˳���
        pUser->Notify = TM_BUF_NOTIFY_EXIT_FLAG; 
        return;
      case TGUI_KEY_UP:
      case TGUI_KEY_LEFT: 
      case TGUI_KEY_PAGEUP: //��Ϊ��һҳ
        if(TMenuBuf.Lut[2]) TMenuBuf.Lut[2]--;
        break;
      case TGUI_KEY_DOWN:
      case TGUI_KEY_RIGHT: 
      case TGUI_KEY_PAGEDOWN: //��Ϊ��һҳ        
        if(TMenuBuf.Lut[2] < (TMenuBuf.Lut[1] - 1)) TMenuBuf.Lut[2]++;
        break;        
    }
    pUser->Notify = TM_BUF_NOTIFY_DISP; //����������ʾ
    return;
  }
  //������ʾ������
  if(Type == TM_NOTIFY_GET_DATA){
    char *pBuf = pUser->pBuf;
    unsigned char w = pUser->w;
    unsigned short CurLine = TMenuBuf.Lut[2] * pUser->h;
    const unsigned char *pBase = (const unsigned char *)TMenuBuf.Para;     
    unsigned short LineCount = RID_GetIdCount(pBase);
    for(unsigned char i = 0; i < pUser->h; i++, CurLine++){
      char *pEnd;
      if(CurLine >= LineCount) pEnd = pBuf; //������պ��ַ�
      else pEnd= strcpyEx(pBuf, (const char *)RI_pGetData(pBase, CurLine), w);
      pBuf += w; //������ʼ
      memset(pEnd, ' ', pBuf - pEnd);//δ��һ������̿ո�
    }
  }
}

