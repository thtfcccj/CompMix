/********************************************************************************

                      ��·����ģ��ʵ��

*******************************************************************************/

#include "ShortMng.h"

/*******************************************************************************
                        ��غ���ʵ��
*******************************************************************************/

//----------------------------������------------------------------
//����1S����������ɨ��
void ShortMng_Task(struct _ShortMng *pPow, unsigned char Ch) //��Ӧͨ��
{
  unsigned char State = pPow->Flag & SHORT_MNG_STATE_MASK;
  //�պ�״̬ÿ�����ڶ����
  if(State == SHORT_MNG_STATE_SHUT){
    if(ShortMng_cbIsShort(Ch)){//ʶ�𵽶�·��
      unsigned char ShortCount;
      if(pPow->Timer){//������·�����
        ShortCount = (pPow->Flag >> SHORT_MNG_PROTECT_COUNT_SHIFT);
        if(ShortCount < (SHORT_MNG_PROTECT_COUNT_MASK >> SHORT_MNG_PROTECT_COUNT_SHIFT)) 
          ShortCount++;
      }
      else//�״ζ�·��
        ShortCount = 1;
      pPow->Flag = (ShortCount << SHORT_MNG_PROTECT_COUNT_SHIFT) | 
                    SHORT_MNG_STATE_BREAK;
      ShortMng_cbBreak(Ch);//�Ͽ�����
      if(ShortCount >= 7) pPow->Timer = 255;//���
      else pPow->Timer = SHORT_MNG_PTC_WAIT + (1 << ShortCount);
      //״̬�ı�ͨ��
      ShortMng_cbStateNotify(Ch, SHORT_MNG_STATE_SHUT, SHORT_MNG_STATE_BREAK);
    }
    else{
      if(pPow->Timer) pPow->Timer--;
      if(pPow->Timer == 0) pPow->Flag &= ~SHORT_MNG_PROTECT_COUNT_MASK;
    }
    return;
  }
  
  //����״̬ʱ�䵽����
  if(pPow->Timer) pPow->Timer--;
  if(pPow->Timer) return; //ʱ��δ��
  
  //ʱ�䵽��
  unsigned char NextState;
  switch(State){
    case SHORT_MNG_STATE_BREAK:  //��·��������,��������
    case SHORT_MNG_STATE_IDIE:  //��������
      ShortMng_cbShut(Ch);
      NextState = SHORT_MNG_STATE_WAIT;
      pPow->Timer = SHORT_MNG_WAIT_OV;
      break;
    case SHORT_MNG_STATE_WAIT:  //�ѽ��룬ת������
      NextState = SHORT_MNG_STATE_SHUT;
      pPow->Timer = SHORT_MNG_NOR_OV;
      break;
    default: 
      NextState = SHORT_MNG_STATE_IDIE; //�쳣
      break;
  }
  
  //״̬�ı��ˣ����²�ͨ��
  if(State != NextState){
    pPow->Flag &= ~SHORT_MNG_STATE_MASK;
    pPow->Flag |= NextState;
    ShortMng_cbStateNotify(Ch, State, NextState);
  }
}




