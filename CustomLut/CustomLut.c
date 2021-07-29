/***********************************************************************

                    用户可定制查找表模块实现
此模块独立与系统,即与应用无关
注:仅支持单调上升的查找表结构
***********************************************************************/
#include "CustomLut.h"

/***********************************************************************
                     相关函数实现
***********************************************************************/

//-----------------------查找表校验函数----------------------------
//检查查找表是否有错误,无误时返回0,否则返回出错位置
signed char CustomLut_Check(struct _CustomLut *pCustomLut)
{
  unsigned char Size = CustomLut_GetSize(pCustomLut);
  if(Size < 2) return -1;//两个点才能构成一条直线
  Size--;
  
  //检查是否为升序排列
  for(unsigned char Pos = 0; Pos < Size; Pos++){
    if(pCustomLut->Info.Tbl[Pos + 1].Source < pCustomLut->Info.Tbl[Pos].Source)
      return -1;//源数据错误
    if(pCustomLut->Info.Tbl[Pos + 1].Destination < pCustomLut->Info.Tbl[Pos].Destination)
      return -1;//目标数据错误
  }
  return 0;//数据校验正确
}  

//--------------------------------初始化函数------------------------------------
void CustomLut_Init(struct _CustomLut *pCustomLut,
                    unsigned char CustomLutId,//此表的ID号，用于对外区分结构
                    signed char Inited)//形参为已初始化标志
{
  //memset(&pCustomLut,0,sizeof(struct _CustomLut)); //无Info类数据注解掉
  pCustomLut->CustomLutId = CustomLutId;
  if(!Inited){//未实始化时初始化
    CustomLut_Restore(pCustomLut);
  }
  else{
    CustomLut_cbPosSave(pCustomLut);
    if(CustomLut_Check(pCustomLut)) 
      CustomLut_Restore(pCustomLut);//检查查找表错误时,恢复默认设置
  }
}  

//-----------------------得到查找表大小函数---------------------------
unsigned char CustomLut_GetSize(struct _CustomLut *pCustomLut)
{
  //轮循查找表,若发现Source为NOLINEAR_CONVERT_NULL时,则认为查找表结束
  unsigned char Pos = 0;
  for(; Pos < CUSTOM_LUT_LUT_SIZE; Pos++){
    if(pCustomLut->Info.Tbl[Pos].Source == NOLINEAR_CONVERT_NULL) break;
  }
  return Pos;
}  

//-----------------------尝试覆盖一个查找表项函数-------------------------
//返回是否覆盖成功(0成功,其它不成功)
signed char CustomLut_ReplaceItem(struct _CustomLut *pCustomLut,
                                   NolinearConvert_t Source,//源数据
                                   NolinearConvert_t Destination)//目标数据
{
  unsigned char Size = CustomLut_GetSize(pCustomLut);
  if(Size >= CUSTOM_LUT_LUT_SIZE) return -1; //已满,禁止插入
  //1.先查找源数据是否在覆盖范围
  unsigned char Pos = 0;
  NolinearConvert_t PrvSource;
  for(; Pos < Size; Pos++){
    PrvSource = pCustomLut->Info.Tbl[Pos].Source;
    if(Source > PrvSource){
      //比当前位置值大但在范围
      if((Source - PrvSource) < CustomLut_cbGetNearSorceDiff(pCustomLut->CustomLutId)) break; 
    }
    else{
      //比当前位置值小但在范围
      if((PrvSource - Source) < CustomLut_cbGetNearSorceDiff(pCustomLut->CustomLutId)) break;
    } 
  }
  if(Pos >= Size) return -1;//没有接近的
  
  //2.强制将此记录覆盖，再校验覆盖后的数据是否通过
  pCustomLut->Info.Tbl[Pos].Source = Source;
  NolinearConvert_t PrvDestination = pCustomLut->Info.Tbl[Pos].Destination;
  pCustomLut->Info.Tbl[Pos].Destination = Destination;
  if(!CustomLut_Check(pCustomLut)){//覆盖成功,保存数据
     CustomLut_cbPushSave(pCustomLut);
     return 0;
  }   
  
  //3.若校验不对，则说明标数据不满足要求,则还原之前的数据
  pCustomLut->Info.Tbl[Pos].Source = PrvSource;
  pCustomLut->Info.Tbl[Pos].Destination = PrvDestination;
  return 1;//可插入，但输入数据不对
}  

