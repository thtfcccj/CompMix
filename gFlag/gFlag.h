/*******************************************************************************

					            APP�����־ģ��
��ģ�齫ȫ����ɢ���ı�����ͳһ���й���
********************************************************************************/
#ifndef __GFLAG_H 
#define __GFLAG_H

//��ģ����ʵ�ֵ�Ӧ�ò�ӿ�
#include "gFlag_App.h" //����ʾ��ģ���gFlag_AppT.h

/****************************************************************************
                             �ṹ����
****************************************************************************/
struct _gFlag{
  unsigned char Bit[GFLAG_BIT_SIZE];      //λ��־
  unsigned char Byte[GFLAG_BYTE_SIZE];   //ͨ���ֽڱ�־
};
extern struct _gFlag gFlag;

/****************************************************************************
                              ��غ���
****************************************************************************/
//--------------------------------��ʼ������------------------------------
void gFlag_Init(void);

//--------------------------------λ��־����------------------------------
#define gFlag_BitIsSet(pos, bitMask)  (gFlag.Bit[pos] & (bitMask))
#define gFlag_SetBit(pos, bitMask)    do{gFlag.Byte[pos] |= bitMask; }while(0)  
#define gFlag_ClrBit(pos, bitMask)    do{gFlag.Byte[pos] &= ~(bitMask); }while(0)  

//-----------------------------�ֽ�λ����---------------------------------
#define gFlag_GetByte(pos)        (gFlag.Byte[pos])
#define gFlag_SetByte(pos, data)  do{gFlag.Byte[pos] = data; }while(0)                


#endif

