/************************************************************************

                         动态Id筛选器
此模块主要用于动态创建Id号查找表时的过程管理
************************************************************************/
#ifndef __DYNA_ID_FILTER_H
#define __DYNA_ID_FILTER_H

/************************************************************************
                           相关配置
************************************************************************/

//定义ID号填充方式,默认MSB, 定义后为LSB
//#define SUPPORT_DYNA_ID_FILTER_LSB

/************************************************************************
                           相关结构
************************************************************************/

//筛选器用于各模块内部查找时形参带入
struct _DynaIdFilter{
  unsigned short UserPos;  //最后一次用户查找的位置
  unsigned short LocalPos; //最后一次用户查找的位置对应的本地数据位置
};

#define DYNA_ID_FILTER_INVALID    0xffff//用户查找的位置为0xffff表示无效

//-------------------------筛选函数定义----------------------------
//返回值由形参LutId决定：
//最高位清零时， 表示筛选函数, 返回非0表示满足条件(必须实现此功能)
//最高位置位时，表示获得最终阵列ID, 返回实际的阵列ID(填充查找表功能时支持,否则可不支持)
typedef unsigned short (*_FilterFun)(unsigned short LutId,//最高位置位表示获得最终阵列ID
                                       unsigned long Para); //用户参数(可转换为指针)

/************************************************************************
                          相关函数
************************************************************************/

//-----------------------------初始化------------------------------
#define DynaIdFilter_Init(paccelerate) \
  do{(paccelerate)->UserPos = DYNA_ID_FILTER_INVALID; }while(0)

//------------------------------是否有效--------------------------------
//使用筛选器前应调用，使用后应更新筛选器
#define DynaIdFilter_IsValid(paccelerate) \
   ((paccelerate)->UserPos != DYNA_ID_FILTER_INVALID)

//----------------------------动态Id填充处理-------------------------
//PosOrId: b14~15!=0: 要先根据设备ID查找位置，找到后返回该位置(pLUT首位)和对应页查找表 
//MSB双字节方式填充查找表,查找表为NULL时返回-1表示未找到，NULL找到
unsigned char* DynaIdFilter_Full(struct _DynaIdFilter *pFilter,//主结构
                                 unsigned short TblLen,     //查找表长度
                                 const _FilterFun funFilter, //筛选函数,禁止为NULL
                                 unsigned long Para,        //形参中可能带入的参数
                                 unsigned short PosOrId,    //读取位置或阵列ID
                                 unsigned char RdLen,       //读取长度
                                 unsigned char *pLUT);      //要填充的查找表NULL不需要
     

#endif //__DYNA_ID_FILTER_H


