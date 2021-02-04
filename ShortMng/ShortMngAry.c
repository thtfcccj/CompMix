/********************************************************************************

                      ��·����ģ������ʵ��

*******************************************************************************/
#include "ShortMngAry.h"
#include "IoCtrl.h"

struct _ShortMngAry ShortMngAry;
static unsigned char _CurCh;//��ǰѲ��ͨ��
/*******************************************************************************
                               ��غ���
*******************************************************************************/

//--------------------------------��ʼ��-----------------------------
//�β�Ϊ�ܵ��ϵ���ʱʱ�䣬���ڷ�ʱ�ϵ�
void ShortMngAry_Init(unsigned char ShutDelay)
{
  CfgPo();//��Դ����IO��ʼ��
  _CurCh = 0;
  for(unsigned char Ch = 0; Ch < SHORT_MNG_ARY_COUNT; Ch++){
    ShortMng_Init(&ShortMngAry.ShortMng[Ch], ShutDelay); 
    ShutDelay++;
  }
}

//-------------------------------��Ϊ����ģʽ------------------------------
void ShortMngAry_ShutDown()//��Ӧͨ��  
{
  for(unsigned char Ch = 0; Ch < SHORT_MNG_ARY_COUNT; Ch++){
    ShortMng_ShutDown(&ShortMngAry.ShortMng[Ch], Ch);
  }
}

//------------------------------������------------------------------
//����128msS����������ɨ��
void ShortMngAry_Task(void) //��Ӧͨ��
{
  ShortMng_Task(ShortMngAry_GetMng(_CurCh), _CurCh);
  _CurCh++;
  if(_CurCh >= SHORT_MNG_ARY_COUNT) _CurCh = 0;
}

//------------------------------��Ӧͨ���Ƿ����------------------------------
signed char ShortMngAry_IsShut(unsigned char Ch)
{
  unsigned char State =  ShortMng_GetState(ShortMngAry_GetMng(Ch));
  if(ShortMng_IsShut(State)) return 1;
  return 0;
}



