/*******************************************************************************
 *                      Info��Ϣ����-ʾ���ļ�   
                                   �����ļ����Ƶ�����Ŀ˽��Ŀ¼�в�������ΪInfo.h���޸�
 *��ģ�������ȷ��ϵͳ�洢����Ϣ��ַ
 *��Ҫ�洢��Ϣ�Ľṹ�����ڴ�ģ���ֹ������ַ
********************************************************************************/
#ifndef __INFO_BASE_H 
#define __INFO_BASE_H 

//---------------------------�õ���Աƫ�ƺ���-------------------------------
#define struct_offset(type, member) \
	((unsigned int)(&(((type *)0)->member))) 

/**********************************************************************
                       ��ģ��洢����
**********************************************************************/

//UvSensorģ��
struct _UvSensorInfoIni{
  unsigned char Reserved[8];//Ԥ���ռ�
};

//DA_Adjģ��
struct _DA_AdjInfoIni{
  unsigned char Reserved[8];//Ԥ���ռ�
};

//ModbusRtuMngģ��
struct _ModbusRtuMngInfoIni{
  unsigned char Reserved[8];//Ԥ���ռ�
};

/**********************************************************************
                        ��Ϣ���ṹ����
**********************************************************************/
//������Ϣ�����ռ��64Byte�ռ��С
typedef struct _InfoBase{
  #if INFO_BASE_RESERVED != 0
  unsigned char Reserved[INFO_BASE_RESERVED]; //�����ռ�
  #endif
   //0ַ�̶�Ϊ��ʼ����־,Ϊ0x5aʱ��ʾ�ѳ�ʼ��,����ʱΪδʹ��
  unsigned char InitedFlag;
  struct _UvSensorInfoIni UvSensorInfoIni;
  struct _DA_AdjInfoIni DA_AdjInfoIni;
  struct _ModbusRtuMngInfoIni ModbusRtuMngInfoIni;
  
}InfoBase_t;

/**********************************************************************
                        ��ػ�ַ������
**********************************************************************/
//InfoBase��ʼ����־
#define InfoBase_GetInitedFlagBase() struct_offset(InfoBase_t,InitedFlag)
//UvSensorģ��
#define UvSensor_GetInfoBase()  struct_offset(InfoBase_t, UvSensorInfoIni)
//DA_Adjģ��
#define DA_Adj_GetInfoBase()  struct_offset(InfoBase_t, DA_AdjInfoIni)
//ModbusRtuMngģ��
#define ModbusRtuMng_GetInfoBase()  struct_offset(InfoBase_t, ModbusRtuMngInfoIni)

#endif //#ifndef __INFO_BASE_H 

