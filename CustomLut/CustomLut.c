/***********************************************************************

                    �û��ɶ��Ʋ��ұ�ģ��ʵ��
��ģ�������ϵͳ,����Ӧ���޹�
ע:��֧�ֵ��������Ĳ��ұ�ṹ
***********************************************************************/
#include "CustomLut.h"

/***********************************************************************
                     ��غ���ʵ��
***********************************************************************/

//-----------------------���ұ�У�麯��----------------------------
//�����ұ��Ƿ��д���,����ʱ����0,���򷵻س���λ��
signed char CustomLut_Check(struct _CustomLut *pCustomLut)
{
  unsigned char Size = CustomLut_GetSize(pCustomLut);
  if(Size < 2) return -1;//��������ܹ���һ��ֱ��
  Size--;
  
  //����Ƿ�Ϊ��������
  for(unsigned char Pos = 0; Pos < Size; Pos++){
    if(pCustomLut->Info.Tbl[Pos + 1].Source < pCustomLut->Info.Tbl[Pos].Source)
      return -1;//Դ���ݴ���
    if(pCustomLut->Info.Tbl[Pos + 1].Destination < pCustomLut->Info.Tbl[Pos].Destination)
      return -1;//Ŀ�����ݴ���
  }
  return 0;//����У����ȷ
}  

//--------------------------------��ʼ������------------------------------------
void CustomLut_Init(struct _CustomLut *pCustomLut,
                    unsigned char CustomLutId,//�˱��ID�ţ����ڶ������ֽṹ
                    signed char Inited)//�β�Ϊ�ѳ�ʼ����־
{
  //memset(&pCustomLut,0,sizeof(struct _CustomLut)); //��Info������ע���
  pCustomLut->CustomLutId = CustomLutId;
  if(!Inited){//δʵʼ��ʱ��ʼ��
    CustomLut_Restore(pCustomLut);
  }
  else{
    CustomLut_cbPosSave(pCustomLut);
    if(CustomLut_Check(pCustomLut)) 
      CustomLut_Restore(pCustomLut);//�����ұ����ʱ,�ָ�Ĭ������
  }
}  

//-----------------------�õ����ұ��С����---------------------------
unsigned char CustomLut_GetSize(struct _CustomLut *pCustomLut)
{
  //��ѭ���ұ�,������SourceΪNOLINEAR_CONVERT_NULLʱ,����Ϊ���ұ����
  unsigned char Pos = 0;
  for(; Pos < CUSTOM_LUT_LUT_SIZE; Pos++){
    if(pCustomLut->Info.Tbl[Pos].Source == NOLINEAR_CONVERT_NULL) break;
  }
  return Pos;
}  

//-----------------------���Ը���һ�����ұ����-------------------------
//�����Ƿ񸲸ǳɹ�(0�ɹ�,�������ɹ�)
signed char CustomLut_ReplaceItem(struct _CustomLut *pCustomLut,
                                   NolinearConvert_t Source,//Դ����
                                   NolinearConvert_t Destination)//Ŀ������
{
  unsigned char Size = CustomLut_GetSize(pCustomLut);
  if(Size >= CUSTOM_LUT_LUT_SIZE) return -1; //����,��ֹ����
  //1.�Ȳ���Դ�����Ƿ��ڸ��Ƿ�Χ
  unsigned char Pos = 0;
  NolinearConvert_t PrvSource;
  for(; Pos < Size; Pos++){
    PrvSource = pCustomLut->Info.Tbl[Pos].Source;
    if(Source > PrvSource){
      //�ȵ�ǰλ��ֵ���ڷ�Χ
      if((Source - PrvSource) < CustomLut_cbGetNearSorceDiff(pCustomLut->CustomLutId)) break; 
    }
    else{
      //�ȵ�ǰλ��ֵС���ڷ�Χ
      if((PrvSource - Source) < CustomLut_cbGetNearSorceDiff(pCustomLut->CustomLutId)) break;
    } 
  }
  if(Pos >= Size) return -1;//û�нӽ���
  
  //2.ǿ�ƽ��˼�¼���ǣ���У�鸲�Ǻ�������Ƿ�ͨ��
  pCustomLut->Info.Tbl[Pos].Source = Source;
  NolinearConvert_t PrvDestination = pCustomLut->Info.Tbl[Pos].Destination;
  pCustomLut->Info.Tbl[Pos].Destination = Destination;
  if(!CustomLut_Check(pCustomLut)){//���ǳɹ�,��������
     CustomLut_cbPushSave(pCustomLut);
     return 0;
  }   
  
  //3.��У�鲻�ԣ���˵�������ݲ�����Ҫ��,��ԭ֮ǰ������
  pCustomLut->Info.Tbl[Pos].Source = PrvSource;
  pCustomLut->Info.Tbl[Pos].Destination = PrvDestination;
  return 1;//�ɲ��룬���������ݲ���
}  

