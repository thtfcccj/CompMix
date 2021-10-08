/***********************************************************************

           (������)���Ʒ�����ת�����ұ�(NolinearConvertTbl)ģ��ʵ��

***********************************************************************/

#include "CustomLut.h"

#include "InfoBase.h"
#include "Eeprom.h"

struct _NolinearConvertTbl CustomLut[CUSTOM_LUT_LUT_SIZE];

/***********************************************************************
                     ��غ���ʵ��
***********************************************************************/

//-----------------------���ұ�У�麯��----------------------------
//�����ұ��Ƿ��д���,����ʱ����0,���򷵻س���λ��
signed char CustomLut_Check(void)
{
  unsigned char Size = CustomLut_GetSize();
  if(Size < 2) return -1;//��������ܹ���һ��ֱ��
  Size--;
  
  //����Ƿ�Ϊ��������
  for(unsigned char Pos = 0; Pos < Size; Pos++){
    if(CustomLut[Pos + 1].Source < CustomLut[Pos].Source)
      return -1;//Դ���ݴ���
    if(CustomLut[Pos + 1].Destination < CustomLut[Pos].Destination)
      return -1;//Ŀ�����ݴ���
  }
  return 0;//����У����ȷ
}  

//-----------------------��ʼ������-------------------------------
void CustomLut_Init(signed char Inited)//�β�Ϊ�ѳ�ʼ����־
{
  //memset(&CustomLut,0,sizeof(struct _CustomLut)); ��Info������ע����   
  if(!Inited){//δʵʼ��ʱ��ʼ��
    CustomLut_Restore();
  }
  else{
    Eeprom_Rd(CustomLut_GetInfoBase(),
              &CustomLut,
              sizeof(CustomLut));
    if(CustomLut_Check()) 
      CustomLut_Restore();//�����ұ����ʱ,�ָ�Ĭ������
  }
}  

//-----------------------�õ����ұ��С����---------------------------
unsigned char CustomLut_GetSize(void)
{
  //��ѭ���ұ�,������SourceΪNOLINEAR_CONVERT_NULLʱ,����Ϊ���ұ����
  unsigned char Pos = 0;
  for(; Pos < CUSTOM_LUT_LUT_SIZE; Pos++){
    if(CustomLut[Pos].Source == NOLINEAR_CONVERT_NULL) break;
  }
  return Pos;
}  

//-----------------------�Ƿ�Ϊͬһ����------------------------------
//��0�ڷ�Χ��0���ڷ�Χ
signed char CustomLut_IsSame(NolinearConvert_t Center,//��������
                             NolinearConvert_t Cur)   //��ǰ����
{
  if(Cur >= Center){
    //�ȵ�ǰλ��ֵ���ڷ�Χ
    if((Cur - Center) < CustomLut_cbGetNearSorceDiff()) 
      return 1;
  }
  else{//�ȵ�ǰλ��ֵС���ڷ�Χ
    if((Center - Cur) < CustomLut_cbGetNearSorceDiff()) 
      return 1;//�ڷ�Χ
  }
  return 0;//���ڷ�Χ
}

//-----------------------������Դ���Ҷ�Ӧ���------------------------------
//����Χ��ʱ����Ϊ�ҵ��ˣ�����λ�ã���δ�ҵ�
signed char CustomLut_FindItem(NolinearConvert_t Source)//Դ����
{
  unsigned char Size = CustomLut_GetSize();
	
  unsigned char Pos = 0;
  NolinearConvert_t PrvSource;
  for(; Pos < Size; Pos++){
    if(CustomLut_IsSame(CustomLut[Pos].Source, Source)) return Pos;//�ҵ���
  }
  return -1;//û�нӽ���
}

//-----------------------���Ը���һ�����ұ����-------------------------
//�����Ƿ񸲸ǳɹ�(0�ɹ�,�������ɹ�)
signed char CustomLut_ReplaceItem(NolinearConvert_t Source,//Դ����
                                 NolinearConvert_t Destination)//Ŀ������
{
  unsigned char Size = CustomLut_GetSize();
  if(Size >= CUSTOM_LUT_LUT_SIZE) return -1; //����,��ֹ����
	if(CustomLut_cbIsDisModify(Destination)) return -1;//��ֹ�޸�ʱ,��ֹ����
  //1.�Ȳ���Դ�����Ƿ��ڸ��Ƿ�Χ
	signed char Pos = CustomLut_FindItem(Source);
	if(Pos < 0) return Pos;//δ�ҵ�
	
	
  //2.ǿ�ƽ��˼�¼���ǣ���У�鸲�Ǻ�������Ƿ�ͨ��
	NolinearConvert_t PrvSource = CustomLut[Pos].Source;
  CustomLut[Pos].Source = Source;
  NolinearConvert_t PrvDestination = CustomLut[Pos].Destination;
  CustomLut[Pos].Destination = Destination;
  if(!CustomLut_Check()){//���ǳɹ�,��������
    Eeprom_Wr(CustomLut_GetInfoBase(),
              &CustomLut,
              sizeof(CustomLut)); 
     return 0;
  }   
  
  //3.��У�鲻�ԣ���˵�������ݲ�����Ҫ��,��ԭ֮ǰ������
  CustomLut[Pos].Source = PrvSource;
  CustomLut[Pos].Destination = PrvDestination;
  return 1;//�ɲ��룬���������ݲ���
}  

