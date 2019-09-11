/*******************************************************************************
 *                      Info信息分配-示例文件   
                                   将此文件复制到各项目私有目录中并重命名为Info.h后修改
 *此模块仅用于确定系统存储的信息基址
 *需要存储信息的结构均需在此模块手工分配地址
********************************************************************************/
#ifndef __INFO_BASE_H 
#define __INFO_BASE_H 

//---------------------------得到成员偏移函数-------------------------------
#define struct_offset(type, member) \
	((unsigned int)(&(((type *)0)->member))) 

/**********************************************************************
                       各模块存储分配
**********************************************************************/

//UvSensor模块
struct _UvSensorInfoIni{
  unsigned char Reserved[8];//预留空间
};

//DA_Adj模块
struct _DA_AdjInfoIni{
  unsigned char Reserved[8];//预留空间
};

//ModbusRtuMng模块
struct _ModbusRtuMngInfoIni{
  unsigned char Reserved[8];//预留空间
};

/**********************************************************************
                        信息区结构定义
**********************************************************************/
//这里信息区最多占用64Byte空间大小
typedef struct _InfoBase{
  #if INFO_BASE_RESERVED != 0
  unsigned char Reserved[INFO_BASE_RESERVED]; //保留空间
  #endif
   //0址固定为初始化标志,为0x5a时表示已初始化,其它时为未使用
  unsigned char InitedFlag;
  struct _UvSensorInfoIni UvSensorInfoIni;
  struct _DA_AdjInfoIni DA_AdjInfoIni;
  struct _ModbusRtuMngInfoIni ModbusRtuMngInfoIni;
  
}InfoBase_t;

/**********************************************************************
                        相关基址操作宏
**********************************************************************/
//InfoBase初始化标志
#define InfoBase_GetInitedFlagBase() struct_offset(InfoBase_t,InitedFlag)
//UvSensor模块
#define UvSensor_GetInfoBase()  struct_offset(InfoBase_t, UvSensorInfoIni)
//DA_Adj模块
#define DA_Adj_GetInfoBase()  struct_offset(InfoBase_t, DA_AdjInfoIni)
//ModbusRtuMng模块
#define ModbusRtuMng_GetInfoBase()  struct_offset(InfoBase_t, ModbusRtuMngInfoIni)

#endif //#ifndef __INFO_BASE_H 

