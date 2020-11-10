/***********************************************************************

                  ����/����ģʽ����/�ṹͳһ���ʽӿ�-������Ϣ����

***********************************************************************/
#ifndef __API_DATA_INFO_H
#define	__API_DATA_INFO_H

/***********************************************************************
                         ����������ض���
***********************************************************************/
//DI = DATA+INFO
//---------------------------���嶨��---------------------------------
//���嶨��:
#define API_DI_TYPE_SHIRT     6  //���࣬������
#define API_DI_TYPE_MASK      (3 << API_DI_TYPE_SHIRT)

#define API_DI_TYPE_OTHER_MASK   0x3C //�ɴ������
//---------------------------���ඨ��---------------------------------
//����1Byte������(��ṹ������),������Byte������ʱ����
#define API_DI_TYPE_B1        (0 << API_DI_TYPE_SHIRT)   
//����2Byte������,������Byte������ʱ����
#define API_DI_TYPE_B2        (1 << API_DI_TYPE_SHIRT)   
//����4Byte������,������Byte������ʱ����
#define API_DI_TYPE_B4        (2 << API_DI_TYPE_SHIRT)    
//������������
#define API_DI_TYPE_OTHER     (3 << API_DI_TYPE_SHIRT)    

//---------------------------1/2/4Byteʱ���ඨ��---------------------------------
#define API_DI_BX_MASK        0x30  //����
//#define API_DI_XXXX         0x30  //����
#define API_DI_STRUCT         0x20  //�ṹ���ʶ
#define API_DI_SIGN           0x10  //������������
#define API_DI_UNSIGN         0x00  //�޷���������

//====================�������ͻ��޷����������Ӷ���================
#define API_DI_DOT            0x08  //С�����ʶ��������С����
#define API_DI_DOT_POS_MASK   0x07  //С����ʱ��ʾС����λ��
#define API_DI_LEN_MASK       0x07  //��С����ʱΪ������ʾ����,0��ʾ�ɱ䳤��(ǰ��0����ʾ) 

//========================�ṹ��ʱ����==========================
//�ṹ���ӳ�������
#define API_DI_STRUCT_LEN_MASK   0x0f 

//�ṹ���û�����:1Byte��ʾ 0-15, 2Byte��ʾ16-31, 3Byte��ʾ32-47,��϶���Ϊ:
#define API_DI_STRUCT_BASE0   (API_DI_STRUCT | (0 << API_DI_TYPE_SHIRT))
#define API_DI_STRUCT_BASE16  (API_DI_STRUCT | (1 << API_DI_TYPE_SHIRT))
#define API_DI_STRUCT_BASE32  (API_DI_STRUCT | (2 << API_DI_TYPE_SHIRT))
//�ṹ�嶨��ʾ��:
//<16Byteʱ:    API_DI_STRUCT_BASE0 | sizeof(struct _???)
//16-31Byteʱ:  API_DI_STRUCT_BASE16 | sizeof(struct _???)
//32-47Byteʱ:  API_DI_STRUCT_BASE32 | sizeof(struct _???)
//���������������㲢����(structLen����<48Byte):
#define ApiId_StructLen(structLen) \
  (API_DI_STRUCT | (((structLen) & 0x30) << 2) | ((structLen) & 0x0f))

//---------------------------������ʱ���ඨ��---------------------------------
#define API_DI_OTHER_MASK    0x20   //���ִ���
#define API_DI_OTHER_STD     0x20   //��׼��̶��ͣ�����Ϊ�û�������������
#define API_DI_OTHER_USER    0x00   //�û������������ͱ�־(API_DI_OTHER_STDû��λ)
    
//========================��׼��̶������ݶ���========================
//��׼����������ֱ��ʹ��:
#define API_DI_STD_MASK     (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x1f)
//�ַ�����,�����ɽ����ַ�����
#define API_DI_STD_STR      (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x00)   
//����������,����4Byte
#define API_DI_STD_SF       (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x01) 
//˫��������,����8Byte
#define API_DI_STD_DF       (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x02)
//3������Byte��
#define API_DI_STD_B3       (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x03)
//���¿ɼ�����չ:
    
//��׼���ͽ���λ��(����):     
#define API_DI_STD_END      (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x04)
//û����(�ݲ��ṩ֧��,254)
#define API_DI_UNDEF        (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x1E)
//����ʶ�������(��255)
#define API_DI_UNUSD        (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x1F)

//========================�û���������ʱ========================
//����������������:
#define API_DI_OTHER_USER_MASK  (API_DI_TYPE_OTHER | API_DI_OTHER_USER | 0x10)   
    
//DAP2324��ʽ��С��������ʾ����ǰ׺(ֱ��ʹ��)��ռ��0x0f,����Ϊ2
#define API_DI_DOT_DISP     (API_DI_TYPE_OTHER | API_DI_OTHER_USER | 0) 
//�����û���������ǰ׺(ֱ��ʹ��)
#define API_DI_USER_OTHER   (API_DI_TYPE_OTHER | API_DI_OTHER_USER | 0x10) 
 
/***********************************************************************
                         д����ʱ�����ؽ������
***********************************************************************/
#define API_DI_WR_MASK     7  //����峤��

#define API_DI_WR_WAIT    0  //�ȴ����(��ת��Ϊ��״̬)
#define API_DI_WR_FINAL   1  //д�ɹ�
#define API_DI_WR_ERR_IN  2  //������Ϣ��ȫ
#define API_DI_WR_ERR_OV  3  //�������ݲ����Ϲ���   
#define API_DI_WR_ARY_OV  6  //����ID����
#define API_DI_WR_UNDDEF  7  //δ����

/***********************************************************************
                         ִ������ʱ, ���ؽ������
***********************************************************************/
#define API_DI_CMD_MASK    7  //����峤��

#define API_DI_CMD_WAIT    0  //�ȴ����(��ת��Ϊ��״̬)
#define API_DI_CMD_FINAL   1  //�ɹ�
#define API_DI_CMD_ERR_IN  2  //������Ϣ��ȫ
#define API_DI_CMD_ERR_STATE  6  //״̬������
#define API_DI_CMD_UNDDEF  7  //δ����

//Api������ָ���������,��Ҫ����ͨѶ��Ҫ�����豸�㲻��Ҫ�˲�����Ӧ�ó���
struct _ApiCmdParaBuf{
  unsigned short DevId;  //ָ���豸
  unsigned char FunId;   //������
  unsigned char Para[3]; //����Ĳ�����
};   
    
/***********************************************************************
                             ��غ���
***********************************************************************/

//-------------------------����Ϣ�������ռλ------------------------------
//0��ʾΪ�ַ���������ַ��ж�,�쳣��δ����ʱ����1
unsigned char ApiDataInfo_GetLen(unsigned char ApiDataInfo);



#endif //#define	__API_DATA_INFO_H

