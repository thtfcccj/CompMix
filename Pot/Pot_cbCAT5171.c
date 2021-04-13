/***************************************************************************

                ���ֵ�λ��(POT)�ص�����-��ʹ��CAT5171�е�ʵ��
ʵ��Ϊ�����ͣ���֧��ͬһI2C����CAT5171оƬ(AD0��һ��)
****************************************************************************/

#include "Pot_cbCAT5171.h"
#include <string.h>

//ֱ�ӵ�����
static struct _I2cData _I2cData;
static unsigned char _Cmd[1];//1����ָ���: 7b:��λʱ��ʾ�м䣬6b:1ʱ�Ͽ�ģʽ��0����
static unsigned char _Data[1]; //��1byte����

/***************************************************************************
                          �ص�����ʵ��
***************************************************************************/

//-------------------------Ӳ����ʼ������-----------------------------
void Pot_cbPos2HwInit(const struct _Pot *pPot)
{
  _I2cData.CmdSize = 1; //ָ��1Byte
  _I2cData.DataSize = 1; //����1Byte
  _I2cData.Flag = I2C_CMD_WR | 10;//Ԥ��ΪĬ�ϱ�־
  _I2cData.pCmd = _Cmd;
  _I2cData.pData = _Data;  
  _Cmd[0] = 0;//Ԥ��Ϊ����д
}

//-------------------------��ʼ��λ��λ�õ�Ӳ��POT-----------------------------
//������ǰλ�ô����ֵ�λ���ϣ����ݲ�ͬͨѶ��Ӳ��ʵ��
//����0:��������������δͨѶ�Ͻ���������ͨѶ������(ͨѶ������Pot_Pos2HwEnd()��֪)
signed char Pot_cbPos2HwStart(const struct _Pot *pPot)
{
  if(pPot->Id) _I2cData.SlvAdr = 45;//0b0101100 | 0b1;
  else _I2cData.SlvAdr = 44;//0b0101100;
  _Data[0] = pPot->CurPos; //����λ��
  //дӲ��
  struct _I2cDev *pI2cDev = Pot_cbpGetI2c(pPot);
  I2cDev_ReStart(pI2cDev, &_I2cData);
  while(!I2cDev_IsEnd(pI2cDev));//�����ȴ�
  if(I2cDev_eGetSatate(pI2cDev)== eI2cDone) return 0;//����ͨѶ
  return -1;//�쳣���� 
}