//-----------------------增加一个查找表项函数-------------------------
//返回是否插入成功
signed char CustomLut_AddItem(struct _CustomLut *pCustomLut,
                              NolinearConvert_t Source,//源数据
                              NolinearConvert_t Destination)//目标数据
{
  unsigned char Size = CustomLut_GetSize(pCustomLut);
  if(Size >= CUSTOM_LUT_LUT_SIZE) return -1; //已满,禁止插入

  //1.找到插入点
  unsigned char Insert = 0;
  for(; Insert < Size; Insert++){
    if(Source <= pCustomLut->Info.Tbl[Insert].Source) break;
  }
  //2.根据前后数据检查目标数据是否正确
  if(Insert == 0){//插入在首个时
    if(Destination >= pCustomLut->Info.Tbl[0].Destination) 
    return -1;//比首个数据要大(后比较为当前)
  }
  else if(Insert >= Size){//插入在首个末端时
    if(Destination < pCustomLut->Info.Tbl[Insert - 1].Destination) 
      return -1;//比首个最末数据要小
    goto _EndSave;//正确时直接插入末端
  }
  else{//在中间时
    if(Destination <= pCustomLut->Info.Tbl[Insert - 1].Destination)
      return -1;  //比前1个数据还要小
    if(Destination >= pCustomLut->Info.Tbl[Insert].Destination)
      return -1;  //比后1个数据还要大(后比较为当前)
  }
  //3.将后续有效数据往后移动(须重最后开始移动防止覆盖)
  for(unsigned char Pos = Size; Pos > Insert; Pos--){
    pCustomLut->Info.Tbl[Pos].Source = pCustomLut->Info.Tbl[Pos - 1].Source;
    pCustomLut->Info.Tbl[Pos].Destination = pCustomLut->Info.Tbl[Pos - 1].Destination;
  }
  //4.插入表中并保存数据
  _EndSave:
    pCustomLut->Info.Tbl[Insert].Source = Source;
    pCustomLut->Info.Tbl[Insert].Destination = Destination;
    Size++;//扩容了
    if(Size < CUSTOM_LUT_LUT_SIZE)//未满时，写结束字符
       pCustomLut->Info.Tbl[Size].Source = NOLINEAR_CONVERT_NULL;
    CustomLut_cbPushSave(pCustomLut);

  return 0;//插入成功            
}  

//-----------------------删除一个查找表项函数-------------------------
//返回是否成功
signed char CustomLut_DelItem(struct _CustomLut *pCustomLut,
                              unsigned char Item)//项位置
{
  unsigned char Size = CustomLut_GetSize(pCustomLut);
  if(Size <= 2) return -1; //2点构成一条线,不能再删除了
  if(Item >= Size) return -1;//不在表内
  Size -= 1;//表示最末数据索引
  //将后续有效数据往前移动
  for(; Item < Size; Item++){
    pCustomLut->Info.Tbl[Item].Source = pCustomLut->Info.Tbl[Item + 1].Source;
    pCustomLut->Info.Tbl[Item].Destination = pCustomLut->Info.Tbl[Item + 1].Destination;
  }
  //将最后数据置结束标志
  pCustomLut->Info.Tbl[Item].Source = NOLINEAR_CONVERT_NULL;

  //保存数据
  CustomLut_cbPushSave(pCustomLut);
             
  return 0;//删除成功 
}

//-----------------------查找表置默认值函数----------------------------
//将查找表置出厂前状态
void CustomLut_Restore(struct _CustomLut *pCustomLut)
{
  CustomLut_cbFullDefaultLut(pCustomLut->Info.Tbl);
  CustomLut_cbPushSave(pCustomLut);
}

