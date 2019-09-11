/***********************************************************************

                  ����ģʽ����/�ṹͳһ���ʽӿڶ���

***********************************************************************/
#ifndef __SINGLETON_API_H
#define	__SINGLETON_API_H

#include "ApiId.h" //ApiHandle_t

/*******************************************************************************
                            �ӿں���ԭ�Ͷ���
*******************************************************************************/

//-----------------�õ���д����(һ��Ϊ���ã���ʱ��Ϊ�Գ�������)-------------------
typedef void (*SingletonAPI_GetData_t)(unsigned char *pData,//���վݻ�����
                                       unsigned char FunId); //����ID

//-----------------���ö�д����(һ��Ϊ���ã���ʱ��Ϊ�Գ�������)--------------
//����д���
typedef unsigned char (*SingletonAPI_SetData_t)(const unsigned char *pData,//��Ӧ����
                                        unsigned char FunId); //����ID

//----------------------------�õ�ֻ��������-----------------------------------
typedef void (*SingletonAPI_GetDataOnly_t)(unsigned char *pData,//���վݻ�����
                                           unsigned char FunId); //����ID                               

//----------------------------ִ�б���ָ��-----------------------------------
//����ִ�н��(��3Bit)+��ʹ�ò�������(��5Bit)
typedef unsigned char (*SingletonAPI_ExeCmd_t)(unsigned char FunId, //����ID 
                                                 const unsigned char *pPara);//��������ʱΪNULL


//-------------------------�õ�������Сֵ-------------------------------
typedef void (*SingletonAPI_GetDataMin_t)(unsigned char *pData,//���վݻ�����
                                          unsigned char FunId); //����ID

//-------------------------�õ��������ֵ-------------------------------
typedef void (*SingletonAPI_GetDataMax_t)(unsigned char *pData, //���վݻ�����
                                          unsigned char FunId); //����ID

//------------------------------�õ�����ͷ----------------------------------
//���ر���ͷ�ַ���
typedef const char *(*SingletonAPI_GetHeader_t)(unsigned char IsOnly,  //��д����(0)orֻ������
                                                 unsigned char FunId); //����ID 
                       
//--------------------------------�õ���������----------------------------------
//�����ַ���
typedef const char *(*SingletonAPI_GetContent_t)(char *pBuf,  //�����õ��Ļ�����
                                  unsigned char IsOnly,  //��д����(0)orֻ������
                                  unsigned char FunId); //����ID

//--------------------------------�õ�������Ϣ----------------------------------
//����ApiIdType��ʶ
typedef unsigned char (*SingletonAPI_GetInfo_t)(unsigned char IsOnly,  //��д����(0)orֻ������
                                                  unsigned char FunId); //����ID

/*******************************************************************************
                            �����ṹ
*******************************************************************************/
//ʵ��ʱ��û�д���ʱ����ΪNULL,����˳�������ApiID����
struct _SingletonAPI{
  //ͨѶ��UI��:
  SingletonAPI_GetData_t GetData;         //�õ���д����(һ��Ϊ���ã���ʱ��Ϊ�Գ�������)
  SingletonAPI_GetDataOnly_t GetDataOnly; //�õ�ֻ��������
  SingletonAPI_SetData_t SetData;         //���ö�д����(һ��Ϊ���ã���ʱ��Ϊ�Գ�������)
  SingletonAPI_ExeCmd_t ExeCmd;           //ִ�б���ָ��(ֻд,UI/���Եȿ��Ʊ���ʱ��ʵ��)
  //UI��:
  SingletonAPI_GetDataMin_t GetDataMin;    //�õ���д������Сֵ(����ʱʹ��)
  SingletonAPI_GetDataMax_t GetDataMax;    //�õ���д�������ֵ(����ʱʹ��)
  SingletonAPI_GetHeader_t GetHeader;      //�õ�����ͷ(��д����/ֻ�����ݹ���)
  SingletonAPI_GetContent_t GetContent;    //ֱ�ӵõ��ַ�����Ϣ(��д����/ֻ�����ݹ���)
  SingletonAPI_GetInfo_t    GetInfo;      //������Ϣ
};

#endif //#define	__SINGLETON_API_H

