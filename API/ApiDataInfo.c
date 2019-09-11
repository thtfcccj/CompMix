/***********************************************************************

        单例/多例模式对像/结构统一访问接口-数据信息定义相关实现

***********************************************************************/

#include "ApiDataInfo.h"

//大类占位(0其它)
static const unsigned char _TypeLenLut[4] = {1,2,4,0};

//标准固定类型占位:
static const unsigned char _StdTypeLenLut[] = {
  0, //0字符串型,长度由结束字符决定  
  4, //1单浮点数型,点用4Byte
  8, //2双浮点数型,点用8Byte
  2, //3两个独立Byte型
  //未定义
 

  4,
  8,2,0

};


/***********************************************************************
                             相关函数
***********************************************************************/

//-------------------------由信息获得数据占位------------------------------
//0表示为字符串需进行字符判断,异常或未定义时返回1
unsigned char ApiDataInfo_GetLen(unsigned char ApiDataInfo)
{
  //字符串时优先判断
  if(ApiDataInfo == API_DI_STD_STR) return 0; 
  
  unsigned char SubDataSize = ApiDataInfo >> API_DI_TYPE_SHIRT;
  
  //结构体时，结构体长度
  if((ApiDataInfo & API_DI_BX_MASK) == API_DI_STRUCT){
    return (SubDataSize << 4) * (ApiDataInfo & API_DI_STRUCT_LEN_MASK);
  }
  //Byte型判断
  unsigned char Len = _TypeLenLut[SubDataSize];
  if(Len) return Len;
  
  //自定义类型判断
  if((ApiDataInfo & API_DI_OTHER_MASK) == API_DI_OTHER_STD){ //标准数据类型
    if(ApiDataInfo >= API_DI_STD_END) return 1;//异常或未定义时返回1
    return _StdTypeLenLut[ApiDataInfo & API_DI_STD_MASK];
  }
    
  //用户类型判断
  if((ApiDataInfo & API_DI_OTHER_USER_MASK) == API_DI_USER_OTHER)
    return 4;//用户其它类型,返回4
  else return 2;//DAP2324格式的小数点与显示掩码
}

