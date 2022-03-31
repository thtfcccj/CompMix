/************************************************************************

                       ��̬Idɸѡ��ʵ��

************************************************************************/

#include "DynaIdFilter.h"  //ɸѡ��
#include "math_3.h"
#include <string.h>

/************************************************************************
                          ��غ���ʵ��
************************************************************************/

//----------------------------�����ұ�---------------------------------
#ifdef SUPPORT_DYNA_ID_FILTER_LSB
  #define _FullLUT(d,p)  pLsbS2Full(d,p)
#else
  #define _FullLUT(d,p)  pMsbS2Full(d,p)
#endif

//----------------------------��̬Id��䴦��-------------------------
//PosOrId: b14~15!=0: Ҫ�ȸ�������IDʱ,����λ�ã��ҵ��󷵻ظ�λ��(pLUT��λ)�Ͷ�Ӧҳ���ұ� 
//MSB˫�ֽڷ�ʽ�����ұ�,���ұ�ΪNULLʱ����-1��ʾδ�ҵ���NULL�ҵ�
unsigned char* DynaIdFilter_Full(struct _DynaIdFilter *pFilter,//���ṹ
                                 unsigned short TblLen,     //���ұ���
                                 const _FilterFun funFilter, //ɸѡ����,��ֹΪNULL
                                 unsigned long Para,        //�β��п��ܴ���Ĳ���
                                 unsigned short PosOrId,    //��ȡλ�û�����ID
                                 unsigned char RdLen,       //��ȡ����
                                 unsigned char *pLUT)      //Ҫ���Ĳ��ұ�NULL����Ҫ
{
  unsigned short EndRdPos;
  //�����ҵ��û���ʼλ��
  unsigned short CurUserPos = 0;
  unsigned short CurLocalPos = 0;  
  unsigned short RdPos;//��ȡλ��
  
  //=======����IDʱ�����Ȳ��ҵ���ӦIDλ�ò��ض�Ӧ��ȡ��ʼλ��==============
  if(PosOrId & 0xC000){
    PosOrId &= 0x3fff;//ΪID��
    RdPos = 0;
    for(; CurLocalPos < TblLen; CurLocalPos++){
      if(!funFilter(CurLocalPos, Para)) continue; //����������
      RdPos++; //����ɸѡ������
      //�������IDʱ,�Ƿ��������
      if(funFilter(0x8000 | CurLocalPos, Para) == PosOrId) break;
    }
    if(CurLocalPos >= TblLen) goto _ErrReturn; //û�ҵ�,���󷵻�
    //�ҵ���ת��Ϊ��ȡλ����,������ҳ������ָ��0������
    if(RdPos < RdLen){//��ҳ��Ϣ��ʼ��
      //CurUserPos = 0; û�ù�
      CurLocalPos = 0;
      RdPos = 0;
    }
    else CurUserPos = RdPos;//������ҳ��ֱ�Ӷ�Ӧ(CurLocalPosҲ��λ��)
    pLUT = _FullLUT(RdPos, pLUT); //��λ�����ұ�ʵ������ֵ
    goto _ToRdPos; //ֱ�Ӷ�ȡ
  }
  else RdPos = PosOrId;//Ϊ��ȡλ��
  if(RdPos >= TblLen) goto _ErrReturn;  //��ȡλ�������λ����

  //============================����������==============================
  //�״ζ�ȡ
  if(RdPos == 0) goto _ToRdPos; 
  
  //ǰ����������Ч
  if((RdPos < RdLen) || !DynaIdFilter_IsValid(pFilter))
    goto _ToRdPos;
  //��λʧ��
  if(pFilter->LocalPos >= TblLen) goto _ToRdPos;
  //�Ȼ���
  CurUserPos = pFilter->UserPos;
  CurLocalPos = pFilter->LocalPos;   
  //����ֱ��������
  if(RdPos == CurUserPos) goto _MsbSFull;  
  //else ����δ����

  //=======================�鵽���ƶ�����ȡλ��=============================
_ToRdPos: //����ȡλ��
    //�������
  if(RdPos >= CurUserPos){
    for(; CurLocalPos < TblLen; CurLocalPos++){
      if(!funFilter(CurLocalPos, Para)) continue; //����������
      //�ҵ���
      if(CurUserPos == RdPos)goto _MsbSFull;//��ָ��λ����
      CurUserPos++;  //�û�λ����һ��
    }
  }
  else{//�������
    for(; CurLocalPos > 0; CurLocalPos--){
      if(!funFilter(CurLocalPos, Para)) continue; //����������
      //�ҵ���
      if(CurUserPos == RdPos) goto _MsbSFull;//��ָ��λ����
      CurUserPos--;  //�û�λ����һ��
    }
  }
_ErrReturn: //���󷵻�
  //����Ҳû�ҵ� 
  if(pLUT == NULL) return (unsigned char*)-1;//����Ҫ���ʱ
  return pLUT; //û�ҵ������
  
  //=======================�ҵ���ʼλ�ú��������=============================
_MsbSFull: //�ҵ���ʼλ���ˣ����
  //�ȸ���ɸѡ��
  pFilter->UserPos = RdPos;
  pFilter->LocalPos = CurLocalPos; 
  
  if(pLUT == NULL) return NULL; //����Ҫ���ʱ
  
  //�õ��������û���ȡ����λ��
  EndRdPos = RdPos + RdLen;
  if(EndRdPos > TblLen) EndRdPos = TblLen;
  
  for(; CurLocalPos < TblLen; CurLocalPos++){
    if(!funFilter(CurLocalPos, Para)) continue; //����������
    pLUT = _FullLUT(funFilter(0x8000 | CurLocalPos, Para), pLUT); //�����ұ�ʵ������ֵ
    RdPos++;  //�û�λ����һ��    
    if(RdPos >= EndRdPos) break; //�������
  }
  return pLUT;
}


