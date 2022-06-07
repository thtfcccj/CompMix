/***********************************************************************

                 ���������ͼģ��ʵ��
��ģ�������Ӧ��
***********************************************************************/

#include "RP_AryArea.h"
#include "ePic.h"
#include "Plot.h"
#include "RP_FixArea.h"
#include <string.h>

/***********************************************************************
                             ��غ���ʵ��
***********************************************************************/

//---------------------------����������------------------------------
static void _PotItem(struct _RP_AryArea *pArea,
                      unsigned long Handle,
                      unsigned short x,
                      unsigned short y,
                      unsigned char Pos,//��ҳ����λ��
                      unsigned char PlotMask)
{
  pArea->FDesc.Rect.x = x;
  pArea->FDesc.Rect.y = y;
  pArea->FDesc.IconBase = pArea->STypeInfo.IconBase;
  pArea->FDesc.IconCount = pArea->STypeInfo.IconCount;
  pArea->FDesc.ParaBase = pArea->STypeInfo.ParaBase;
  pArea->FDesc.ParaCount = pArea->STypeInfo.ParaCount;
  //���ù̶������ͼ
  const struct _RpAryAreaDesc *pAryDesc = pArea->pAryDesc;   //������
  if(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN){//����ͼ�仯����ʱ�滻����
    if(Pos == pArea->Focus) pArea->FDesc.cBg = pAryDesc->cFocus;
    else pArea->FDesc.cBg = pAryDesc->cBg;
  }
  RP_FixArea(Handle, PlotMask, &pArea->FDesc);
}

//--------------------------���µ�ǰ������------------------------------
//����0����,����δ����
static signed char  _UpdateSTypeInfo(struct _RP_AryArea *pArea,
                                       unsigned long Handle,
                                       unsigned short AryId)
{
  signed char Resumme = RP_AryArea_cbGetSTypeInfo(Handle, AryId,
                                                  &pArea->STypeInfo);
  //������
  if(!Resumme){
    pArea->AryCount = AryId;//��̬����
  }
  return Resumme;
}

