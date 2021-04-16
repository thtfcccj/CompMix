/*******************************************************************************

                            非线性校正模块
此模块适用于源数据(x轴)单调上升的应用，不反表时，目标数据(y轴)不限，但均应为正值
*******************************************************************************/

#include "NolinearConvert.h"
#include <string.h>

#ifdef SUPPORT_NOLINEAR_CONVERT_BUF
  struct _NolinearConvertBuf NolinearConvertBuf;
#endif

//定义查找表运算过程中的Q值,越大越精确，但应注意超限
#define _NolinearConvert_Q   8

//---------------------------------------非线性转换函数-------------------------
//pLut: X轴为已上升排序查找表阵列入口，Y不限，均不为负
//X,Y值不能差太远，否则因Q值问题可能影响精度
//计算为负时，将返回0
NolinearConvert_t NolinearConvert(NolinearConvert_t Source, //数据源
                                  const struct _NolinearConvertTbl *pLut,//NULL时=Source
                                  unsigned char LutSize)  //  //查找表大小,0时自动
{
  if(pLut == NULL) return Source; //无表
  if(!LutSize) LutSize = NolinearConvert_GetSize(pLut);//自动查找结束点
  if(LutSize < 2) return Source; //一点或无点时，表示无表项
  
  unsigned char Pos;   //数据源所在查找表区间位置
  NolinearConvert_long_t Muti;//倍率

  //检查数据源所在查找表区间:暂以直接查找方式进行
  Pos = 0;
  for(; Pos < LutSize; Pos++){
    if(Source < (pLut + Pos)->Source) break;
  }

  //得到离找到点最近的两点,将数据源看作x轴,数据目标看作y轴
  //复用LutSize用作低点位置
  if(!Pos) LutSize = 0;  //在最低电以下了
  else if(Pos >= LutSize) LutSize = LutSize - 2;  //超过最高点了
  else LutSize = Pos - 1;
  pLut += LutSize;//移至低点

  const struct _NolinearConvertTbl *pNextLut = pLut + 1;
  //求倍率
  if(pNextLut->Destination >= pLut->Destination) //Y轴升序
    Muti = pNextLut->Destination - pLut->Destination;
  else Muti =  pLut->Destination - pNextLut->Destination;
  Muti <<= _NolinearConvert_Q;
  Muti /= (pNextLut->Source - pLut->Source);
  //求Y增量
  if(!Pos) Source = pLut->Source - Source;
  else Source -= pLut->Source;
  Source = (Source * Muti) >> _NolinearConvert_Q;//Source=Y增量

  //求Y点位置并送出
  if(!Pos){//零点以下时
    if(pNextLut->Destination >= pLut->Destination){ //Y轴升序
      if(pLut->Destination > Source) //防止为负值
        return pLut->Destination - Source;
      else return 0;
    }
    else return pLut->Destination + Source;//Y轴降序
  }
  
  if(pNextLut->Destination >= pLut->Destination) //Y轴升序
    return pLut->Destination + Source;
  else{ //Y轴降序
    if(pLut->Destination > Source) //防止为负值
      return pLut->Destination - Source;
    else return 0;
  }
}

//------------------------------得到查找表大小----------------------------------
//查找表定义有结束点时,可用此函数判断大小
unsigned char NolinearConvert_GetSize(const struct _NolinearConvertTbl *pLut)
{
  unsigned char LutSize = 0;
  for(; LutSize < NOLINEAR_CONVERT_LUT_MAX; LutSize++){
      if((pLut + LutSize)->Source == NOLINEAR_CONVERT_NULL)//结束了
        break;
  }
  return LutSize;
}

//--------------------------查找表反向Copy函数----------------------------------
//即将pOrgLut交换x,y轴后copy到结果表中
void NolinearConvert_AntiCopy(struct _NolinearConvertTbl *pDeLut,//目标,需>=源空间
                              const struct _NolinearConvertTbl *pOrgLut,//源
                              unsigned char LutSize)  //源查找表大小,0时自动  
{
  if(!LutSize) LutSize = NolinearConvert_GetSize(pOrgLut);//自动查找结束点
  for(unsigned char i = 0; i < LutSize; i++, pDeLut++, pOrgLut++){
    pDeLut->Source = pOrgLut->Destination;
    pDeLut->Destination = pOrgLut->Source;
  }
  //强制插入结束点
  if(LutSize < NOLINEAR_CONVERT_LUT_MAX){
    pDeLut->Source = NOLINEAR_CONVERT_NULL; 
  }
}





