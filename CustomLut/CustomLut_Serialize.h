/******************************************************************************

               CustomLut���л�(Serialize)�뷴���л�(Deserialize)�ӿ�

*******************************************************************************/
#ifndef _CUSTON_LUT_SERIALIZE_H
#define _CUSTON_LUT_SERIALIZE_H

/******************************************************************************
                           �����ӿ�
*******************************************************************************/
#include "CustomLut.h"

//-------------------------------���л�����--------------------------------
//������ pCustomLut->Info.Tbl[]���л������������ؽ���λ��
unsigned char *CustomLut_pSerialize(const struct _CustomLut *pCustomLut,
                                     unsigned char *pData);//���н��ջ�����

//-----------------------------�����л�����--------------------------------
//���������ݻָ������أ����ػ���������λ��
const unsigned char *pCustomLut_Deserialize(struct _CustomLut *pCustomLut,
                                            const unsigned char *pData);


#endif  //#define _CUSTON_LUT_SERIALIZE_H



