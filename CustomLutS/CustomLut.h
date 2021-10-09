/***********************************************************************

               (单例化)定制非线性转换查找表模块
此模块独立与系统,即与应用无关
注:仅支持单调上升的查找表结构
***********************************************************************/
#ifndef __CUSTOM_LUT_H
#define __CUSTOM_LUT_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif

/*********************************************************************
                        相关配置
*********************************************************************/

//定义索引表大小,即允许最大多少个点
#ifndef CUSTOM_LUT_LUT_SIZE
  #define CUSTOM_LUT_LUT_SIZE    8
#endif

/*********************************************************************
                        相关结构
*********************************************************************/

#include "NolinearConvert.h"
//此结构保存EEPROM
extern struct _NolinearConvertTbl CustomLut[CUSTOM_LUT_LUT_SIZE];

/*********************************************************************
                        相关函数
*********************************************************************/

//-----------------------初始化函数---------------------------------------
void CustomLut_Init(signed char Inited);

//-----------------------得到查找表函数------------------------------------
//const struct _NolinearConvertTbl *CustomLut_GetLut(void);
#define CustomLut_GetLut()  (CustomLut)

//-----------------------得到查找表项函数------------------------------------
//const struct _NolinearConvertTbl *CustomLut_GetLutItem(unsigned char Item);
#define CustomLut_GetLutItem(item)  (&CustomLut[item])

//-----------------------得到查找表大小函数--------------------------------
unsigned char CustomLut_GetSize(void);

//-----------------------是否为相拟项数据------------------------------
//非0在范围，0还在范围
signed char CustomLut_IsSimilar(NolinearConvert_t Center,//中心数据
                                NolinearConvert_t Cur);   //当前数据

//-----------------------由数据源查找相似项函数------------------------------
//在误差范围内时，认为找到了，返回位置，负未找到
signed char CustomLut_FindSimilarItem(NolinearConvert_t Source);//源数据

//-----------------------由数据源查找相同项函数------------------------------
//相同时，认为找到了，返回位置，负未找到
signed char CustomLut_FindSameItem(NolinearConvert_t Source);//源数据

//-----------------------尝试覆盖一个查找表项函数-------------------------
//返回是否覆盖成功(0成功,其它不成功)
signed char CustomLut_ReplaceItem(NolinearConvert_t Source,//源数据
                                   NolinearConvert_t Destination);//目标数据

//-----------------------增加一个查找表项函数------------------------------
//返回是否插入成功
signed char CustomLut_AddItem(NolinearConvert_t Source,//源数据
                               NolinearConvert_t Destination);//目标数据

//-----------------------插入一个查找表项函数-------------------------
void CustomLut_InsertItem(NolinearConvert_t Source,//源数据
                          NolinearConvert_t Destination);//目标数据

//-----------------------删除一个查找表项函数------------------------------
//返回是否成功
signed char CustomLut_DelItem(unsigned char Item);//项位置,最高位为0x80时强制删除

//-----------------------查找表置默认值函数---------------------------------
//将查找表置出厂前状态
void CustomLut_Restore(void);

/******************************************************************************
                        回调函数
******************************************************************************/

//-----------------------填充默认查找表结构函数--------------------------------
//应根据应用情况决定,查找表大小为CUSTOM_LUT_LUT_SIZE
void CustomLut_cbFullDefaultLut(void);

//-----------------------得到临近点源数据差值函数------------------------------
//此函数用于插入数据时,判断插入的数据是否重复,若重复，将覆盖原来的值
//不支持个性查找表时,可不实现此函数
unsigned char CustomLut_cbGetNearSorceDiff(void);

//-----------------------目标数据是否禁止修改------------------------------------
//内部在覆盖与删除前，会调用此函数
signed char CustomLut_cbIsDisModify(NolinearConvert_t Destination);//目标数据


#endif //#define __CUSTOM_LUT_H
