/*******************************************************************************

     温度模块硬件相关定义-使用外部10KNTC, 3380K 曲线补偿表实现

*******************************************************************************/
#include "Temp.h"
#include "NolinearConvert.h"

/*******************************************************************************
                            相关回调函数实现 
*******************************************************************************/
#ifdef SUPPORT_TEMP_LUT

//------------------------------------校准曲线--------------------------------
//根据样品试验结果取特征点,最多也取8个点
static const struct _NolinearConvertTbl _OrgTemp2Temp[] = {
  /* 0 */ (128 - 33)<<7, (64 - 40)<<8,//16.5
  /* 1 */ (128 - 31)<<7, (64 - 30)<<8,//15.5
  /* 2 */ (64 - 14)<<8,  (64 - 25)<<8,
  /* 3 */ (64 - 10)<<8,  (64 - 15)<<8, 
  /* 4 */ (64 - 0 )<<8,  (64 -  0)<<8,  //校准点1
  /* 5 */ (64 + 40)<<8,  (64 + 40)<<8,  //校准点2, 两点间误差1.2度 
  /* 6 */ (64 + 54)<<8,  (64 + 55)<<8,  //基本不差
  /* 7 */ (64 + 65)<<8,  (64 + 70)<<8,  //差5度
};

//------------------------------------温度校正--------------------------------
//如：线性度校正，其它附加处理等,输入为Q8温度，输出为结果温度
unsigned char Temp_cbAppendPro(unsigned short OrgTempQ8)
{
  OrgTempQ8 = NolinearConvert(OrgTempQ8, //数据源
                             _OrgTemp2Temp,  //已上升排序查找表阵列入口
                             8);
  if(OrgTempQ8 & 0x80) OrgTempQ8 += 0x80; //四舍五入
  return (unsigned char)(OrgTempQ8 >> 8);
}

//----------------------------------温度反校正---------------------------------
//与Temp_cbAppendPro()成对使用,输入为结果温度，输出为原始温度(非Q8)
unsigned char Temp_cbAntiAppendPro(unsigned char CurTemp)
{
  NolinearConvert_AntiCopy(NolinearConvertBuf.Tbl, //使用缓冲区反向
                           _OrgTemp2Temp, 
                           8);
  unsigned short Q8 = (unsigned short)CurTemp << 8;
  Q8 = NolinearConvert(Q8, //数据源
                       NolinearConvertBuf.Tbl,  //已上升排序查找表阵列入口
                       8);
  if(Q8 & 0x80) Q8 += 0x80; //四舍五入
  return (unsigned char)(Q8 >> 8);
}

#endif //SUPPORT_TEMP_LUT

