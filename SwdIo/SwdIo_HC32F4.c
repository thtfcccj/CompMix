/***********************************************************************

		                    SWD����IO���л�ģ��-��HC32F4�е�ʵ��
��ģ�������SWD��IO��֮����л���Ҳ������ͨ���ر�SWD���ӽ���
***********************************************************************/

#include "SwdIo.h"
#include "CMSIS.h"

#ifdef  SUPPORT_SWD_IO_UNLOCK //֧��SWDͨ��IO����ʱ
  unsigned char SwdIo_LockTimer = SWD_IO_UNLICK_TIME;
#endif

/***********************************************************************
		                      ��غ���ʵ��
***********************************************************************/

//---------------------SWD������Ϊ��ͨIO-------------------------------
#ifndef _APP_DEBUG   //��������ʱ
void SwdIo_ToIo(void)
{
  M4_PORT->PWPR = 0xA501;
  M4_PORT->PSPCR = 0; 
  M4_PORT->PWPR = 0xA500;    
}
#endif  

//---------------------SWD����Ϊ��ͨIO�������0-------------------
#ifndef _APP_DEBUG   //��������ʱ
//Ӧ�ڳ�ʼ��ʱ����SwdIo_ToIo()�����ڲ���,���ô˺������û����0ģʽ
void SwdIo_IoOut0(void)
{
  M4_PORT->PWPR = 0xA501;
  M4_PORT->PCRA14 = 0x32;
  M4_PORT->PCRA13 = 0x32;  
  M4_PORT->PFSRA14 = 0;
  M4_PORT->PFSRA13 = 0;
  M4_PORT->PWPR = 0xA500;      
}
#endif

//-------------------------------���û�SWDģʽ---------------------------
#ifndef _APP_DEBUG   //��������ʱ
//ͬʱ��SWD������Ϊ����ģʽ������SWD����
void SwdIo_ToSwd(void)
{
  M4_PORT->PWPR = 0xA501; 
  M4_PORT->PSPCR = 0x0B; //������ȼ�
  //���û�����ģʽ��ֹ����
  M4_PORT->PCRA14 = 0;
  M4_PORT->PCRA13 = 0;  
  M4_PORT->PFSRA14 = 0; 
  M4_PORT->PFSRA13 = 0;
  M4_PORT->PWPR = 0xA500; 
}
#endif

//----------------------��ͨ������-------------------------------
#ifdef  SUPPORT_SWD_IO_UNLOCK //֧��SWDͨ��IO����ʱ
//����256ms������ɨ��
void SwdIo_Task(void)
{
  if(SwdIo_cb_IsInvalid()){//��Чʱ
    SwdIo_LockTimer = SWD_IO_UNLICK_TIME;
    //ע���˿ڿ�������ģ����ʹ�ã��ʲ��ܻ���

  }
  else{//�͵�ƽ��
    if(!SwdIo_LockTimer){//�������
      SwdIo_ToSwd();
    }
    else SwdIo_LockTimer--;
  }
}
#endif //SUPPORT_SWD_IO_UNLOCK


