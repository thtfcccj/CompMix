/*******************************************************************************

                    用户可定制查找表模块
此模块独立与系统,即与应用无关
注:仅支持单调上升的查找表结构
********************************************************************************/
#ifndef __CUSTOM_LUT_H
#define __CUSTOM_LUT_H

/*******************************************************************************
                                相关配置
*******************************************************************************/

//定义索引表大小,即允许最大多少个点
#ifndef CUSTOM_LUT_LUT_SIZE
  #define CUSTOM_LUT_LUT_SIZE    10     //默认最大10个点
#endif

/*******************************************************************************
                                 相关结构
*******************************************************************************/
#include "Eeprom.h"
#include "NolinearConvert.h"
//信息结构
struct _CustomLutInfo{
  //个性查找表,以0xff结尾
  struct _NolinearConvertTbl Tbl[CUSTOM_LUT_LUT_SIZE];
};

//运行结构
struct _CustomLut{
  unsigned char CustomLutId;         
  struct _CustomLutInfo Info; //信息结构
};

/********************************************************************************
                        相关函数
********************************************************************************/

//--------------------------------初始化函数------------------------------------
void CustomLut_Init(struct _CustomLut *pCustomLut,
                    unsigned char CustomLutId,//此表的ID号，用于对外区分结构
                    signed char Inited);//形参为已初始化标志

//----------------------------得到查找表函数------------------------------------
#define CustomLut_pGetLut(customLut)   ((customLut)->Info)

//----------------------------得到查找表项函数-------------------------------
#define CustomLut_GetLutItem(customLut, item)  ((customLut)->Info.Tbl[item])

//----------------------------得到查找表大小函数--------------------------------
unsigned char CustomLut_GetSize(struct _CustomLut *pCustomLut);

//-----------------------尝试覆盖一个查找表项函数------------------------------
//返回是否覆盖成功(0成功,其它不成功)
signed char CustomLut_ReplaceItem(struct _CustomLut *pCustomLut,
                                  NolinearConvert_t Source,//源数据
                                  NolinearConvert_t Destination);//目标数据

//----------------------------增加一个查找表项函数------------------------------
//返回是否插入成功
signed char CustomLut_AddItem(struct _CustomLut *pCustomLut,
                               NolinearConvert_t Source,//源数据
                               NolinearConvert_t Destination);//目标数据

//----------------------------删除一个查找表项函数------------------------------
//返回是否成功
signed char CustomLut_DelItem(struct _CustomLut *pCustomLut,
                               unsigned char Item);//项位置

//----------------------------查找表置默认值函数--------------------------------
//将查找表置出厂前状态
void CustomLut_Restore(struct _CustomLut *pCustomLut);

/********************************************************************************
                        回调函数
********************************************************************************/

//----------------------------得以信息结构基址----------------------------------
EepromAdr_t CustomLut_cbGetInfoBase(unsigned char CustomLutId);

//----------------------------填充默认查找表结构函数----------------------------
//应根据应用情况决定,查找表大小为CUSTOM_LUT_LUT_SIZE
void CustomLut_cbFullDefaultLut(struct _NolinearConvertTbl *pTbl);

//----------------------------得到临近点源数据差值函数-------------------------
//此函数用于插入数据时,判断插入的数据是否重复,若重复，将覆盖原来的值
//不支持个性查找表时,可不实现此函数
unsigned char CustomLut_cbGetNearSorceDiff(unsigned char CustomLutId);


#endif //#define __CUSTOM_LUT_H
