/*******************************************************************************

                         ѹ��ʱ������-��TMenuϵͳ�еĲ˵�ʵ��
��ģ��ͨ���ص�������Ӧ��
*******************************************************************************/

#include "ZipTime_TMenu.h"
#include "TMenu_MNumAdj.h"
#include "UiTips.h"

#include <string.h>

#include "ZipTime.h"
#include "RTC.h" //RTC_SetRTC

//----------------------------�ڲ��ַ���Դ------------------------------- 
#ifdef TM_EN_MUTI_LAN            //����ѡ���Ͷ������ʱ
  //�ݲ�֧�ֳ���ѡ�����ַ���
#elif defined(TM_RESOURCE_LAN)  //����Դ�ļ���ȡ�������ʱ
  //�˵�
  #include "lsAry.h"
  //��ʾ
  #include "Rid.h"
  #include "RidNote.h"
  #define ls_OpFinal   RID_pGeNote(RID_NOTE_OP_FINAL)
#else  //��һ�����ַ���ʱ
  //�˵�
  static const char ls_Year[] = {"��"};
  static const char ls_Month[] = {"��"};
  static const char ls_Day[] = {"��"};
  static const char ls_Hour[] = {"ʱ"};
  static const char ls_Minute[] = {"��"};
  static const char ls_TimeSet[]= {"ʱ������"};
  //��ʾ
  extern const char ls_OpFinal[]= {"�����ɹ�!"};
  
#endif

//����
const struct _MNumDesc TimeSetDesc[] = {
  {MNUM_TYPE_DEC, ZipTime_cbGetStartYear(), ZipTime_cbGetStartYear() + 63},//��
  {MNUM_TYPE_DEC, 1, 12},//��
  {MNUM_TYPE_DEC, 1, 31},//��
  {MNUM_TYPE_DEC, 0, 23},//ʱ
  {MNUM_TYPE_DEC, 0, 59},//��  
};

//ʱ���Ӧ���ַ�
const LanCode_t* const lsAry_Time[] = {
  ls_Year, ls_Month, ls_Day, ls_Hour, ls_Minute};

/*******************************************************************************
                           ���е���ʵ��
���úú��б��沢��ʾ
*******************************************************************************/

//----------------------------------�ص�����------------------------------- 
static void _Notify(unsigned char Type,//ͨ������
                    //���ݲ˵�����������������������
                    //������ṹ�ڿؼ��ﶨ��,ֱ��ָ��ת������
                    void *pv)
{
  struct _MNumAdjUser *pUser = (struct _MNumAdjUser *)pv;
  switch(Type){
  case TM_NOTIFY_GET_DATA:{ //����ǰֵװ��
    unsigned long ZipTime = ZipTime_GetRTC();
    pUser->Value[0] = ZipTime_GetInfo(0, ZipTime) + ZipTime_cbGetStartYear();
    pUser->Value[1] = ZipTime_GetInfo(1, ZipTime);
    pUser->Value[2] = ZipTime_GetInfo(2, ZipTime);
    pUser->Value[3] = ZipTime_GetInfo(3, ZipTime);
    pUser->Value[4] = ZipTime_GetInfo(4, ZipTime);
    
    break;
  }
  case TM_NOTIFY_SET_DATA:{ //�����趨ֵ
    RTC_SetRTC(pUser->Value[0],
               pUser->Value[1],
               pUser->Value[2],
               pUser->Value[3],
               pUser->Value[4],
               0);
    UiTips_UpdateS(ls_OpFinal); //��ʾ�ɹ�
    break; 
  }
  case TM_NOTIFY_MNUM_GET_DESC:{ //�õ���ֵ����
    memcpy(&pUser->Desc, 
           &TimeSetDesc[pUser->CurItem], 
           sizeof(struct _MNumDesc));
    break;
  }
  default:break;
  }
}


//--------------------------����˵��ṹ----------------------------

const TMenu_t ZipTime_TMenu = {//�˵��ṹ
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE, //�˵�����Ϊ��ֵ����ģʽģʽ���û�����־
  5,                          //�ɲ˵����;�����������ݴ�С
  ls_TimeSet,                //�˵�ͷ,ΪNULLʱ�ӻص����ȡ
  ZIP_TIME_TMENU_PARENT,      //���ѵĸ��˵�
  lsAry_Time,                //������ѵ��Ӳ˵���������ͷ
  _Notify,                    //���û��ռ佻����ͨ������
};

/*******************************************************************************
                    ÿ�������꼴����ʵ��
*******************************************************************************/

//----------------------------------�ص�����------------------------------- 
static void _TimeSetNotify(unsigned char Type,//ͨ������
                    //���ݲ˵�����������������������
                    //������ṹ�ڿؼ��ﶨ��,ֱ��ָ��ת������
                    void *pv)
{
  struct _MNumAdjUser *pUser = (struct _MNumAdjUser *)pv;
  unsigned long ZipTime = ZipTime_GetRTC();
  switch(Type){
  case TM_NOTIFY_GET_DATA:{ //����ǰֵװ��
    pUser->Value[0] = ZipTime_GetInfo(0, ZipTime) + ZipTime_cbGetStartYear();
    pUser->Value[1] = ZipTime_GetInfo(1, ZipTime);
    pUser->Value[2] = ZipTime_GetInfo(2, ZipTime);
    pUser->Value[3] = ZipTime_GetInfo(3, ZipTime);
    pUser->Value[4] = ZipTime_GetInfo(4, ZipTime);
    break;
  }
  case TM_NOTIFY_CHANGED:{ //��ֵ���ı�
    RTC_SetRTC(pUser->Value[0],
               pUser->Value[1],
               pUser->Value[2],
               pUser->Value[3],
               pUser->Value[4],
               ZipTime_GetInfo(5, ZipTime));
    break; 
  }
  case TM_NOTIFY_MNUM_GET_DESC:{ //�õ���ֵ����
    memcpy(&pUser->Desc, 
           &TimeSetDesc[pUser->CurItem], 
           sizeof(struct _MNumDesc));
    break;
  }
  default:break;
  }
}

//--------------------------����˵��ṹ----------------------------
const TMenu_t ZipTimeS_TMenu = {//�˵��ṹ
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE, //�˵�����Ϊ��ֵ����ģʽģʽ���û�����־
  5,                          //�ɲ˵����;�����������ݴ�С
  ls_TimeSet,                //�˵�ͷ,ΪNULLʱ�ӻص����ȡ
  ZIP_TIME_TMENU_PARENT2,   //���ѵĸ��˵�
  lsAry_Time,                //������ѵ��Ӳ˵���������ͷ
  _TimeSetNotify,            //���û��ռ佻����ͨ������
};


