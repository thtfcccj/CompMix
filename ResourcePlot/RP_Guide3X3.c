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
static void _Refresh(const struct _RP_Guide3X3 *pGuide,
                      unsigned short Mask)
{
  unsigned char Focus = pGuide->Focus;
  for(unsigned char Pos = 0; Pos < 9; Pos++){
    if(!(Mask & (1 << Pos))) continue;//��ˢ��
    unsigned char IconId;
    if(Pos == Focus){//����ʱ
      if(Mask & 0x0800) IconId = 0;//����ǿ����ʾ��ѡ���У����ڶ���
      else IconId = 1; //ѡ��״̬
    }
    else IconId = 0;//�ǽ��㲻ѡ��
    IconId += pGuide->Desc.IconBase + Pos * 2;
    unsigned long Handle = pGuide->Desc.Handle;
    const unsigned char *ePic = //��ͼ��ID�ҵ�ͼ����
       RP_Guide3X3_cbGetIcon(Handle, IconId);
    if((ePic == NULL) || (RP_Guide3X3_cbGetIconSize(Handle, IconId) == 0))
      
    //��ͼƬ�� �ҵ�x,y����ɫ����Ϣ
    //���õ�ɫ��
    Plot_SetPenStyle(pGuide->Desc.Bg);    
    Plot_SetPenColor(RP_Guide3X3_cbGetIconFg(Handle,IconId));
    ePic_ePicBuf(ePic, RP_cbGetIconSize(Handle,IconId));//��ʼ��ePic
    unsigned short pw = pGuide->Desc.Rect.w / 3; //��ռ���
    if(ePicBuf.Header.w >= pw) continue;//ͼƬ��Ϣ�쳣
    unsigned short x = (pw - ePicBuf.Header.w) / 2;
    x  += (Pos % 3) * pw + pGuide->Desc.Rect.x;
    unsigned short ph = pGuide->Desc.Rect.h / 3; //��ռ�߶�
    if(ePicBuf.Header.h >= ph) continue;//ͼƬ��Ϣ�쳣
    unsigned short y = (ph - ePicBuf.Header.h) / 2;
    y  += (Pos / 3) * ph + pGuide->Desc.Rect.y;
    ePic_Plot(x,y);//��ͼ 
  }
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
    
  if(Cfg & RP_GUIDE3X3_AUTO_ENTER) //�Զ�ѡ��ʱ
    RP_Guide3X3_cbSelFinal(pGuide->Desc.Handle, Focus);
  else _Refresh(pGuide, (1 << Focus) | (1 << OrgFocus));//ˢ�±䶯ǰ��ͼ��
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


