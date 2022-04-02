/***********************************************************************

                 ��Դ�ļ��͵�����ģʽ3x3�˵�ʵ��
��ģ�������Ӧ��
***********************************************************************/

#include "RP_Guide3X3.h"
#include "ePic.h"
#include "Plot.h"
#include <string.h>

/***********************************************************************
                             ��غ���ʵ��
***********************************************************************/

//-----------------------------ˢ�½���----------------------------
static void _Refresh(struct _RP_Guide3X3 *pGuide,
                      unsigned short Mask)
{
  unsigned char Focus = pGuide->Focus;
  for(unsigned char Pos = 0; Pos < 9; Pos++){
    if(!(Mask & (1 << Pos))) continue;//��ˢ��
    unsigned char IconId;
    #ifdef SUPPORT_RP_GUIDE3X3_PIC2//˫ͼ��ģʽʱ
    if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_PIC2){
      if(Pos == Focus){//����ʱ
        if(Mask & 0x0800) IconId = 0;//����ǿ����ʾ��ѡ���У����ڶ���
        else IconId = 1; //ѡ��״̬
      }
      else IconId = 0;//�ǽ��㲻ѡ��
      IconId += pGuide->Desc.IconBase + Pos * 2;
    }
    else
    #endif //SUPPORT_RP_GUIDE3X3_PIC2
      IconId = pGuide->Desc.IconBase + Pos;// ����ģʽֱ�Ӷ�Ӧ
    
    unsigned long Handle = pGuide->Desc.Handle;
    //��ͼ��ID�ҵ�ͼ����
    const unsigned char *ePic = RP_Guide3X3_cbGetIcon(Handle, IconId);
    if((ePic == NULL) || (RP_Guide3X3_cbGetIconSize(Handle, IconId) == 0))
      return; //�쳣
      
    //==================��ͼƬ�� �ҵ�x,y����ɫ����Ϣ==================
    Plot_SetPenStyle(pGuide->Desc.Bg);   
    ePic_ePicBuf(ePic, RP_cbGetIconSize(Handle,IconId));//��ʼ��ePic    
    unsigned short pw = pGuide->Desc.Rect.w / 3; //��ռ���
    if(ePicBuf.Header.w >= pw) continue;//ͼƬ��Ϣ�쳣
    unsigned short x = (pw - ePicBuf.Header.w) / 2;
    x  += (Pos % 3) * pw + pGuide->Desc.Rect.x;
    unsigned short ph = pGuide->Desc.Rect.h / 3; //��ռ�߶�
    if(ePicBuf.Header.h >= ph) continue;//ͼƬ��Ϣ�쳣
    unsigned short y = (ph - ePicBuf.Header.h) / 2;
    y  += (Pos / 3) * ph + pGuide->Desc.Rect.y;
    if(Pos == Focus){//����ʱ
      #ifdef SUPPORT_RP_GUIDE3X3_POP//����ģʽ: ͼ���������ƶ�2������
      if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_POP){
        x -= 2;
        y -= 2;
      }
      #endif
      #ifdef SUPPORT_RP_GUIDE3X3_PUSH//����ģʽ: ͼ���������ƶ�2������
      if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_PUSH){
        x += 2;
        y += 2;
      }
      #endif
    }//end ����ʱ
    //����ͼ���ɫ������ePicģʽ
    Plot_SetPenColor(RP_Guide3X3_cbGetIconFg(Handle,IconId));
    ePic_Plot(x,y);//��ͼ 
    
    if((Pos != pGuide->Focus) && (Pos != pGuide->PrvFocus)) continue;
    
    //=====================����ʱ������ʽ���ڴ���=========================
    #ifdef SUPPORT_RP_GUIDE3X3_RECT //���ģʽ:
    if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_RECT){
      if(Pos == pGuide->Focus) // ��������Fg[0]������Fg[1]ɫ��
        Plot_SetPenColor(pGuide->Desc.Fg[1]);
      else// ����ϴν���ʱɫ��
        Plot_SetPenColor(pGuide->Desc.Bg);
      Plot_Rect(x - 1,y - 1, ePicBuf.Header.w + 2, ePicBuf.Header.h + 2);
      if(pGuide->Desc.Fg[0])//˫ɫ��
        Plot_Rect(x - 2,y - 2, ePicBuf.Header.w + 4, ePicBuf.Header.h + 4);
      continue;
    }
    #endif //SUPPORT_RP_GUIDE3X3_RECT
    
    #ifdef SUPPORT_RP_GUIDE3X3_POP//����ģʽ:
    if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_POP){
      //ȡ������ʱ����ȥ��������ϲ�δ��������ͼ��ۼ�
      if(Pos == pGuide->PrvFocus){
        Plot_FullRect(x - 2, y - 2, 2, ePicBuf.Header.h);
        Plot_FullRect(x - 2, y - 2, ePicBuf.Header.w,2);
        continue;
      }
      //����ʱ,�Ҳ����²�����Fg[0]Fg[1]ɫ����������ָ����Ӱ
      unsigned short Offset = x + ePicBuf.Header.w;
      Plot_SetPenColor(pGuide->Desc.Fg[0]);
      Plot_LineV(Offset, y+1,ePicBuf.Header.h);
      Plot_SetPenColor(pGuide->Desc.Fg[1]);
      Plot_LineV(Offset + 1,y + 2,ePicBuf.Header.h);
      Offset = y + ePicBuf.Header.h;
      Plot_SetPenColor(pGuide->Desc.Fg[0]);      
      Plot_LineH(x + 1, Offset, ePicBuf.Header.w);
      Plot_SetPenColor(pGuide->Desc.Fg[1]);         
      Plot_LineH(x + 2, Offset + 1, ePicBuf.Header.w);
      continue;
    }
    #endif //SUPPORT_RP_GUIDE3X3_POP

     
    #ifdef SUPPORT_RP_GUIDE3X3_PUSH //����ģʽ:     
    if(pGuide->Desc.Style == RP_GUIDE3X3_STYLE_PUSH){
      //ȡ������ʱ����ȥ���Ҳ����²�δ��������ͼ��ۼ�
      if(Pos == pGuide->PrvFocus){
        Plot_FullRect(x + ePicBuf.Header.w, y + 2, 2, ePicBuf.Header.h);
        Plot_FullRect(x + 2, y + ePicBuf.Header.h, ePicBuf.Header.w,2);
        continue;
      }
      //ͼ���������ƶ�2�����أ�ͬʱ������ϲ����ӹ���������ָ����Ӱ
      Plot_SetPenColor(pGuide->Desc.Fg[0]);
      Plot_LineV(x - 1, y - 1,ePicBuf.Header.h);
      Plot_SetPenColor(pGuide->Desc.Fg[1]);
      Plot_LineV(x - 2, y - 2,ePicBuf.Header.h);
      Plot_SetPenColor(pGuide->Desc.Fg[0]);      
      Plot_LineH(x - 1, y - 1, ePicBuf.Header.w);
      Plot_SetPenColor(pGuide->Desc.Fg[1]);         
      Plot_LineH(x - 2, y - 2, ePicBuf.Header.w);
    }
    #endif //SUPPORT_RP_GUIDE3X3_PUSH
  }//end for
}

