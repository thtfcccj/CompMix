/*******************************************************************************

                            非线性校正模块
此模块适用于源数据(x轴)单调上升的应用，目标数据(y轴)不限，但均应为正值
*******************************************************************************/
#ifndef __NOLINEAR_CONVERT_H
#define __NOLINEAR_CONVERT_H

/****************************************************************************
                             相关配置
****************************************************************************/

#if NOLINEAR_CONVERT_LEVEL == 1 //表为unsigned char型
  #define NolinearConvert_t unsigned char   //类型
  #define NOLINEAR_CONVERT_NULL   0xff       //表结束标志,用于源中
  #define NolinearConvert_long_t unsigned short  //内部乘法运算时的单位
#elif NOLINEAR_CONVERT_LEVEL == 4 //表为unsigned long型
  #define NolinearConvert_t unsigned long   //类型
  #define NOLINEAR_CONVERT_NULL   0xffffffff //表结束标志,用于源中
  #define NolinearConvert_long_t unsigned long long  //内部乘法运算时的单位

#else //NOLINEAR_CONVERT_LEVEL == 2 //默认,表为unsigned short型
  #define NolinearConvert_t unsigned short //类型
  #define NOLINEAR_CONVERT_NULL   0xffff    //表结束标志,用于源中
  #define NolinearConvert_long_t unsigned long  //内部乘法运算时的单位
#endif

#ifndef NOLINEAR_CONVERT_LUN_MAX   //查找表最大表项
  #define NOLINEAR_CONVERT_LUN_MAX  32
#endif

/****************************************************************************
                           相关结构
****************************************************************************/
//查找表阵列元素结构
struct _NolinearConvertTbl{
  NolinearConvert_t Source;          //源数据,即非线性数据,NOLINEAR_CONVERT_NULL结束
  NolinearConvert_t Destination;    //数据目标,即转换后的线性数据
};

/*******************************************************************************
                           相关函数
*******************************************************************************/

//-------------------------------非线性转换函数----------------------------------
//pLut: X轴为已上升排序查找表阵列入口，Y不限，均不为负
//计算为负时，将返回0
NolinearConvert_t NolinearConvert(NolinearConvert_t Source, //数据源
                                  const struct _NolinearConvertTbl *pLut,//NULL时=Source
                                  unsigned char LutSize);  //查找表大小,0时自动

//------------------------------得到查找表大小----------------------------------
//查找表定义有结束点时,可用此函数判断大小
unsigned char NolinearConvert_GetSize(const struct _NolinearConvertTbl *pLut);                     

//--------------------------查找表反向Copy函数----------------------------------
//即将pOrgLut交换x,y轴后copy到结果表中
void NolinearConvert_AntiCopy(struct _NolinearConvertTbl *pDeLut,//目标,需>=源空间
                              const struct _NolinearConvertTbl *pOrgLut,//源
                              unsigned char LutSize);  //源查找表大小,0时自动                   



#endif //#define __NOLINEAR_CONVERT_H
