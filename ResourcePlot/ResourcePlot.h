/******************************************************************************

                         ��Դ��Ϣ��ͼģ��

Ϊ����ز���������Դǰ׺��Rp(Resource Plot)��ͷ
******************************************************************************/
#ifndef __RESOURCE_PLOT_H 
#define __RESOURCE_PLOT_H 
#ifdef SUPPORT_EX_PREINCLUDE//��֧��Preinlude�r
  #include "Preinclude.h"
#endif
#include "Color.h"

/******************************************************************************

                             ��Դ�����ṹ

******************************************************************************/

//---------------------------------��������ṹ-------------------------------
//����Ļ���Ͻ�Ϊ��㣬ȫ��������Ϊ��λ
struct _RpRect{
  unsigned short x; //���������ؾ���
  unsigned short y; //���������ؾ���
  unsigned short w; //���ؿ��
  unsigned short h; //���ظ߶�
};

//--------------------------------����������ṹ---------------------------------
//����������������Ĭ�ϱ���ɫ
struct _RpMainAreaDesc{     //->�˽ṹ����Դ���е�ǰ׺��ʶ  "RPM_"
  struct _RpRect Rect;      //λ�÷�Χ��Ϣ
  Color_t cBg;               //Ĭ�ϱ���ɫ
};

//------------------------------�̶����������ṹ--------------------------------
//�����������Ϣ�����ô���Ϣ����һƬ������Ҫ���Ƶ���Ϣ����ͼ�������
struct _RpFixAreaDesc{     //->�˽ṹ����Դ���е�ǰ׺��ʶ"RPF_"
  struct _RpRect Rect;     //λ�÷�Χ��Ϣ
  Color_t cBg;              //������ı���ɫ
  unsigned char IconBase;  //ͼ����ϢID���ұ���ʼ
  unsigned char IconCount; //ͼ����ϢID���ұ����
  unsigned char ParaBase;  //������ϢID���ұ���ʼ
  unsigned char ParaCount; //������ϢID���ұ����  
};

//----------------------------���н����е�����������----------------------------
//���������ж����ͬ��������(��״̬���˵�)����ʱ��������������ͬ�Ľṹ�Ի��Ƹ���
struct _RpItemAreaDesc{    //->�˽ṹ����Դ���е�ǰ׺��ʶ  "RPX_"
  struct _RpRect Rect;     //λ�÷�Χ��Ϣ��x,yΪ���������е����λ��
  unsigned char  RowCount; //���������У������������и���
  unsigned char  ColCount; //���������У������������и���
  union{//����������Ϣ��˵���
    unsigned char IconBgId;  //����ͼID,������Ϊ���㣬����ͼΪδ�У��ڶ�ͼΪѡ��
    unsigned char IconBase;  //�˵�ͼID,������Ϊ���㣬����ͼΪδ�У��ڶ�ͼΪѡ��
  };
  unsigned char  BitInfo;  //λ��Ϣ��������
  Color_t cBg;              //������ı���ɫ
  Color_t cFocus;           //��ѡ��ʱ�߽���ɫ  
};
//λ��Ϣ����Ϊ��
#define RP_ITEM_MENU               0x80  //������Ϊ�˵���Ĭ��Ϊ����Ԫ��
#define RP_ITEM_DIS_FOCUS          0x40  //�޽�������������Ӧ���
#define RP_ITEM_ICON_BG_VALID      0x20  //����ͼͼƬ��Ч,����Ϊ����ɫ��ͼ
#define RP_ITEM_ICON_BG_FOCUS_EN   0x10  //����ͼƬ������Ч����ѡ��ʱʹ��ͼƬ
#define RP_ITEM_ID_MASK            0x0F  //Ӧ�ò�ʹ��ͬһ����ʱ���˽ṹ��ID��

//-----------------------------ͼ����Ϣ�ṹ------------------------------
//��״̬���е�ʱ��ͼ�꣬ʹ�ô˽ṹʱ��Ҫ��������������߶�,512������
struct _RpIconDesc{        //->�˽ṹ����Դ���е�ǰ׺��ʶ  "RPI_ARY"
  unsigned char Info;     //��Ϣλ��������
  unsigned char xl;       //λ��x���8λ
  unsigned char yl;       //λ��y���8λ 
};

