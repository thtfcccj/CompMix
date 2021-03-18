/*******************************************************************************

                         �¶��豸-������ʵ��
������ʱ��ʹ�ô���ģ��ʵ����
*******************************************************************************/
#include "TempAry.h"

struct _TempDev TempDevAry[TEMP_ARY_COUNT];


/*******************************************************************************
                           �����Ϊ����
*******************************************************************************/

//------------------------------��ʼ������-------------------------------------
void TempAry_Init(unsigned char IsInited) //�Ƿ��ѳ�ʼ��
{
  for(unsigned char Ch = 0; Ch < TEMP_ARY_COUNT; Ch++){
    TempDev_Init(&TempDevAry[Ch], IsInited, Ch);
  }
}


//---------------------------------�����¶�ֵ-----------------------------------
//���ڵ����Ը����¶�,�����ڷ��ж������ڵ���
void TempAry_Update(void)
{
  for(unsigned char Ch = 0; Ch < TEMP_ARY_COUNT; Ch++){
    TempDev_Update(&TempDevAry[Ch], TempAry_cbGetOrgSignal(Ch));
  }
}

//---------------------------�Ժ��������-------------------------------
//����TempAry_Update()�������Ľ����е���(����32ms����)
#ifdef SUPPORT_TEMP_DEV_FILETER //֧���˲�ʱ
void TempAry_UpdateLater(void)
{
  for(unsigned char Ch = 0; Ch < TEMP_ARY_COUNT; Ch++){
    TempDev_UpdateLater(&TempDevAry[Ch]);
  }
}
#endif




