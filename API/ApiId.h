/***********************************************************************

                  ����/����ģʽ����/�ṹͳһ���ʽӿ�ID�Ŷ���
��ID������ͨѶʱ���ֲ�ͬ���ʽӿ�
***********************************************************************/
#ifndef __API_ID_H
#define	__API_ID_H


//---------------------------ApiID����----------------------------------
//������8������3Bit�����

//ͨѶ��UI��(0-3,���żд):
#define API_ID_GET_DATA          0  //�õ���д����(����ID=��д����ID)
#define API_ID_GET_DATA_ONLY     1  //�õ�ֻ��������(����ID=ֻ������ID)
#define API_ID_SET_DATA          2  //���ö�д����(����ID=��д����ID)
#define API_ID_EXE_CMD           3  //ִ�б���ָ��(����ID=�����ID)

//UI��(ȫ��Ϊ��)
#define API_ID_GET_DATA_MIN      4  //�õ���д������Сֵ(����ID=��д����ID)
#define API_ID_GET_DATA_MAX      5  //�õ���д�������ֵ(����ID=��д����ID)
#define API_ID_GET_HEADER        6  //�õ�����ͷ(�ٹ���ID=��д����ID��ֻ������ID)
#define API_ID_GET_CONTENT       7  //ֱ�ӵõ��ַ�����Ϣ(�ٹ���ID=��д����ID��ֻ������ID)

#define API_ID_COUNT             8 //����

//ע�٣� 
//    ���ڵõ�����ͷ��ֱ�ӵõ��ַ�����Ϣʱ�����ֶ�д����ID��ֻ������ID����,��Ӧ����أ�
//��ͨ������;���ж�(����ΪNetFunMsg_IsDataOnly())

//------------------------------ApiID���ඨ��---------------------------------
#define API_ID_IS_UI_ONLY        0x04 //4~7����ΪUI��Ҫ������,ȫ��Ϊ��
//ΪUI��ͨѶ����ʱ0-3����:
#define API_ID_IS_NO_RW_DATA     0x01 //1,3Ϊ�Ƕ�д����(��0,2�ɶ�д)
#define API_ID_IS_WR             0x02 //0,1Ϊ����2,4Ϊд

//API�Ƿ�Ϊ��д����:ֻ����������
#define ApiId_IsWr(apiId)  ((apiId == API_ID_SET_DATA) || (apiId == API_ID_EXE_CMD))

//---------------------------Api��������----------------------------------
//��Ϊϵͳȫ�ֶ��壺�����Сϵͳ���
//����ģʽʱ: ���ڻ�ȡ�����������ָ��͹���ID��

#ifndef ApiHandle_t
  #define ApiHandle_t  unsigned char
#endif

#endif //#define	__API_ID_H

