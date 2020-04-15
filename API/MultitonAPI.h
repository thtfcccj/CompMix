/***********************************************************************

                  ����ģʽ����/�ṹ����ͳһ���ʽӿڶ���
//��������Ӧ<65536
***********************************************************************/
#ifndef __MULTITON_API_H
#define	__MULTITON_API_H

#include "ApiId.h" //ApiHandle_t

/*******************************************************************************
                            �ӿں���ԭ�Ͷ���
*******************************************************************************/

//-----------------�õ���д����(һ��Ϊ���ã���ʱ��Ϊ�Գ�������)-------------------
typedef void (*MultitonAPI_GetData_t)(unsigned char *pData,//���վݻ�����
                                      ApiHandle_t Handle, //���+����ID
                                      unsigned short AryId);//��������ID��

//-----------------���ö�д����(һ��Ϊ���ã���ʱ��Ϊ�Գ�������)--------------
//����д���
typedef unsigned char (*MultitonAPI_SetData_t)(const unsigned char *pData,//��Ӧ����
                                      ApiHandle_t Handle,//���+����ID
                                      unsigned short AryId);//��������ID��

//----------------------------�õ�ֻ��������-----------------------------------
typedef void (*MultitonAPI_GetDataOnly_t)(unsigned char *pData,//���վݻ�����
                                          ApiHandle_t Handle, //���+����ID
                                          unsigned short AryId);//��������ID��                                  

//----------------------------ִ�б���ָ��-----------------------------------
//����ִ�н��(��3Bit)+��ʹ�ò�������(��5Bit)
typedef unsigned char (*MultitonAPI_ExeCmd_t)(ApiHandle_t Handle,//���+����ID
                                              const unsigned char *pPara,//��������ʱΪNULL
                                              unsigned short AryId);//��������ID��

//-------------------------�õ�������Сֵ-------------------------------
typedef void (*MultitonAPI_GetDataMin_t)(unsigned char *pData,//���վݻ�����
                                         ApiHandle_t Handle,//���+����ID
                                         unsigned short AryId);//��������ID��

//-------------------------�õ��������ֵ-------------------------------
typedef void (*MultitonAPI_GetDataMax_t)(unsigned char *pData, //���վݻ�����
                                         ApiHandle_t Handle,//���+����ID
                                         unsigned short AryId);//��������ID��

//------------------------------�õ�����ͷ----------------------------------
//���ر���ͷ�ַ���
typedef const char *(*MultitonAPI_GetHeader_t)(unsigned char IsOnly,  //��д����(0)orֻ������
                                               ApiHandle_t Handle);//���+����ID  
                       
//--------------------------------�õ���������----------------------------------
//�����ַ���
typedef const char *(*MultitonAPI_GetContent_t)(char *pBuf,  //�����õ��Ļ�����
                                  unsigned char IsOnly,  //��д����(0)orֻ������
                                  ApiHandle_t Handle,  //���+����ID
                                  unsigned short AryId); //��������ID��

//--------------------------------�õ���������----------------------------------
//����ApiIdType��ʶ
typedef unsigned char (*MultitonAPI_GetType_t)(unsigned char IsOnly,  //��д����(0)orֻ������
                                                 unsigned char FunId, //����ID
                                                 unsigned short AryId); //��������ID��

/*******************************************************************************
                            �����ṹ
*******************************************************************************/
//ʵ��ʱ��û�д���ʱ����ΪNULL,����˳�������ApiID����
struct _MultitonAPI{
  //ͨѶ��UI��:
  MultitonAPI_GetData_t GetData;         //�õ���д����(һ��Ϊ���ã���ʱ��Ϊ�Գ�������)
  MultitonAPI_GetDataOnly_t GetDataOnly; //�õ�ֻ��������
  MultitonAPI_SetData_t SetData;         //���ö�д����(һ��Ϊ���ã���ʱ��Ϊ�Գ�������)
  MultitonAPI_ExeCmd_t ExeCmd;           //ִ��ָ��
  //UI��:
  MultitonAPI_GetDataMin_t GetDataMin;    //�õ���д������Сֵ(����ʱʹ��)
  MultitonAPI_GetDataMax_t GetDataMax;    //�õ���д�������ֵ(����ʱʹ��)
  MultitonAPI_GetHeader_t GetHeader;      //�õ�����ͷ(��д����/ֻ�����ݹ���)
  MultitonAPI_GetContent_t GetContent;    //ֱ�ӵõ��ַ�����Ϣ(��д����/ֻ�����ݹ���)
  MultitonAPI_GetType_t    GetType;       //�ַ���Ϣ(����������)
};

#endif //#define	__MULTITON_API_H