//-----------------------����һ�����ұ����-------------------------
//�����Ƿ����ɹ�
signed char CustomLut_AddItem(NolinearConvert_t Source,//Դ����
                              NolinearConvert_t Destination)//Ŀ������
{
  unsigned char Size = CustomLut_GetSize();
  if(Size >= CUSTOM_LUT_LUT_SIZE) return -1; //����,��ֹ����

  //1.�ҵ������
  unsigned char Insert = 0;
  for(; Insert < Size; Insert++){
    if(Source <= CustomLut[Insert].Source) break;
  }
  //2.����ǰ�����ݼ��Ŀ�������Ƿ���ȷ
  if(Insert == 0){//�������׸�ʱ
    if(Destination >= CustomLut[0].Destination) 
    return -1;//���׸�����Ҫ��(��Ƚ�Ϊ��ǰ)
  }
  else if(Insert >= Size){//�������׸�ĩ��ʱ
    if(Destination < CustomLut[Insert - 1].Destination) 
      return -1;//���׸���ĩ����ҪС
    goto _EndSave;//��ȷʱֱ�Ӳ���ĩ��
  }
  else{//���м�ʱ
    if(Destination <= CustomLut[Insert - 1].Destination)
      return -1;  //��ǰ1�����ݻ�ҪС
    if(Destination >= CustomLut[Insert].Destination)
      return -1;  //�Ⱥ�1�����ݻ�Ҫ��(��Ƚ�Ϊ��ǰ)
  }
  //3.��������Ч���������ƶ�(�������ʼ�ƶ���ֹ����)
  for(unsigned char Pos = Size; Pos > Insert; Pos--){
    CustomLut[Pos].Source = CustomLut[Pos - 1].Source;
    CustomLut[Pos].Destination = CustomLut[Pos - 1].Destination;
  }
  //4.������в���������
  _EndSave:
    CustomLut[Insert].Source = Source;
    CustomLut[Insert].Destination = Destination;
    Size++;//������
    if(Size < CUSTOM_LUT_LUT_SIZE)//δ��ʱ��д�����ַ�
       CustomLut[Size].Source = NOLINEAR_CONVERT_NULL;
    Eeprom_Wr(CustomLut_GetInfoBase(),
              &CustomLut,
              sizeof(CustomLut)); 

  return 0;//����ɹ�            
}  

//-----------------------����һ�����ұ����-------------------------
void CustomLut_InsertItem(NolinearConvert_t Source,//Դ����
                          NolinearConvert_t Destination)//Ŀ������
{
  //�����Բ��ұ����Ƿ�����ͬ��,��ʱ�ſ��Ը���
  if(CustomLut_ReplaceItem(Source, Destination))
    CustomLut_AddItem(Source, Destination);//����ֱ�Ӳ���
}

//-----------------------ɾ��һ�����ұ����-------------------------
//�����Ƿ�ɹ�
signed char CustomLut_DelItem(unsigned char Item)//��λ��,���λΪ0x80ʱǿ��ɾ��
{
  unsigned char Size = CustomLut_GetSize();
  if(Size <= 2) return -1; //2�㹹��һ����,������ɾ����

  if(Item >= Size) return -1;//���ڱ���	
	if(Item & 0x80) Item &=~0x80;//ǿ��ɾ��ʱ������д	
	else{//���,��ֹ�޸�ʱ,��ֹ����
		if(CustomLut_cbIsDisModify(CustomLut[Item].Destination))
			return -1;
  }

  Size -= 1;//��ʾ��ĩ��������
  //��������Ч������ǰ�ƶ�
  for(; Item < Size; Item++){
    CustomLut[Item].Source = CustomLut[Item + 1].Source;
    CustomLut[Item].Destination = CustomLut[Item + 1].Destination;
  }
  //����������ý�����־
  CustomLut[Item].Source = NOLINEAR_CONVERT_NULL;

  //��������
  Eeprom_Wr(CustomLut_GetInfoBase(),
            &CustomLut,
            sizeof(CustomLut));
             
  return 0;//ɾ���ɹ� 
}

//-----------------------���ұ���Ĭ��ֵ����----------------------------
//�����ұ��ó���ǰ״̬
void CustomLut_Restore(void)
{
  CustomLut_cbFullDefaultLut();
  Eeprom_Wr(CustomLut_GetInfoBase(),
            &CustomLut,
            sizeof(CustomLut));
}

