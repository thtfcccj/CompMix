/******************************************************************************

                     Rp���Handle������˵��

******************************************************************************/
#ifndef __RP_HANDLE_H 
#define __RP_HANDLE_H 

//--------------------------------Handle����----------------------------------
//Handle��ΪΨһ��ID����Ӧ�ò㽻������λ��Ϊ���¼����֣�

//ʵʱˢ�µ��ñ�־,����Ҫʵʱ����,�ڿ�ȡͼƬ���ַ���Դʱ���ɷ����쳣��ֹͣˢ��
#define RP_HANDLE_REAL_REFRESH 0x80000000

//���࣬�̶��������ڴ���ʶ��
#define RP_HANDLE_MTYPE_MASK   0x7f000000
//���࣬��RP_AryArea�ص����(ĳ���豸)
#define RP_HANDLE_STYPE_MASK   0x00ff0000

#define RP_HANDLE_TYPE_MASK   (RP_HANDLE_MTYPE_MASK | RP_HANDLE_STYPE_MASK)

//����ID��(ĳ���豸)
#define RP_HANDLE_ARY_MASK     0x00ffffff   



#endif  //#ifndef __RP_HANDLE_H 



