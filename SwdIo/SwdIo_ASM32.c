/***********************************************************************

		                    SWD����IO���л�ģ��-��ASM32�е�ʵ��
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
  RCC->REGLOCK = RCC_REGLOCKKEY;
  RCC->IOMUX = ((unsigned long)0x5A69 << 16) | (0 << 0);
  RCC->REGLOCK = RCC_RESGLOCKKEY;
}
#endif  

//---------------------SWD����Ϊ��ͨIO�������0-------------------
#ifndef _APP_DEBUG   //��������ʱ
//Ӧ�ڳ�ʼ��ʱ����SwdIo_ToIo()�����ڲ���,���ô˺������û����0ģʽ
void SwdIo_IoOut0(void)
{
  //Ĭ������Ϊ������裬��������Ϊ���0����߿�������
  GPIOC->DOCL = (1 << 7);  GPIOC->DIR |= (1 << 7);
  GPIOD->DOCL = (1 << 1);  GPIOD->DIR |= (1 << 1);    
}
#endif

//-------------------------------���û�SWDģʽ---------------------------
#ifndef _APP_DEBUG   //��������ʱ
//ͬʱ��SWD������Ϊ����ģʽ������SWD����
void SwdIo_ToSwd(void)
{
  RCC->REGLOCK = RCC_REGLOCKKEY;
  RCC->IOMUX = ((unsigned long)0x5A69 << 16) | (1 << 0); 
  RCC->REGLOCK = RCC_RESGLOCKKEY;
  //���û�����ģʽ��ֹ����
  GPIOC->DIR &= ~(1 << 7); GPIOD->DIR &= ~(1 << 1);
  GPIOC->AFR &= ~((unsigned long)0x0f << (7 * 4)); 
  GPIOD->AFR &= ~((unsigned long)0x0f << (1 * 4)); 
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


