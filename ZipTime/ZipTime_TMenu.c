/*******************************************************************************

                         ѹ��ʱ������-��TMenuϵͳ�еĲ˵�ʵ��
��ģ��ͨ���ص�������Ӧ��
*******************************************************************************/

#include "ZipTime_TMenu.h"
#include "ZipTime.h"
#include "TMenu_MNumAdj.h"

#include <string.h>

//����
static const struct _MNumDesc _TimeSetDesc[] = {
  {MNUM_TYPE_DEC, ZipTime_cbGetStartYear(), ZipTime_cbGetStartYear() + 63},//��
  {MNUM_TYPE_DEC, 1, 12},//��
  {MNUM_TYPE_DEC, 1, 31},//��
  {MNUM_TYPE_DEC, 0, 23},//ʱ
  {MNUM_TYPE_DEC, 0, 59},//��  
};

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
    unsigned long ZipTime = 
      ZipTime_FromTime(pUser->Value[0] - ZipTime_cbGetStartYear(),
                       pUser->Value[1],
                       pUser->Value[2],
                       pUser->Value[3],
                       pUser->Value[4],
                       0);               //�̶�Ϊ0
    Menu_ZipTime_cbSet(ZipTime);
    TMenu_Note(NULL); //��ʾ�ɹ�
    break; 
  }
  case TM_NOTIFY_MNUM_GET_DESC:{ //�õ���ֵ����
    memcpy(&pUser->Desc, 
           &_TimeSetDesc[pUser->CurItem], 
           sizeof(struct _MNumDesc));
    break;
  }
  default:break;
  }
}

//--------------------------����˵��ṹ----------------------------
#ifdef TM_EN_MUTI_LAN            //����ѡ���Ͷ������ʱ
  //�ݲ�֧�ֳ���ѡ�����ַ���
#elif defined(TM_RESOURCE_LAN)  //����Դ�ļ���ȡ�������ʱ
  static const LanCode_t  _TimeSet[]= {ZIP_TIME_YMENU_PID, 0};
  static const LanCode_t _Year[] = {ZIP_TIME_YMENU_PID, 1};
  static const LanCode_t _Month[] = {ZIP_TIME_YMENU_PID, 2};
  static const LanCode_t _Day[] = {ZIP_TIME_YMENU_PID, 3};
  static const LanCode_t _Hour[] = {ZIP_TIME_YMENU_PID, 4};
  static const LanCode_t _Minute[] = {ZIP_TIME_YMENU_PID, 5};
#else  //��һ�����ַ���ʱ
  static const char _TimeSet[]= {"ʱ������"};
  static const char _Year[] = {"��"};
  static const char _Month[] = {"��"};
  static const char _Day[] = {"��"};
  static const char _Hour[] = {"ʱ"};
  static const char _Minute[] = {"��"};
#endif

static const char * const _TimeAry[] = {
  _Year, _Month, _Day, _Hour, _Minute
};

const TMenu_t Menu_ZipTime = {//�˵��ṹ
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE, //�˵�����Ϊ��ֵ����ģʽģʽ���û�����־
  5,                          //�ɲ˵����;�����������ݴ�С
  _TimeSet,                //�˵�ͷ,ΪNULLʱ�ӻص����ȡ
  &ZIP_TIME_TMENU_PARENT,      //���ѵĸ��˵�
  _TimeAry,                //������ѵ��Ӳ˵���������ͷ
  _Notify,                    //���û��ռ佻����ͨ������
};

