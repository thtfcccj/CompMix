/**********************************************************************

               InfoBase�еĴ�������̬�������ʵ��

**********************************************************************/

#include "InfoBaseDyncMng.h"
#include "InfoBase.h"

EepromLen_t InfoBaseDyncMngBase = 0;

/***************************************************************************
                                 ��ͨ����ʵ��
***************************************************************************/
//-----------------------------�������¿ɷ��������С--------------------------
EepromLen_t InfoBaseDyncMng_GetFreeSize(void)
{
  return INFO_BASE_DYNC_COUNT - InfoBaseDyncMngBase;
}

//-----------------------------��̬�ڴ���亯��--------------------------
//���ػ�ַλ�ã�Ϊ0��ʾ�ڴ治��
//����ʧ��,��ͨ��InfoBaseDyncMng_GetFreeSize()��ѯ���ø����Լ���֧��
EepromLen_t InfoBaseDyncMng_Malloc(unsigned char ClassSize, //���ʹ�С
                                   unsigned short Count)    //���͸���
{
  EepromLen_t NextBase = InfoBaseDyncMngBase + (ClassSize * Count);
  if(NextBase > INFO_BASE_DYNC_COUNT){//����������
    InfoBaseDyncMng_cbOvNotify();//ͨ��
    return 0; 
  }
  EepromLen_t CurBase = InfoBaseDyncMngBase;
  InfoBaseDyncMngBase = NextBase;
  
  return CurBase + INFO_BASE_DYNC_BASE; //���ϻ�ַ
}