//-------------------------------ˢ�½���------------------------------
static void _Refresh(struct _RP_AryArea *pArea,
                      signed char IsRefreshFocus)//ˢ�½��㣬����ΪPlotMask
{
  unsigned char PlotMask;
  if(IsRefreshFocus) PlotMask = 0x0F | 0x10; //ˢ�½���λ��
  else{
    PlotMask = pArea->PlotMask;
    pArea->PlotMask = 0;
    PlotMask &= ~0x10;//��ֹ�쳣��λ
  }
  const struct _RpMainAreaDesc *pMainDesc = pArea->pMainDesc; //��������
  unsigned short x = pMainDesc->Rect.x;
  unsigned short y = pMainDesc->Rect.y;  
  if(PlotMask & 0x80){//�������屳��
    Plot_SetBrushColor(pMainDesc->cBg);
    Plot_FullRect(x, y,pMainDesc->Rect.w,pMainDesc->Rect.h);
    PlotMask = 0x0f;//������
  }
  
  const struct _RpAryAreaDesc *pAryDesc = pArea->pAryDesc;   //������
  unsigned char RowCount = pAryDesc->RowCount;
  //�õ���������x��ʼ
  unsigned short pw = pAryDesc->Rect.w; //��ռ���
  x += pAryDesc->Rect.x; //x��ʼ
  
  //�õ���߶�����y��ʼ
  unsigned short ph = pAryDesc->Rect.h; //��ռ�߶�
  y += pAryDesc->Rect.y; //y��ʼ  
  
  //��ʼ��FDesc���ι̶�����
  pArea->FDesc.Rect.w = pAryDesc->Rect.w;
  pArea->FDesc.Rect.h = pAryDesc->Rect.h;
  pArea->FDesc.cBg = pAryDesc->cBg;
  pArea->FDesc.IconBgId = pAryDesc->IconBgId;
  //��ʼ��RP_FixArea��Ҫ��PlotMask��Handle;
  unsigned long Handle = pArea->Handle;
  if(PlotMask == 0){//��ˢ��ʵʱ����ʱ
    Handle |= RP_HANDLE_REAL_REFRESH;
    PlotMask = 0x0C; //����������������ͼ
  }
  if(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN)
    PlotMask &= ~0x01;//��Ϊ����ͼƬ�����ñ���ͼƬ���汳��ͼ
  
  unsigned short AryId = pArea->AryStart;
  unsigned short FocusAryId; //��ѡAryid
  if(pAryDesc->BitInfo & RP_ITEM_DIS_FOCUS) FocusAryId = 0xffff;//�޽���
  else FocusAryId = AryId + pArea->Focus;
  unsigned char Pos;
  //========================�������½���λ��======================
  if(IsRefreshFocus){
    if(pAryDesc->BitInfo & RP_ITEM_DIS_FOCUS) return;//�޽���
    //����ʧ��ʱ��ʾ�����ˣ��ָ�
    if(!_UpdateSTypeInfo(pArea, Handle, FocusAryId)){
      pArea->Focus = pArea->PrvFocus;
      return;
    }
    Pos = pArea->Focus;
    //�߿�ʽ����ʱ
    if(!(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN)){
      Plot_SetPenColor(pAryDesc->cFocus);
      Plot_Rect(x + pw * (Pos % RowCount) - 1, 
                y + ph * (Pos / RowCount) - 1,
                pArea->FDesc.Rect.w + 2,
                pArea->FDesc.Rect.h + 2);
      if(Pos == pArea->PrvFocus) return; //��������һ��
      //���±�ҳ�ϴεĽ�����������
      Pos = pArea->PrvFocus;
      pArea->PrvFocus = pArea->Focus; //������
      Plot_SetPenColor(pAryDesc->cBg); //�ñ���ɫ���
      Plot_Rect(x + pw * (Pos % RowCount) - 1, 
                y + ph * (Pos / RowCount) - 1,
                pArea->FDesc.Rect.w + 2,
                pArea->FDesc.Rect.h + 2);      
      return; //��������
    }
    _PotItem(pArea, Handle | FocusAryId | pArea->STypeInfo.SType,
             x + pw * (Pos % RowCount),
             y + ph * (Pos / RowCount),
             Pos,PlotMask); //0x10: �ڶ�����Ϊѡ��
    if(Pos == pArea->PrvFocus) return; //��������һ��
    //���±�ҳ�ϴεĽ�����������
    Pos = pArea->PrvFocus;
    pArea->PrvFocus = pArea->Focus; //������    
    AryId += Pos;   //�ϴν���

    _UpdateSTypeInfo(pArea, Handle, AryId);
    _PotItem(pArea, Handle | AryId | pArea->STypeInfo.SType,
             x + pw * (Pos % RowCount),
             y + ph * (Pos / RowCount),
             Pos, PlotMask & 0x0f);  //δѡ��
    return;
  }
  //========================ɨ���������=========================
  unsigned short Count = RowCount * pAryDesc->ColCount;//ҳ����������
  if(Count > 255) return ;//�쳣
  
  Pos = 0; //��ͷ��ʼ
  for(; Pos < Count; Pos++, AryId++){
    if(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN){//����ͼ�仯����ʱ�滻����
      if(Pos == pArea->Focus) pArea->FDesc.cBg = pAryDesc->cFocus;
      else pArea->FDesc.cBg = pAryDesc->cBg;
    }
    //����ʧ��ʱ��ʾ������
    if(!_UpdateSTypeInfo(pArea, Handle, AryId)){
      break;
    }
    //��֯FDesc��x,y�����Ϣ,_PotItem()ֱ��չ�������Ч�ʣ�
    pArea->FDesc.Rect.x = x + pw * (Pos % RowCount);
    pArea->FDesc.Rect.y = y + ph * (Pos / RowCount);
    pArea->FDesc.IconBase = pArea->STypeInfo.IconBase;
    pArea->FDesc.IconCount = pArea->STypeInfo.IconCount;
    pArea->FDesc.ParaBase = pArea->STypeInfo.ParaBase;
    pArea->FDesc.ParaCount = pArea->STypeInfo.ParaCount;
    //���ù̶������ͼ
    unsigned char CurPlotMask = PlotMask;
    if((AryId == FocusAryId) && (PlotMask & 0x03))
      CurPlotMask |= 0x10;//���Ʊ���������ʱ,�ڶ�����Ϊѡ��
    RP_FixArea(Handle | AryId | pArea->STypeInfo.SType, 
               CurPlotMask, &pArea->FDesc);
  }//end for
  if(Pos >= Count) return;
  if(PlotMask == 0xff) return;//������������
  //������������ֹ��ҳʱ����(ʵ��Ӧˢ����ҳ) 
  Plot_SetBrushColor(pMainDesc->cBg); 
  for(; Pos < Count; Pos++){
    pArea->FDesc.Rect.x = x + pw * (Pos % RowCount);
    pArea->FDesc.Rect.y = y + ph * (Pos / RowCount);
    Plot_FullRect(pArea->FDesc.Rect.x-1, 
                  pArea->FDesc.Rect.y-1,
                  pArea->FDesc.Rect.w+2,
                  pArea->FDesc.Rect.h+2);
  }
}

