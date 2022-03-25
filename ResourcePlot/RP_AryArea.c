/***********************************************************************

                 ��Դ�ļ��͵�����ģʽ3x3�˵�ʵ��
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
                      unsigned char PlotMask)
{
  pArea->FDesc.Rect.x = x;
  pArea->FDesc.Rect.y = y;
  pArea->FDesc.IconBase = pArea->STypeInfo.IconBase;
  pArea->FDesc.IconCount = pArea->STypeInfo.IconCount;
  pArea->FDesc.ParaBase = pArea->STypeInfo.ParaBase;
  pArea->FDesc.ParaCount = pArea->STypeInfo.ParaCount;
  //���ù̶������ͼ
  RP_FixArea(Handle, PlotMask, &pArea->FDesc);
}

//-------------------------------ˢ�½���------------------------------
static void _Refresh(struct _RP_AryArea *pArea,
                      signed char IsRefreshFocus)//ˢ�½��㣬����ΪPlotMask
{
  unsigned char PlotMask;
  if(IsRefreshFocus) PlotMask = 0x0E; //ˢ�½���λ��
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
  unsigned short pw = pMainDesc->Rect.w / RowCount; //��ռ���
  if(!(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN)) pw -= 2; //�������߿�߶�
  if(pw <= pAryDesc->Rect.w) return;//�����Ŀ��̫խ���쳣
  x += pAryDesc->Rect.x + pw / 2; //x��ʼ
  //�õ���߶�����y��ʼ
  unsigned short ph = pMainDesc->Rect.h / pAryDesc->ColCount; //��ռ�߶�
  if(!(pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN)) ph -= 2; //�������߿�߶�
  if(ph <= pAryDesc->Rect.h) return;//�����ĸ߶�̫խ���쳣
  y += pAryDesc->Rect.y + ph / 2; //y��ʼ

  //��ʼ��FDesc���ι̶�����
  pArea->FDesc.Rect.w = pMainDesc->Rect.w;
  pArea->FDesc.Rect.h = pMainDesc->Rect.w;
  pArea->FDesc.cBg = pAryDesc->cBg;
  pArea->FDesc.IconBgId = pAryDesc->IconBgId;
  //��ʼ��RP_FixArea��Ҫ��PlotMask��Handle;
  unsigned long Handle = pArea->Handle;
  if(PlotMask == 0){//��ˢ��ʵʱ����ʱ
    Handle |= RP_HANDLE_REAL_REFRESH;
    PlotMask = 0x0C; //����������������ͼ
  }
  unsigned short AryId = pArea->AryStart;
  unsigned short FocusAryId; //��ѡAryid
  if(pAryDesc->BitInfo & RP_ITEM_DIS_FOCUS) FocusAryId = 0xffff;//�޽���
  else FocusAryId = AryId + pArea->Focus;
  unsigned char Pos;
  //========================�������½���λ��======================
  if(IsRefreshFocus){
    if(pAryDesc->BitInfo & RP_ITEM_DIS_FOCUS) return;//�޽���

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
    //���±�ҳ������������
    RP_AryArea_cbGetSTypeInfo(Handle, FocusAryId, &pArea->STypeInfo);
    _PotItem(pArea, Handle | FocusAryId | pArea->STypeInfo.SType,
             x + pw * (Pos % RowCount),
             y + ph * (Pos / RowCount),
             0x0F | 0x10); //0x10: �ڶ�����Ϊѡ��
    if(Pos == pArea->PrvFocus) return; //��������һ��
    //���±�ҳ�ϴεĽ�����������
    Pos = pArea->PrvFocus;
    pArea->PrvFocus = pArea->Focus; //������    
    AryId += Pos;   //�ϴν���
    RP_AryArea_cbGetSTypeInfo(Handle, AryId, &pArea->STypeInfo);
    _PotItem(pArea, Handle | AryId | pArea->STypeInfo.SType,
             x + pw * (Pos % RowCount),
             y + ph * (Pos / RowCount),
             0x0F);  //δѡ��
    return;
  }
  //========================ɨ���������=========================
  unsigned short Count = RowCount * pAryDesc->ColCount;//ҳ����������
  if(Count > 255) return ;//�쳣
  Pos = 0; //��ͷ��ʼ
  for(; Pos < Count; Pos++, AryId++){
    signed char Resumme = RP_AryArea_cbGetSTypeInfo(Handle, AryId,
                                                    &pArea->STypeInfo);
    if(Resumme == 0){//������
      pArea->AryCount = AryId;//��̬����
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
  if(pArea->pAryDesc->BitInfo & RP_ITEM_ICON_BG_FOCUS_EN) return;
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
  pArea->PlotMask = 0xff; //ˢ������
  _Refresh(pArea, 0); //����ҳ��
  _RefreshFocus(pArea); //���½���

}

//--------------------------�Ϸ�ҳ����----------------------------
//�����Ƿ���Ӧ
static signed char _PageUp(struct _RP_AryArea *pArea,
                           unsigned short Count)
{
  if(pArea->AryCount < Count) return 0;//ֻ��һҳ������
  
  if(pArea->AryStart >= Count) pArea->AryStart -= Count;
  else//�ػ������һҳ
    pArea->AryStart = (pArea->AryCount / Count) * Count;
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
//GuideKey����Ϊ: ȷ��0��1��2��3��4, 5��һҳ��6��һҳ
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
      if((Focus >= RowCount) > 0) Focus -= RowCount; //����1��
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
    default: return; //��������֧�� 
  }
  pArea->Focus = Focus;
  _ResetTimer(pArea);
  //ҳ�仯
  if(PrvAryStart != pArea->AryStart){
    pArea->PlotMask = 0xff; //ˢ������
    _Refresh(pArea, 0); //����ҳ��
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


