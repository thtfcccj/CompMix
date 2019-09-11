/***********************************************************************

                  定义标准数据类型的简化版

为兼容性考虑外，不推荐使用简化定义
***********************************************************************/
#ifndef __DEF_STD_TYPE_H
#define __DEF_STD_TYPE_H

//--------------------------------读写unsigned定义----------------------------
#ifndef u32
  #define u32 unsigned long
#endif

#ifndef u16
  #define u16 unsigned short
#endif
  
#ifndef u8
  #define u8 unsigned char
#endif

//--------------------------------只读unsigned定义----------------------------
#ifndef uc32
  #define uc32 const unsigned long
#endif
  
#ifndef uc16
  #define uc16 const unsigned short
#endif  
  
#ifndef uc8
  #define uc8 const unsigned char
#endif 
  
//--------------------------------字符串 char定义 ----------------------------
#ifndef c8
  #define c8 char
#endif
  
#ifndef cc8
  #define cc8 const char
#endif
  
//--------------------------------读写signed定义----------------------------
#ifndef vu32
  #define vu32 signed long
#endif

#ifndef vu16
  #define vu16 signed short
#endif
  
#ifndef vu8
  #define vu8 signed char
#endif
  
//--------------------------------只读signed定义----------------------------
#ifndef vuc32
  #define vuc32 const signed long
#endif
  
#ifndef vuc16
  #define vuc16 const signed short
#endif  
  
#ifndef vuc8
  #define vuc8 const signed char
#endif  
  

#endif //__DEF_STD_TYPE_H


 