//------------------------ˢ�½���������----------------------------
//Refresh()�������
void _RefreshFocus(struct _RP_AryArea *pArea)
{
  //ͼƬ������ʱ���Ѿ����¹��ˡ�
  if(pArea->pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN){
    pArea->PrvFocus = pArea->Focus; //������ 
    return;
  }
  _Refresh(pArea, 1); //�赥������
}

//-----------------------------��λ��ض�ʱ��----------------------------
void _ResetTimer(struct _RP_AryArea *pArea)
{
  pArea->AutoPageTimer = RP_ARY_AREA_KEY_KEEP_OV + 
                         RP_ARY_AREA_AUTO_SCREEN_OV;
  pArea->RefreshTimer = RP_ARY_AREA_REFRESH_PERIOD;  
}


//-----------------------------��ʼ������----------------------------
void RP_AryArea_Init(struct _RP_AryArea *pArea,    //���ṹ
                     const struct _RpMainAreaDesc *pMainDesc,//��������
                     const struct _RpAryAreaDesc *pAryDesc, //������                     
                     unsigned long Handle, //����� 
                     unsigned short AryCount)   //��ʼ��ʱ���д�С
{
  memset(pArea, 0, sizeof(struct _RP_AryArea));
  pArea->pMainDesc = pMainDesc;
  pArea->pAryDesc = pAryDesc;  
  pArea->Handle = Handle;  
  pArea->AryCount = AryCount;
  _ResetTimer(pArea);
  RP_AryArea_RefreshAll(pArea);//ˢ������
}

//--------------------------�Ϸ�ҳ����----------------------------
//�����Ƿ���Ӧ
static signed char _PageUp(struct _RP_AryArea *pArea,
                           unsigned short Count)
{
  if(pArea->AryCount < Count) return 0;//ֻ��һҳ������
  
  if(pArea->AryStart >= Count) pArea->AryStart -= Count;
  else//�ػ������һҳ
    pArea->AryStart = ((pArea->AryCount - 1) / Count) * Count;
  return 1;
}               

//--------------------------�·�ҳ����----------------------------
//�����Ƿ���Ӧ
static signed char _PageDown(struct _RP_AryArea *pArea,
                              unsigned short Count)
{
  if(pArea->AryCount < Count) return 0;//ֻ��һҳ������
  if((pArea->AryStart + Count) < pArea->AryCount)//����ҳʱ
    pArea->AryStart += Count;
  else pArea->AryStart = 0;//���һҳʱ�ص���1ҳ
  
  return 1;
}

