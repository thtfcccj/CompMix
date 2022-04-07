/***********************************************************************

                  ��������Դͨ�ò�������ʵ��

***********************************************************************/
#include "ResourceIndex.h"

//FLASH_CAPABILITY:  Ϊ��ǰFLASH������ȫ������

//---------------------------�쳣ʱ���ؿ���Դ����------------------------
const char StrSpace[] = " ";  //�ո��ַ�" "

/***********************************************************************
                          ����ʵ��
***********************************************************************/

//----------------------����ԴID�õ��������λ��-------------------------
unsigned short RI_GetOffPos(const unsigned char *pBase,//��Դ��ַ
                              unsigned short ResId)
{
  #if RI_HEADER_LEN == 1
    return *(pBase + ResId);
  #else // RI_HEADER_LEN == 2
    pBase += 2 * ResId; //shortΪ��λ
    #if SUPPORT_RI_MSB //��λ��ǰ
      return ((unsigned short)*pBase << 8) + *(pBase + 1);
    #else //��λ��ǰ
      return (((unsigned short)*(pBase + 1)) << 8) + *pBase;
    #endif
  #endif
}

//------------------------------����ԴID�õ�����--------------------------
//δ�ҵ�ʱ����RI_NULL
const unsigned char *RI_pGetData(const unsigned char *pBase,//��Դ��ַ
                                  unsigned char ResId)
{
  unsigned short OffPos = RI_GetOffPos(pBase, ResId);
  if(OffPos >= (FLASH_CAPABILITY - (unsigned long)pBase)) //�쳣
    return RI_NULL;
  return pBase + OffPos;  
}

//----------------------------����ԴID�õ����ݴ�С-------------------------
//δ�ҵ�ʱ����0
unsigned short RI_GetSize(const unsigned char *pBase,//��Դ��ַ
                           unsigned short ResId)
{
  unsigned short CurOffPos = RI_GetOffPos(pBase, ResId);
  unsigned short NextOffPos = RI_GetOffPos(pBase, ResId + 1);
  if(CurOffPos > NextOffPos) return 0;//�쳣
  unsigned short Len = NextOffPos - CurOffPos; 
  if(Len >= (FLASH_CAPABILITY - (unsigned long)pBase)) return 0; //�쳣
  return Len;
}

//------------------------------�õ���Դ��ID����---------------------------
unsigned short RID_GetIdCount(const unsigned char *pBase)
{
  #if RI_HEADER_LEN == 1
    return  RI_GetOffPos(pBase, 0);
  #else // RI_HEADER_LEN == 2
    return  RI_GetOffPos(pBase, 0) / 2;
  #endif
}




