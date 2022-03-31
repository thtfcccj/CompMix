/************************************************************************

                       动态Id筛选器实现

************************************************************************/

#include "DynaIdFilter.h"  //筛选器
#include "math_3.h"
#include <string.h>

/************************************************************************
                          相关函数实现
************************************************************************/

//----------------------------填充查找表---------------------------------
#ifdef SUPPORT_DYNA_ID_FILTER_LSB
  #define _FullLUT(d,p)  pLsbS2Full(d,p)
#else
  #define _FullLUT(d,p)  pMsbS2Full(d,p)
#endif

//----------------------------动态Id填充处理-------------------------
//PosOrId: b14~15!=0: 要先根据阵列ID时,查找位置，找到后返回该位置(pLUT首位)和对应页查找表 
//MSB双字节方式填充查找表,查找表为NULL时返回-1表示未找到，NULL找到
unsigned char* DynaIdFilter_Full(struct _DynaIdFilter *pFilter,//主结构
                                 unsigned short TblLen,     //查找表长度
                                 const _FilterFun funFilter, //筛选函数,禁止为NULL
                                 unsigned long Para,        //形参中可能带入的参数
                                 unsigned short PosOrId,    //读取位置或阵列ID
                                 unsigned char RdLen,       //读取长度
                                 unsigned char *pLUT)      //要填充的查找表NULL不需要
{
  unsigned short EndRdPos;
  //用于找到用户起始位置
  unsigned short CurUserPos = 0;
  unsigned short CurLocalPos = 0;  
  unsigned short RdPos;//读取位置
  
  //=======阵列ID时，需先查找到对应ID位置并回对应读取起始位置==============
  if(PosOrId & 0xC000){
    PosOrId &= 0x3fff;//为ID了
    RdPos = 0;
    for(; CurLocalPos < TblLen; CurLocalPos++){
      if(!funFilter(CurLocalPos, Para)) continue; //不满足条件
      RdPos++; //满足筛选条件了
      //检查阵列ID时,是否符合条件
      if(funFilter(0x8000 | CurLocalPos, Para) == PosOrId) break;
    }
    if(CurLocalPos >= TblLen) goto _ErrReturn; //没找到,错误返回
    //找到并转换为读取位置了,若在首页，则需指向0并继续
    if(RdPos < RdLen){//首页信息初始化
      //CurUserPos = 0; 没用过
      CurLocalPos = 0;
      RdPos = 0;
    }
    else CurUserPos = RdPos;//不在首页，直接对应(CurLocalPos也定位了)
    pLUT = _FullLUT(RdPos, pLUT); //首位填充查找表实际阵列值
    goto _ToRdPos; //直接读取
  }
  else RdPos = PosOrId;//为读取位置
  if(RdPos >= TblLen) goto _ErrReturn;  //读取位置起过表位置了

  //============================检查命中情况==============================
  //首次读取
  if(RdPos == 0) goto _ToRdPos; 
  
  //前几个，或无效
  if((RdPos < RdLen) || !DynaIdFilter_IsValid(pFilter))
    goto _ToRdPos;
  //定位失败
  if(pFilter->LocalPos >= TblLen) goto _ToRdPos;
  //先缓冲
  CurUserPos = pFilter->UserPos;
  CurLocalPos = pFilter->LocalPos;   
  //缓冲直接命中了
  if(RdPos == CurUserPos) goto _MsbSFull;  
  //else 缓冲未命中

  //=======================查到并移动到读取位置=============================
_ToRdPos: //到读取位置
    //正向查找
  if(RdPos >= CurUserPos){
    for(; CurLocalPos < TblLen; CurLocalPos++){
      if(!funFilter(CurLocalPos, Para)) continue; //不满足条件
      //找到了
      if(CurUserPos == RdPos)goto _MsbSFull;//到指定位置了
      CurUserPos++;  //用户位置下一个
    }
  }
  else{//反向查找
    for(; CurLocalPos > 0; CurLocalPos--){
      if(!funFilter(CurLocalPos, Para)) continue; //不满足条件
      //找到了
      if(CurUserPos == RdPos) goto _MsbSFull;//到指定位置了
      CurUserPos--;  //用户位置下一个
    }
  }
_ErrReturn: //错误返回
  //找完也没找到 
  if(pLUT == NULL) return (unsigned char*)-1;//不需要填充时
  return pLUT; //没找到不填充
  
  //=======================找到起始位置后填充数据=============================
_MsbSFull: //找到起始位置了，填充
  //先更新筛选器
  pFilter->UserPos = RdPos;
  pFilter->LocalPos = CurLocalPos; 
  
  if(pLUT == NULL) return NULL; //不需要填充时
  
  //得到并纠正用户读取结束位置
  EndRdPos = RdPos + RdLen;
  if(EndRdPos > TblLen) EndRdPos = TblLen;
  
  for(; CurLocalPos < TblLen; CurLocalPos++){
    if(!funFilter(CurLocalPos, Para)) continue; //不满足条件
    pLUT = _FullLUT(funFilter(0x8000 | CurLocalPos, Para), pLUT); //填充查找表实际阵列值
    RdPos++;  //用户位置下一个    
    if(RdPos >= EndRdPos) break; //填充完了
  }
  return pLUT;
}


