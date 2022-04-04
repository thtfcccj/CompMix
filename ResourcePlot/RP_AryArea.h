/******************************************************************************

                         ���������ͼģ��
��ģ������ڲ˵������Ӧ��,���֧��ÿҳ256��
******************************************************************************/
#ifndef __RP_ARY_AREA_H 
#define __RP_ARY_AREA_H 
  
/***********************************************************************
                             ˵��
***********************************************************************/
//�����з�ʽ��֯��ÿ�����൱��һ���̶�����,�磺ÿ��2��3�У�
//              0   �� ��    1
//              2   �� ��    3
//              4   �� ��    5

/***********************************************************************
                           �������
***********************************************************************/

#ifndef RP_ARY_AREA_KEY_KEEP_OV    
  #define RP_ARY_AREA_KEY_KEEP_OV   10//������ͣ��ʱ��
#endif

#ifndef RP_ARY_AREA_AUTO_SCREEN_OV    
  #define RP_ARY_AREA_AUTO_SCREEN_OV   10//�Զ�����ʱ��
#endif

#ifndef RP_ARY_AREA_REFRESH_PERIOD    
  #define RP_ARY_AREA_REFRESH_PERIOD   10//��ʵʱ�����Զ�����ʱ��
#endif

/***********************************************************************
                             ��ؽṹ
***********************************************************************/
//����
#include "ResourcePlot.h"
#include "Color.h"

//---------------------------��������Ϣ------------------------------
struct _RP_AryArea_STypeInfo{
  unsigned long SType;  //������,��handle�е�RP_HANDLE_STYPE_MASKλ
  //������handle�£�������ͼ����������ұ���Ϣ��
  unsigned char IconBase;  //ͼ����ϢID���ұ���ʼ
  unsigned char IconCount; //ͼ����ϢID���ұ����
  unsigned char ParaBase;  //������ϢID���ұ���ʼ
  unsigned char ParaCount; //������ϢID���ұ���� 
};

//---------------------------���ṹ----------------------------------
struct _RP_AryArea{
  
  const struct _RpMainAreaDesc *pMainDesc; //��������
  const struct _RpAryAreaDesc *pAryDesc;   //������ 
  unsigned long Handle;                   //����� 
  struct _RP_AryArea_STypeInfo  STypeInfo; //��ȡ���ĵ�ǰ��������Ϣ
  struct _RpFixAreaDesc FDesc;        //���ڻ����������ݵĹ̶������ṹ
  //�ڲ�����
  unsigned short AryCount;       //���и���  
  unsigned short AryStart;       //��ҳ��������ʼ
  unsigned char Focus;           //ҳ�ڽ���,��ѡ��ĵڼ���
  unsigned char PrvFocus;        //��ҳ����,��ҳ�ڽ�����ͬʱ��������ҳ
  
  unsigned char PlotMask;        //��ͼ�������룬b0���,b1��ͼ,b2����,b7����
  
  unsigned char AutoPageTimer;   //�Զ���ҳ���ƶ�ʱ��
  unsigned char RefreshTimer;    //��ͨ���ݸ��¶�ʱ��  
  //unsigned char DyncTimer;      //������ʱ��,�����ڷ�ҳ��Ч(�ݲ�֧��)
};

/************************************************************************
                           ��غ���
*************************************************************************/

//-----------------------------��ʼ������----------------------------
void RP_AryArea_Init(struct _RP_AryArea *pArea,    //���ṹ
                     const struct _RpMainAreaDesc *pMainDesc,//��������
                     const struct _RpAryAreaDesc *pAryDesc, //������                     
                     unsigned long Handle, //����� 
                     unsigned short AryCount);    //��ʼ��ʱ���д�С

//----------------------------��������----------------------------
//1s���ҵ���һ��
void RP_AryArea_Task(struct _RP_AryArea *pArea); 

//-------------------------------ˢ�����к���----------------------------
//���������ؽ���������
#define RP_AryArea_RefreshAll(area) do{(area)->PlotMask = 0xff; }while(0)

//-------------------------------ˢ����ͨ���ݺ���----------------------------
//���������ؽ���ͨ����
#define RP_AryArea_RefreshNor(area) do{(area)->RefreshTimer = 0x06; }while(0)

//-----------------------------����������----------------------------
//GuideKey����Ϊ: ȷ��0��1��2��3��4, 5��һҳ��6��һҳ,�����ּ�'1'-'9'
void RP_AryArea_Key(struct _RP_AryArea *pArea, unsigned char GuideKey); 

/******************************************************************************
                               �ص�����
*******************************************************************************/

//-------------------����Handle������ID�õ������������Ϣ----------------------
//����0������������Item�ˣ����������pSTypeInfo
signed char RP_AryArea_cbGetSTypeInfo(unsigned long Handle, 
                                      unsigned short AryId,                        
                                      struct _RP_AryArea_STypeInfo *pSTypeInfo);

//------------------------------ĳ�ѡ��------------------------------------
//��ĳ��(����)�ϰ���ȷ�ϡ����󣬽����ô˺���
void RP_AryArea_cbSelFinal(unsigned long Handle,
                           unsigned short AryId);//��ѡ����


#endif



