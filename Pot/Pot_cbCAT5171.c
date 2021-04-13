/***************************************************************************

                数字电位器(POT)回调函数-在使用CAT5171中的实现
实现为阻塞型，并支持同一I2C两个CAT5171芯片(AD0不一样)
****************************************************************************/

#include "Pot_cbCAT5171.h"
#include <string.h>

//直接单例化
static struct _I2cData _I2cData;
static unsigned char _Cmd[1];//1个字指令节: 7b:置位时表示中间，6b:1时断开模式，0正常
static unsigned char _Data[1]; //跟1byte数据

/***************************************************************************
                          回调函数实现
***************************************************************************/

//-------------------------硬件初始化调用-----------------------------
void Pot_cbPos2HwInit(const struct _Pot *pPot)
{
  _I2cData.CmdSize = 1; //指令1Byte
  _I2cData.DataSize = 1; //数据1Byte
  _I2cData.Flag = I2C_CMD_WR | 10;//预置为默认标志
  _I2cData.pCmd = _Cmd;
  _I2cData.pData = _Data;  
  _Cmd[0] = 0;//预置为正常写
}

//-------------------------开始电位器位置到硬件POT-----------------------------
//即将当前位置打到数字电位器上，根据不同通讯由硬件实现
//返回0:正常结束，负：未通讯上结束，正：通讯过程中(通讯完成需调Pot_Pos2HwEnd()告知)
signed char Pot_cbPos2HwStart(const struct _Pot *pPot)
{
  if(pPot->Id) _I2cData.SlvAdr = 45;//0b0101100 | 0b1;
  else _I2cData.SlvAdr = 44;//0b0101100;
  _Data[0] = pPot->CurPos; //给出位置
  //写硬件
  struct _I2cDev *pI2cDev = Pot_cbpGetI2c(pPot);
  I2cDev_ReStart(pI2cDev, &_I2cData);
  while(!I2cDev_IsEnd(pI2cDev));//阻塞等待
  if(I2cDev_eGetSatate(pI2cDev)== eI2cDone) return 0;//正常通讯
  return -1;//异常结束 
}


