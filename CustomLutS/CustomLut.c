/***********************************************************************

           (单例化)定制非线性转换查找表(NolinearConvertTbl)模块实现

***********************************************************************/

#include "CustomLut.h"

#include "InfoBase.h"
#include "Eeprom.h"

struct _NolinearConvertTbl CustomLut[CUSTOM_LUT_LUT_SIZE];

/***********************************************************************
                     相关函数实现
***********************************************************************/

//-----------------------查找表校验函数----------------------------
//检查查找表是否有错误,无误时返回0,否则返回出错位置
signed char CustomLut_Check(void)
{
  unsigned char Size = CustomLut_GetSize();
  if(Size < 2) return -1;//两个点才能构成一条直线
  Size--;
  
  //检查是否为升序排列
  for(unsigned char Pos = 0; Pos < Size; Pos++){
    if(CustomLut[Pos + 1].Source < CustomLut[Pos].Source)
      return -1;//源数据错误
    if(CustomLut[Pos + 1].Destination < CustomLut[Pos].Destination)
      return -1;//目标数据错误
  }
  return 0;//数据校验正确
}  

//-----------------------初始化函数-------------------------------
void CustomLut_Init(signed char Inited)//形参为已初始化标志
{
  //memset(&CustomLut,0,sizeof(struct _CustomLut)); 无Info类数据注解提   
  if(!Inited){//未实始化时初始化
    CustomLut_Restore();
  }
  else{
    Eeprom_Rd(CustomLut_GetInfoBase(),
              &CustomLut,
              sizeof(CustomLut));
    if(CustomLut_Check()) 
      CustomLut_Restore();//检查查找表错误时,恢复默认设置
  }
}  

//-----------------------得到查找表大小函数---------------------------
unsigned char CustomLut_GetSize(void)
{
  //轮循查找表,若发现Source为NOLINEAR_CONVERT_NULL时,则认为查找表结束
  unsigned char Pos = 0;
  for(; Pos < CUSTOM_LUT_LUT_SIZE; Pos++){
    if(CustomLut[Pos].Source == NOLINEAR_CONVERT_NULL) break;
  }
  return Pos;
}  

//-----------------------是否为同一数据------------------------------
//非0在范围，0还在范围
signed char CustomLut_IsSame(NolinearConvert_t Center,//中心数据
                             NolinearConvert_t Cur)   //当前数据
{
  if(Cur >= Center){
    //比当前位置值大但在范围
    if((Cur - Center) < CustomLut_cbGetNearSorceDiff()) 
      return 1;
  }
  else{//比当前位置值小但在范围
    if((Center - Cur) < CustomLut_cbGetNearSorceDiff()) 
      return 1;//在范围
  }
  return 0;//不在范围
}

//-----------------------由数据源查找对应项函数------------------------------
//在误差范围内时，认为找到了，返回位置，负未找到
signed char CustomLut_FindItem(NolinearConvert_t Source)//源数据
{
  unsigned char Size = CustomLut_GetSize();
	
  unsigned char Pos = 0;
  NolinearConvert_t PrvSource;
  for(; Pos < Size; Pos++){
    if(CustomLut_IsSame(CustomLut[Pos].Source, Source)) return Pos;//找到了
  }
  return -1;//没有接近的
}

//-----------------------尝试覆盖一个查找表项函数-------------------------
//返回是否覆盖成功(0成功,其它不成功)
signed char CustomLut_ReplaceItem(NolinearConvert_t Source,//源数据
                                 NolinearConvert_t Destination)//目标数据
{
  unsigned char Size = CustomLut_GetSize();
  if(Size >= CUSTOM_LUT_LUT_SIZE) return -1; //已满,禁止插入
	if(CustomLut_cbIsDisModify(Destination)) return -1;//禁止修改时,禁止插入
  //1.先查找源数据是否在覆盖范围
	signed char Pos = CustomLut_FindItem(Source);
	if(Pos < 0) return Pos;//未找到
	
	
  //2.强制将此记录覆盖，再校验覆盖后的数据是否通过
	NolinearConvert_t PrvSource = CustomLut[Pos].Source;
  CustomLut[Pos].Source = Source;
  NolinearConvert_t PrvDestination = CustomLut[Pos].Destination;
  CustomLut[Pos].Destination = Destination;
  if(!CustomLut_Check()){//覆盖成功,保存数据
    Eeprom_Wr(CustomLut_GetInfoBase(),
              &CustomLut,
              sizeof(CustomLut)); 
     return 0;
  }   
  
  //3.若校验不对，则说明标数据不满足要求,则还原之前的数据
  CustomLut[Pos].Source = PrvSource;
  CustomLut[Pos].Destination = PrvDestination;
  return 1;//可插入，但输入数据不对
}  

