/*******************************************************************************

                     结构操作宏

*******************************************************************************/
#ifndef __STRUCT_H 
#define __STRUCT_H 


//---------------------------得到成员偏移函数-------------------------------
#define struct_offset(type, member) \
	((unsigned int)(&(((type *)0)->member))) 

//---------------------------由成员得到结构函数-------------------------------
#define struct_get(memberPtr, structType, structMember) \
	((structType*) ((char*) memberPtr - struct_offset(structType, structMember)))

#endif
