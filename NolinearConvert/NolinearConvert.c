/*******************************************************************************

                            ������У��ģ��
��ģ��������Դ����(x��)����������Ӧ�ã�������ʱ��Ŀ������(y��)���ޣ�����ӦΪ��ֵ
*******************************************************************************/

#include "NolinearConvert.h"
#include <string.h>

#ifdef SUPPORT_NOLINEAR_CONVERT_BUF
  struct _NolinearConvertBuf NolinearConvertBuf;
#endif

//������ұ���������е�Qֵ,Խ��Խ��ȷ����Ӧע�ⳬ��
#define _NolinearConvert_Q   8

//---------------------------------------������ת������-------------------------
//pLut: X��Ϊ������������ұ�������ڣ�Y���ޣ�����Ϊ��
//X,Yֵ���ܲ�̫Զ��������Qֵ�������Ӱ�쾫��
//����Ϊ��ʱ��������0
NolinearConvert_t NolinearConvert(NolinearConvert_t Source, //����Դ
                                  const struct _NolinearConvertTbl *pLut,//NULLʱ=Source
                                  unsigned char LutSize)  //  //���ұ��С,0ʱ�Զ�
{
  if(pLut == NULL) return Source; //�ޱ�
  if(!LutSize) LutSize = NolinearConvert_GetSize(pLut);//�Զ����ҽ�����
  if(LutSize < 2) return Source; //һ����޵�ʱ����ʾ�ޱ���
  
  unsigned char Pos;   //����Դ���ڲ��ұ�����λ��
  NolinearConvert_long_t Muti;//����

  //�������Դ���ڲ��ұ�����:����ֱ�Ӳ��ҷ�ʽ����
  Pos = 0;
  for(; Pos < LutSize; Pos++){
    if(Source < (pLut + Pos)->Source) break;
  }

  //�õ����ҵ������������,������Դ����x��,����Ŀ�꿴��y��
  //����LutSize�����͵�λ��
  if(!Pos) LutSize = 0;  //����͵�������
  else if(Pos >= LutSize) LutSize = LutSize - 2;  //������ߵ���
  else LutSize = Pos - 1;
  pLut += LutSize;//�����͵�

  const struct _NolinearConvertTbl *pNextLut = pLut + 1;
  //����
  if(pNextLut->Destination >= pLut->Destination) //Y������
    Muti = pNextLut->Destination - pLut->Destination;
  else Muti =  pLut->Destination - pNextLut->Destination;
  Muti <<= _NolinearConvert_Q;
  Muti /= (pNextLut->Source - pLut->Source);
  //��Y����
  if(!Pos) Source = pLut->Source - Source;
  else Source -= pLut->Source;
  Source = (Source * Muti) >> _NolinearConvert_Q;//Source=Y����

  //��Y��λ�ò��ͳ�
  if(!Pos){//�������ʱ
    if(pNextLut->Destination >= pLut->Destination){ //Y������
      if(pLut->Destination > Source) //��ֹΪ��ֵ
        return pLut->Destination - Source;
      else return 0;
    }
    else return pLut->Destination + Source;//Y�ή��
  }
  
  if(pNextLut->Destination >= pLut->Destination) //Y������
    return pLut->Destination + Source;
  else{ //Y�ή��
    if(pLut->Destination > Source) //��ֹΪ��ֵ
      return pLut->Destination - Source;
    else return 0;
  }
}

//------------------------------�õ����ұ��С----------------------------------
//���ұ����н�����ʱ,���ô˺����жϴ�С
unsigned char NolinearConvert_GetSize(const struct _NolinearConvertTbl *pLut)
{
  unsigned char LutSize = 0;
  for(; LutSize < NOLINEAR_CONVERT_LUT_MAX; LutSize++){
      if((pLut + LutSize)->Source == NOLINEAR_CONVERT_NULL)//������
        break;
  }
  return LutSize;
}

//--------------------------���ұ���Copy����----------------------------------
//����pOrgLut����x,y���copy���������
void NolinearConvert_AntiCopy(struct _NolinearConvertTbl *pDeLut,//Ŀ��,��>=Դ�ռ�
                              const struct _NolinearConvertTbl *pOrgLut,//Դ
                              unsigned char LutSize)  //Դ���ұ��С,0ʱ�Զ�  
{
  if(!LutSize) LutSize = NolinearConvert_GetSize(pOrgLut);//�Զ����ҽ�����
  for(unsigned char i = 0; i < LutSize; i++, pDeLut++, pOrgLut++){
    pDeLut->Source = pOrgLut->Destination;
    pDeLut->Destination = pOrgLut->Source;
  }
  //ǿ�Ʋ��������
  if(LutSize < NOLINEAR_CONVERT_LUT_MAX){
    pDeLut->Source = NOLINEAR_CONVERT_NULL; 
  }
}





