/*******************************************************************************

                �¶�ģ��Ӳ����ض���-HC32�ڲ��¶�ʱ

*******************************************************************************/
#ifndef __TEMP_HC32_H
#define __TEMP_HC32_H

/*******************************************************************************
                            ���˵�� 
*******************************************************************************/
//----------------------HC32�ڲ��¶ȴ���������˵��------------------------------
  //�������ϣ����㹫ʽΪ�������¶� = 25 + 0.0803 �� Vref �� ( AdcValue �C Trim)
  //����ƫ��ΪSEN_TEMP_ZERO��64, �����ں�����7����:
  //����ƫ���¶� = 25 + 0.0803 �� Vref �� ( AdcValue �C Trim) + 64 - 7
  //             ->82 + 0.0803 �� Vref �� ( AdcValue �C Trim)       
  //����̶�ʹ���ڲ� 2.5V�ο������λ��Ϊ��0x00100C36  ��3�澫��
  //������ת��Ϊ����ļ��㹫ʽ �����¶� = (AdcValue - ZERO) * GAIN��
  //�������ϵת��, �����Ƶ�:
  //  ->                    89 + (0.0803 �� 2.5) �� ( AdcValue �C Trim)
  //  ->                    89 + 0.20075 �� ( AdcValue �C Trim)
  //  -> (0.20075 * 408.468) + 0.20075 �� (AdcValue �C Trim)
  //  ->  0.20075 X ((AdcValue �C Trim) + 408.468)
  //  ->  0.20075 X (AdcValue - (Trim - 408.468))  ��:

/*******************************************************************************
                            Ӳ����ض��� 
*******************************************************************************/
//����Ӳ�����ԣ�֧�ֶ��ֲ�ͬ���˲���ʽ:

//֧���˲�ʱ,ͬʱ���建�����, <255
//#define SUPPORT_TEMP_FILETER  10 

//֧�ּ�ƽ��
//#define SUPPORT_TEMP_FILETER_AVERVGE   

//֧��ƽ����ȥ�������Сֵ��ͬʱ����ǰ��ȥ������,<(SUPPORT_TEMP_FILETER / 3)
//#define SUPPORT_TEMP_FILETER_AVERVGE_MID    3

//Ĭ���¶����ADֵ   ZERO = (Trim - 408.468),ȡ��=(Trim - 408)�ٶ���Ϊ��:
#define TEMP_DEFAULT_ZERO ((*(unsigned short*)(0x00100C36) - 408))  

//Ĭ���¶�ADֵ Qֵ:  GAIN = 0.20075 (1 << SEN_TEMP_Q) = 6578
#define TEMP_DEFAULT_GAIN   6578

//����ʹ�õ�Qֵ
#define TEMP_GAIN_Q  15

//�²�궨ʱ����С�²�ֵ
#define TEMP_DIFF_TEPM_MIN   10 

//�²�궨ʱ����С�ź�ֵ
#define TEMP_DIFF_SIGAL_MIN  10   

#endif





