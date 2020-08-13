/*******************************************************************************

                     �ṹ������

*******************************************************************************/
#ifndef __STRUCT_H 
#define __STRUCT_H 


//---------------------------�õ���Աƫ�ƺ���-------------------------------
#define struct_offset(type, member) \
	((unsigned int)(&(((type *)0)->member))) 

//---------------------------�ɳ�Ա�õ��ṹ����-------------------------------
#define struct_get(memberPtr, structType, structMember) \
	((structType*) ((char*) memberPtr - struct_offset(structType, structMember)))

#endif
