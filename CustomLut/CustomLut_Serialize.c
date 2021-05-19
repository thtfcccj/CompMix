/******************************************************************************

               CustomLut���л�(Serialize)�뷴���л�(Deserialize)ʵ��

*******************************************************************************/

#include "CustomLut_Serialize.h"

//-------------------------------���л�����--------------------------------
//������ pCustomLut->Info.Tbl[]���л������������ؽ���λ��
unsigned char *CustomLut_pSerialize(const struct _CustomLut *pCustomLut,
                                     unsigned char *pData)//���н��ջ�����
{
  const struct _NolinearConvertTbl *pTbl = pCustomLut->Info.Tbl;
  for(unsigned char Count = CUSTOM_LUT_LUT_SIZE; Count > 0; Count--){
    *pData++ = pTbl->Source >> 8;
    *pData++ = pTbl->Source & 0xff;
    *pData++ = pTbl->Destination >> 8;
    *pData++ = pTbl->Destination & 0xff;
    pTbl++;
  }
  return pData;
}

//-----------------------------�����л�����--------------------------------
//���������ݻָ������أ����ػ���������λ��
const unsigned char *pCustomLut_Deserialize(struct _CustomLut *pCustomLut,
                                            const unsigned char *pData)
{
  struct _NolinearConvertTbl *pTbl = pCustomLut->Info.Tbl;
  for(unsigned char Count = CUSTOM_LUT_LUT_SIZE; Count > 0; Count--){
    pTbl->Source = ((unsigned short)*pData << 8) | *(pData + 1); 
    pData += 2;
    pTbl->Destination = ((unsigned short)*pData << 8) | *(pData + 1); 
    pData += 2;
    pTbl++;
  }
  return pData;
}