//-----------------------增加一个查找表项函数-------------------------
//返回是否插入成功
signed char CustomLut_AddItem(NolinearConvert_t Source,//源数据
                              NolinearConvert_t Destination)//目标数据
{
  unsigned char Size = CustomLut_GetSize();
  if(Size >= CUSTOM_LUT_LUT_SIZE) return -1; //已满,禁止插入

  //1.找到插入点
  unsigned char Insert = 0;
  for(; Insert < Size; Insert++){
    if(Source <= CustomLut[Insert].Source) break;
  }
  //2.根据前后数据检查目标数据是否正确
  if(Insert == 0){//插入在首个时
    if(Destination >= CustomLut[0].Destination) 
    return -1;//比首个数据要大(后比较为当前)
  }
  else if(Insert >= Size){//插入在首个末端时
    if(Destination < CustomLut[Insert - 1].Destination) 
      return -1;//比首个最末数据要小
    goto _EndSave;//正确时直接插入末端
  }
  else{//在中间时
    if(Destination <= CustomLut[Insert - 1].Destination)
      return -1;  //比前1个数据还要小
    if(Destination >= CustomLut[Insert].Destination)
      return -1;  //比后1个数据还要大(后比较为当前)
  }
  //3.将后续有效数据往后移动(须重最后开始移动防止覆盖)
  for(unsigned char Pos = Size; Pos > Insert; Pos--){
    CustomLut[Pos].Source = CustomLut[Pos - 1].Source;
    CustomLut[Pos].Destination = CustomLut[Pos - 1].Destination;
  }
  //4.插入表中并保存数据
  _EndSave:
    CustomLut[Insert].Source = Source;
    CustomLut[Insert].Destination = Destination;
    Size++;//扩容了
    if(Size < CUSTOM_LUT_LUT_SIZE)//未满时，写结束字符
       CustomLut[Size].Source = NOLINEAR_CONVERT_NULL;
    Eeprom_Wr(CustomLut_GetInfoBase(),
              &CustomLut,
              sizeof(CustomLut)); 

  return 0;//插入成功            
}  

//-----------------------插入一个查找表项函数-------------------------
void CustomLut_InsertItem(NolinearConvert_t Source,//源数据
                          NolinearConvert_t Destination)//目标数据
{
  //检查个性查找表里是否有相同项,无时才可以覆盖
  if(CustomLut_ReplaceItem(Source, Destination))
    CustomLut_AddItem(Source, Destination);//否则直接插入
}

//-----------------------删除一个查找表项函数-------------------------
//返回是否成功
signed char CustomLut_DelItem(unsigned char Item)//项位置,最高位为0x80时强制删除
{
  unsigned char Size = CustomLut_GetSize();
  if(Size <= 2) return -1; //2点构成一条线,不能再删除了

  if(Item >= Size) return -1;//不在表内	
	if(Item & 0x80) Item &=~0x80;//强制删除时不检查可写	
	else{//检查,禁止修改时,禁止插入
		if(CustomLut_cbIsDisModify(CustomLut[Item].Destination))
			return -1;
  }

  Size -= 1;//表示最末数据索引
  //将后续有效数据往前移动
  for(; Item < Size; Item++){
    CustomLut[Item].Source = CustomLut[Item + 1].Source;
    CustomLut[Item].Destination = CustomLut[Item + 1].Destination;
  }
  //将最后数据置结束标志
  CustomLut[Item].Source = NOLINEAR_CONVERT_NULL;

  //保存数据
  Eeprom_Wr(CustomLut_GetInfoBase(),
            &CustomLut,
            sizeof(CustomLut));
             
  return 0;//删除成功 
}

//-----------------------查找表置默认值函数----------------------------
//将查找表置出厂前状态
void CustomLut_Restore(void)
{
  CustomLut_cbFullDefaultLut();
  Eeprom_Wr(CustomLut_GetInfoBase(),
            &CustomLut,
            sizeof(CustomLut));
}

