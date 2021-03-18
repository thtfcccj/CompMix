/*******************************************************************************

                            ������У��ģ��
��ģ��������Դ����(x��)����������Ӧ�ã�������ʱ��Ŀ������(y��)���ޣ�����ӦΪ��ֵ
*******************************************************************************/
#ifndef __NOLINEAR_CONVERT_H
#define __NOLINEAR_CONVERT_H

/****************************************************************************
                             �������
****************************************************************************/

//�Ƿ�֧��(��������)���ұ��壬��ʹ�ô˻��巴��
//ע��!!!!!!!!�˻����޼ӽ������ƣ�ϵͳ����Ϊȷ��ͬһʱ��ֻ����һ��λ��ʹ��!!!!!
//#define SUPPORT_NOLINEAR_CONVERT_BUF

#if NOLINEAR_CONVERT_LEVEL == 1 //��Ϊunsigned char��
  #define NolinearConvert_t unsigned char   //����
  #define NOLINEAR_CONVERT_NULL   0xff       //�������־,����Դ��
  #define NolinearConvert_long_t unsigned short  //�ڲ��˷�����ʱ�ĵ�λ
#elif NOLINEAR_CONVERT_LEVEL == 4 //��Ϊunsigned long��
  #define NolinearConvert_t unsigned long   //����
  #define NOLINEAR_CONVERT_NULL   0xffffffff //�������־,����Դ��
  #define NolinearConvert_long_t unsigned long long  //�ڲ��˷�����ʱ�ĵ�λ

#else //NOLINEAR_CONVERT_LEVEL == 2 //Ĭ��,��Ϊunsigned short��
  #define NolinearConvert_t unsigned short //����
  #define NOLINEAR_CONVERT_NULL   0xffff    //�������־,����Դ��
  #define NolinearConvert_long_t unsigned long  //�ڲ��˷�����ʱ�ĵ�λ
#endif

#ifndef NOLINEAR_CONVERT_LUN_MAX   //���ұ�������
  #define NOLINEAR_CONVERT_LUN_MAX  32
#endif

/****************************************************************************
                           ��ؽṹ
****************************************************************************/
//���ұ�����Ԫ�ؽṹ
struct _NolinearConvertTbl{
  NolinearConvert_t Source;          //Դ����,������������,NOLINEAR_CONVERT_NULL����
  NolinearConvert_t Destination;    //����Ŀ��,��ת�������������
};

#ifdef SUPPORT_NOLINEAR_CONVERT_BUF
  struct _NolinearConvertBuf{
    struct _NolinearConvertTbl Tbl[NOLINEAR_CONVERT_LUN_MAX + 1];//��β
  };
  extern struct _NolinearConvertBuf NolinearConvertBuf;
#endif

/*******************************************************************************
                           ��غ���
*******************************************************************************/

//-------------------------------������ת������----------------------------------
//pLut: X��Ϊ������������ұ�������ڣ�Y���ޣ�����Ϊ��
//����Ϊ��ʱ��������0
NolinearConvert_t NolinearConvert(NolinearConvert_t Source, //����Դ
                                  const struct _NolinearConvertTbl *pLut,//NULLʱ=Source
                                  unsigned char LutSize);  //���ұ��С,0ʱ�Զ�

//------------------------------�õ����ұ��С----------------------------------
//���ұ����н�����ʱ,���ô˺����жϴ�С
unsigned char NolinearConvert_GetSize(const struct _NolinearConvertTbl *pLut);                     

//--------------------------���ұ���Copy����----------------------------------
//����pOrgLut����x,y���copy���������
void NolinearConvert_AntiCopy(struct _NolinearConvertTbl *pDeLut,//Ŀ��,��>=Դ�ռ�
                              const struct _NolinearConvertTbl *pOrgLut,//Դ
                              unsigned char LutSize);  //Դ���ұ��С,0ʱ�Զ�                   

//--------------------------------�õ��ڽ�����----------------------------------
//��ʹ�ô˻��巴��,�˻����޼ӽ������ƣ�ϵͳ����Ϊȷ��ͬһʱ��ֻ����һ��λ��ʹ��
#ifdef SUPPORT_NOLINEAR_CONVERT_BUF
  //struct _NolinearConvertTbl NolinearConvert_*NolinearConvert_pGetBuf(void)  
  #define NolinearConvert_pGetBuf() (NolinearConvertBuf.Tbl)
#endif

#endif //#define __NOLINEAR_CONVERT_H