//-----------------------����һ�����ұ����-------------------------
//�����Ƿ����ɹ�
signed char CustomLut_AddItem(struct _CustomLut *pCustomLut,
                              NolinearConvert_t Source,//Դ����
                              NolinearConvert_t Destination)//Ŀ������
{
  unsigned char Size = CustomLut_GetSize(pCustomLut);
  if(Size >= CUSTOM_LUT_LUT_SIZE) return -1; //����,��ֹ����

  //1.�ҵ������
  unsigned char Insert = 0;
  for(; Insert < Size; Insert++){
    if(Source <= pCustomLut->Info.Tbl[Insert].Source) break;
  }
  //2.����ǰ�����ݼ��Ŀ�������Ƿ���ȷ
  if(Insert == 0){//�������׸�ʱ
    if(Destination >= pCustomLut->Info.Tbl[0].Destination) 
    return -1;//���׸�����Ҫ��(��Ƚ�Ϊ��ǰ)
  }
  else if(Insert >= Size){//�������׸�ĩ��ʱ
    if(Destination < pCustomLut->Info.Tbl[Insert - 1].Destination) 
      return -1;//���׸���ĩ����ҪС
    goto _EndSave;//��ȷʱֱ�Ӳ���ĩ��
  }
  else{//���м�ʱ
    if(Destination <= pCustomLut->Info.Tbl[Insert - 1].Destination)
      return -1;  //��ǰ1�����ݻ�ҪС
    if(Destination >= pCustomLut->Info.Tbl[Insert].Destination)
      return -1;  //�Ⱥ�1�����ݻ�Ҫ��(��Ƚ�Ϊ��ǰ)
  }
  //3.��������Ч���������ƶ�(�������ʼ�ƶ���ֹ����)
  for(unsigned char Pos = Size; Pos > Insert; Pos--){
    pCustomLut->Info.Tbl[Pos].Source = pCustomLut->Info.Tbl[Pos - 1].Source;
    pCustomLut->Info.Tbl[Pos].Destination = pCustomLut->Info.Tbl[Pos - 1].Destination;
  }
  //4.������в���������
  _EndSave:
    pCustomLut->Info.Tbl[Insert].Source = Source;
    pCustomLut->Info.Tbl[Insert].Destination = Destination;
    Size++;//������
    if(Size < CUSTOM_LUT_LUT_SIZE)//δ��ʱ��д�����ַ�
       pCustomLut->Info.Tbl[Size].Source = NOLINEAR_CONVERT_NULL;
    CustomLut_cbPushSave(pCustomLut);

  return 0;//����ɹ�            
}  

//-----------------------ɾ��һ�����ұ����-------------------------
//�����Ƿ�ɹ�
signed char CustomLut_DelItem(struct _CustomLut *pCustomLut,
                              unsigned char Item)//��λ��
{
  unsigned char Size = CustomLut_GetSize(pCustomLut);
  if(Size <= 2) return -1; //2�㹹��һ����,������ɾ����
  if(Item >= Size) return -1;//���ڱ���
  Size -= 1;//��ʾ��ĩ��������
  //��������Ч������ǰ�ƶ�
  for(; Item < Size; Item++){
    pCustomLut->Info.Tbl[Item].Source = pCustomLut->Info.Tbl[Item + 1].Source;
    pCustomLut->Info.Tbl[Item].Destination = pCustomLut->Info.Tbl[Item + 1].Destination;
  }
  //����������ý�����־
  pCustomLut->Info.Tbl[Item].Source = NOLINEAR_CONVERT_NULL;

  //��������
  CustomLut_cbPushSave(pCustomLut);
             
  return 0;//ɾ���ɹ� 
}

//-----------------------���ұ���Ĭ��ֵ����----------------------------
//�����ұ��ó���ǰ״̬
void CustomLut_Restore(struct _CustomLut *pCustomLut)
{
  CustomLut_cbFullDefaultLut(pCustomLut->Info.Tbl);
  CustomLut_cbPushSave(pCustomLut);
}