//-----------------------------��ʼ������----------------------------
void RP_Guide3X3_Init(struct _RP_Guide3X3 *pGuide,//ȷ��Desc��ʼ��
                      signed char DefaultFocus) //Ĭ�Ͻ���,�����м�
{
  //����ʱ��
  if(pGuide->Desc.Cfg & RP_GUIDE3X3_AUTO_ENTER)
    pGuide->Countdown = RP_GUIDE3X3_AUTO_SEL_OV;
  else pGuide->Countdown = 0;
  pGuide->DyncTimer = 0;
  //����ģʽ������߽ǹ���
  if(pGuide->Desc.Cfg & RP_GUIDE3X3_FAST_MODE) 
    pGuide->Desc.Cfg &= ~((1 << 0)|(1 << 2)|(1 << 6)|(1 << 8));
  
  //���Focus��ʼ������ֹ����ʼ��ʱ����
  if((DefaultFocus < 0) || (DefaultFocus >= 9))//�������м�
    DefaultFocus = 5; 
  unsigned short Cfg = pGuide->Desc.Cfg;
  signed char Focus = DefaultFocus;
  for(; Focus >= 0; Focus--){//��ǰ�����
    if(Cfg & (1 << Focus)){//�ҵ���
      pGuide->Focus = Focus;
      _Refresh(pGuide,pGuide->Desc.Cfg);//ȫ��ˢ��
      return; 
    }
  }
  for(; Focus < 9; Focus++){//�ú������
    if(Cfg & (1 << Focus)){//�ҵ���
      pGuide->Focus = Focus;
      _Refresh(pGuide,pGuide->Desc.Cfg);//ȫ��ˢ��
      return; 
    }
  }
  //ȫ������ѡ��,ǿ�ƿ����м�
  pGuide->Desc.Cfg |= (1 << 5);
  pGuide->Focus = 5;
  pGuide->PrvFocus = 0xff;//�ϴ��޽���
  return;
}

