/*******************************************************************************

					            APP��������ģ��ʵ��

********************************************************************************/
#include  "gCfg.h"
#include  "Eeprom.h"
#include  "InfoBase.h"
#include  <String.h>


struct _gCfg gCfg;

static const struct _gCfg _Default = {
  GCFG_BIT_DEFAULT,
  GCFG_BYTE_DEFAULT,
};

/****************************************************************************
                             ��غ���ʵ��
****************************************************************************/
//--------------------------------��ʼ������------------------------------
void gCfg_Init(signed char IsInited)
{
  memset(&gCfg, 0 , sizeof(struct _gCfg));
  
  //�ڲ�������ʼ��  
  if(!IsInited){//װ��Ĭ��
    memcpy(&gCfg, &_Default, sizeof(struct _gCfg));
    Eeprom_Wr(gCfg_GetInfoBase(),
              &gCfg,
              sizeof(struct _gCfg));
  }
  else{
    Eeprom_Rd(gCfg_GetInfoBase(),
                &gCfg,
                sizeof(struct _gCfg));
  }
}

//-----------------------------���ò�����λ����--------------------------
void gCfg_CfgBit(unsigned char Pos,    //λ���±�
                     unsigned char BitCfg,    //λ����
                     unsigned char ValidMask) //Ӱ��λ
{
  gCfg.Bit[Pos] &= ~ValidMask;
  gCfg.Bit[Pos] |= (BitCfg & ValidMask);
  //����
  Eeprom_Wr(gCfg_GetInfoBase(),
            &gCfg,
            sizeof(struct _gCfg));
}

//-------------------------------����ͨ���ֽ����ݲ�����----------------------
void gCfg_CfgData(unsigned char Pos,
                       unsigned char Byte)
{
  gCfg.Byte[Pos] = Byte;
  //����
  Eeprom_Wr(gCfg_GetInfoBase(),
            &gCfg,
            sizeof(struct _gCfg));  
}