//-----------------------------����������----------------------------
//GuideKey����Ϊ: ȷ��0��1��2��3��4, 5��һҳ��6��һҳ,�����ּ�'1'-'9'
void RP_AryArea_Key(struct _RP_AryArea *pArea,
                      unsigned char GuideKey)
{
  const struct _RpAryAreaDesc *pAryDesc = pArea->pAryDesc;   //������
  unsigned char Focus = pArea->Focus; 
  unsigned char RowCount = pAryDesc->RowCount;
  unsigned char ColCount = pAryDesc->ColCount; 
  unsigned short Count = RowCount * ColCount;
  unsigned short PrvAryStart = pArea->AryStart;
  switch(GuideKey){
    case 0: //ȷ�ϼ���
      RP_AryArea_cbSelFinal(pArea->Handle, pArea->AryStart + Focus);
      _ResetTimer(pArea);
      return;
    case 1: //���
      if(RowCount >= 2){ //����ʱ��ʾ����ѡ��
        if(Focus % RowCount) Focus--; //����1��
        else Focus += RowCount - 1;//�ػ�������
        break;
      }
      //else �������Ϊ�Ϸ�ҳ��
    case 5: _PageUp(pArea, Count);break;//�Ϸ�ҳ��    
    case 3: //�Ҽ�
      if(RowCount >= 2){ //����ʱ��ʾ����ѡ��
        if((Focus % RowCount) < (RowCount - 1)) Focus++; //����1��
        else Focus -= Focus % RowCount;//�������ˣ��ػ�������
        break;
      }
      //else �Ҽ�����Ϊ�·�ҳ��
    case 6: _PageDown(pArea, Count);break;//�·�ҳ��    
    case 2: //�ϼ�
      if(Focus >= RowCount) Focus -= RowCount; //����1��
      else if(!pArea->AryStart) return;//�������ϼ��ػ������(��Ӧ����¼�������ʼ)
      else{//��һҳ���һ�Ŷ�Ӧλ��
        if(_PageUp(pArea, Count)){//��ҳ�ɹ���
          Focus += Count - RowCount;
        }
      }
      break; 
    case 4: //�¼�
      if((Focus + RowCount) < Count) Focus += RowCount; //����1��
      else{//��һҳ��1�Ŷ�Ӧλ��
        if(_PageDown(pArea, Count)){//��ҳ�ɹ���
          Focus = Focus % RowCount;
        }
      }
      break;  
    default: //������
      //��Ӧ���ּ�
      if((GuideKey >= '1') && (GuideKey <= '9')){
        Focus = GuideKey - '1';
      }
      else return; //��������֧�� 
  }
  pArea->Focus = Focus;
  _ResetTimer(pArea);
  //ҳ�仯
  if(PrvAryStart != pArea->AryStart){
    pArea->PlotMask = 0x7f; //ˢ������
    _Refresh(pArea, 0); //����ҳ��
    //����(��ĩҳ)����ʱ����
    unsigned short EndPos = pArea->AryCount - pArea->AryStart;
    if(Focus >= EndPos){
      pArea->Focus = EndPos - 1; //���һ��
      pArea->PrvFocus = pArea->Focus;//�������ϴ�
      _Refresh(pArea, 1); //�������½���
      return;
    }
    _RefreshFocus(pArea); //���½���
    return;
  }
  
  //������仯��
  if(Focus != pArea->PrvFocus){
    _Refresh(pArea, 1); //�������½���
  }
}

//----------------------------��������----------------------------
//1s���ҵ���һ��
void RP_AryArea_Task(struct _RP_AryArea *pArea)
{
  //�Զ���ҳ����
  if(pArea->AutoPageTimer){
    pArea->AutoPageTimer--;
    if(!pArea->AutoPageTimer){//ʱ�䵽��,��������
      RP_AryArea_Key(pArea, 6);
      pArea->AutoPageTimer = RP_ARY_AREA_AUTO_SCREEN_OV;//�ӹ������ʱ
      return;
    }
  }
  //��ͨ�����Զ�����
  if(pArea->RefreshTimer){
    pArea->RefreshTimer--;
    if(!pArea->RefreshTimer){//ʱ�䵽��,������ͨ���� 
      pArea->RefreshTimer = RP_ARY_AREA_REFRESH_PERIOD;
      pArea->PlotMask = 0x0f; //ˢ����ͨ����
      _Refresh(pArea, 0);     //�赥������
      return;
    }
  }
  //ʵʱ���ݸ���
  if(pArea->pAryDesc->BitInfo & RP_ITEM_REAL_REFRESH){
    pArea->PlotMask = 0; //ʵʱ����
    _Refresh(pArea, 0);  //�赥������  
  }
}

//-------------------------------ˢ�����к���----------------------------
//���������ؽ���������
void RP_AryArea_RefreshAll(struct _RP_AryArea *pArea)
{
  pArea->PlotMask = 0xff; 
  _Refresh(pArea, 0); //����ҳ��
  _RefreshFocus(pArea); //���½���
}


