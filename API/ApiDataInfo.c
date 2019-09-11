/***********************************************************************

        ����/����ģʽ����/�ṹͳһ���ʽӿ�-������Ϣ�������ʵ��

***********************************************************************/

#include "ApiDataInfo.h"

//����ռλ(0����)
static const unsigned char _TypeLenLut[4] = {1,2,4,0};

//��׼�̶�����ռλ:
static const unsigned char _StdTypeLenLut[] = {
  0, //0�ַ�����,�����ɽ����ַ�����  
  4, //1����������,����4Byte
  8, //2˫��������,����8Byte
  2, //3��������Byte��
  //δ����
 

  4,
  8,2,0

};


/***********************************************************************
                             ��غ���
***********************************************************************/

//-------------------------����Ϣ�������ռλ------------------------------
//0��ʾΪ�ַ���������ַ��ж�,�쳣��δ����ʱ����1
unsigned char ApiDataInfo_GetLen(unsigned char ApiDataInfo)
{
  //�ַ���ʱ�����ж�
  if(ApiDataInfo == API_DI_STD_STR) return 0; 
  
  unsigned char SubDataSize = ApiDataInfo >> API_DI_TYPE_SHIRT;
  
  //�ṹ��ʱ���ṹ�峤��
  if((ApiDataInfo & API_DI_BX_MASK) == API_DI_STRUCT){
    return (SubDataSize << 4) * (ApiDataInfo & API_DI_STRUCT_LEN_MASK);
  }
  //Byte���ж�
  unsigned char Len = _TypeLenLut[SubDataSize];
  if(Len) return Len;
  
  //�Զ��������ж�
  if((ApiDataInfo & API_DI_OTHER_MASK) == API_DI_OTHER_STD){ //��׼��������
    if(ApiDataInfo >= API_DI_STD_END) return 1;//�쳣��δ����ʱ����1
    return _StdTypeLenLut[ApiDataInfo & API_DI_STD_MASK];
  }
    
  //�û������ж�
  if((ApiDataInfo & API_DI_OTHER_USER_MASK) == API_DI_USER_OTHER)
    return 4;//�û���������,����4
  else return 2;//DAP2324��ʽ��С��������ʾ����
}