//��Ϣλ����Ϊ��
#define RP_ICON_ID_MASK      0x3F  //b0~b5ͼ��ID
#define RP_ICON_X9           0x40  //b6=1ʱx��+256
#define RP_ICON_Y9           0x80  //b7=1ʱy��+256

//ע1��ϵͳ����ͬ���͵�ͼ��ͳһ���������ҳ���ͼ����ƺ������ұ���л��ơ�
//ע2��ͼ�����Ϊwmp��ʽʱ,��Դ�ļ�Ӧ�ṩͼ��ID��Ӧ��ǰ��ɫ�������ڻ��ơ�

//-----------------------------������Ϣ�ṹ------------------------------
//��״̬���е�ʱ�䣬ʹ�ô˽ṹʱ��Ҫ��������������߶�,512������
struct _RpParaDesc{        //->�˽ṹ����Դ���е�ǰ׺��ʶ  "RPP_ARY"
  unsigned char Info;     //��Ϣλ��������
  unsigned char xl;       //���λ��x���8λ
  unsigned char yl;       //λ��y���8λ
  unsigned char Para;     //���������������� 
};

//��Ϣλ����Ϊ��
#define RP_PARA_ID_MASK      0x3F  //b0~b5����ID
#define RP_PARA_X9           0x40  //b6=1ʱx��+256
#define RP_PARA_Y9           0x80  //b7=1ʱy��+256
//��������,����Ϊ��
#define RP_PARA_FONT_MASK    0x0F  //��ʾʹ�õ�����ID��


//ע1��ϵͳ����ͬ���͵Ļ�ͼ��������������У������ҳ��ú������и��ơ�
//ע2����Դ�ļ�Ӧ�ṩ����ID��ص���ɫ(�ɸ���״̬�л���ɫ)�����ڻ��ơ�

//-----------------------------�����ṹ����˵��-----------------------
//��Դ���е�����ǰ׺��ʶ����Ϊ
//��ɫ���У�ͳһ�ԡ�C_����ͷ�������;����Ϣ
//��ռ������2�ֽڶ��룬�Զ�����ɫ��

/***********************************************************************
                        �̶������ͼ���-���ӿں�����ص�����
***********************************************************************/

//----------------------------�̶������ͼ------------------------------
void RP_PaintFixArea(unsigned char Handle,
                     unsigned char PlotMask,//��������7b����,0-6b����0x7f����
                     const struct _RpFixAreaDesc *pDesc);


/********************************************************************************
                           �ص�����
********************************************************************************/

//----------------------------��ͼ��AryID��ͼ��Ϣ------------------------------
const struct _RpIconDesc *RP_cbGetIconDesc(unsigned char Handle,
                                           unsigned char IconAryId);

//----------------------------��ͼ��ID�ҵ�ͼ����------------------------------
const unsigned char *RP_cbGetIcon(unsigned char Handle,
                                   unsigned char IconId);

//----------------------------��ͼ��ID�ҵ�ͼ���ݴ�С----------------------------
unsigned long RP_cbGetIconSize(unsigned char Handle,
                               unsigned char IconId);

//----------------------------��ͼ��ID��ͼ��ǰ��ɫ------------------------------
//��ͼ��ID�ҵ�ͼ
Color_t RP_cbGetIconFg(unsigned char Handle,
                       unsigned char IconId);


//----------------------------�ɲ���AryID�Ҳ�����Ϣ----------------------------
const struct _RpParaDesc *RP_cbGetParaDesc(unsigned char Handle,
                                            unsigned char ParaAryId);

//----------------------------�ɲ���AryID���ַ���------------------------------
const char * RP_cbGetString(unsigned char Handle,
                             unsigned char ParaAryId);

//----------------------------�ɲ���ID��ͼ��ǰ��ɫ------------------------------
//��ͼ��ID�ҵ�ͼ
Color_t RP_cbGetParaFg(unsigned char Handle,
                       unsigned char ParaId);


#endif  //#ifndef __RESOURCE_PLOT_H 



