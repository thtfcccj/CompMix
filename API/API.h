/***********************************************************************

                                  API�ӿ�
//��ʵ��Ӧ�����
***********************************************************************/
#ifndef __API_H
#define	__API_H

#include "MultitonAPI.h"
#include "SingletonAPI.h"

/***********************************************************************
                             ��غ���
***********************************************************************/

//---------------------�����ͻ��˫�ֽڶ���MultitonAPI�����ӿ�---------------
//����NULL��ʾ��֧�ֵ����ͻ�����ID����
const struct _MultitonAPI *API_pGetMultitonD(unsigned char TypeId,
                                              unsigned short AryId);

//---------------------�����ͻ�õ��ֽڶ���MultitonAPI�����ӿ�---------------
//����NULL��ʾ��֧�ֵ����ͻ�����ID����
const struct _MultitonAPI *API_pGetMultitonS(unsigned char TypeId,
                                              unsigned char AryId);

//----------------------�����ͻ�õ���������SingletonAPI�����ӿ�-------------
//����NULL��֧��
const struct _SingletonAPI *API_pGetSingleton(unsigned char TypeId);

//--------------------------������API����ͬ��ͨ��------------------------
//д����ʱ�������Ƿ�д������(��ʱӦ��ֹ���±�������)
//ʵ��Ӧ�����
signed char  API_MultitonSync(const struct _MultitonAPI *pApi,
                              unsigned char TypeId,
                              unsigned short AryId, //����ID
                              unsigned char MsgType); //NET_FUN_MSG_TYPE_MASK��Ϣ����


#endif //#define	__API_DATA_INFO_H