//-----------------------------����������----------------------------
//GuideKey����Ϊ: ȷ��0��1��2��3��4,
void RP_Guide3X3_Key(struct _RP_Guide3X3 *pGuide,
                      unsigned char GuideKey)
{
  unsigned char Focus = pGuide->Focus;
  unsigned char OrgFocus = Focus;  
  unsigned short Cfg = pGuide->Desc.Cfg;
  switch(GuideKey){
    case 0: //ȷ�ϼ���
      RP_Guide3X3_cbSelFinal(pGuide->Desc.Handle, Focus);
      return;
    case 1: //�������1��
      if(Focus % 3) Focus--; //����1��
      else break; //�����������
      //����ѡ��ʱ���
      if(Cfg & (1 << Focus)) break; 
      //������ѡ��ʱ��������
      if(Focus % 3) Focus--;
      else Focus = OrgFocus; //������������ѡ���軹ԭ
      if(Cfg & (1 << Focus)) break; //��������ѡ����
      else Focus = OrgFocus; //��������ѡ�񣬻�ԭ
      break;
    case 2: //�ϼ�����1��
      if(Focus >= 3) Focus -= 3; //����1��
      else break; //�������ϱ���
      //����ѡ��ʱ���
      if(Cfg & (1 << Focus)) break; 
      //������ѡ��ʱ��������
      if(Focus >= 3) Focus -= 3; //����1��
      else Focus = OrgFocus; //�������ϲ�����ѡ���軹ԭ
      if(Cfg & (1 << Focus)) break; //��������ѡ����
      else Focus = OrgFocus; //���ϲ�����ѡ�񣬻�ԭ
      break;    
    case 3: //�Ҽ�����1��
      if((Focus % 3) != 2) Focus++; //����1��
      else break; //�������ұ���
      //����ѡ��ʱ���
      if(Cfg & (1 << Focus)) break; 
      //������ѡ��ʱ��������
      if((Focus % 3) != 2) Focus++; //����1��
      else Focus = OrgFocus; //������������ѡ���軹ԭ
      if(Cfg & (1 << Focus)) break; //��������ѡ����
      else Focus = OrgFocus; //��������ѡ�񣬻�ԭ
      break;
    case 4: //�¼�����1��
      if(Focus < 6) Focus += 3; //����1��
      else break; //�������±���
      //����ѡ��ʱ���
      if(Cfg & (1 << Focus)) break; 
      //������ѡ��ʱ��������
      if(Focus < 6) Focus += 3; //����1��
      else Focus = OrgFocus; //�������²�����ѡ���軹ԭ
      if(Cfg & (1 << Focus)) break; //��������ѡ����
      else Focus = OrgFocus; //���ϲ�����ѡ�񣬻�ԭ
      break;
    default: return; //��������֧�� 
  }
  
  if(Focus == OrgFocus) return;//ѡ��δ�ı�
  pGuide->Focus = Focus;
  pGuide->PrvFocus = OrgFocus;  
    
  if(Cfg & RP_GUIDE3X3_AUTO_ENTER) //�Զ�ѡ��ʱ
    RP_Guide3X3_cbSelFinal(pGuide->Desc.Handle, Focus);
  else{//ˢ�±䶯ǰ��ͼ��
    _Refresh(pGuide, (1 << Focus) | (1 << OrgFocus));
    pGuide->PrvFocus = 0xff; //�ϴ�ˢ�����
  }
}

//----------------------------��������----------------------------
//1s���ҵ���һ��
void RP_Guide3X3_Task(struct _RP_Guide3X3 *pGuide)
{
  //�Զ�ѡ�񵹼�ʱ������
  if(pGuide->Countdown){
    pGuide->Countdown--;
    if(!pGuide->Countdown){//ʱ�䵽��
      RP_Guide3X3_cbSelFinal(pGuide->Desc.Handle, pGuide->Focus);
      return;
    }
  }
  //��������: ������ˢ�¶���
  if(pGuide->Desc.Cfg & RP_GUIDE3X3_EN_DYNC){
    pGuide->DyncTimer++;
    unsigned short Mask = (1 << pGuide->Focus);
    if(pGuide->DyncTimer & 1) Mask |= 0x0800; //�÷�ѡ��ͼ��ˢ��
    _Refresh(pGuide, Mask);//ˢ����ʾ
  }
}


