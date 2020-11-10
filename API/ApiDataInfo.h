/***********************************************************************

                  单例/多例模式对像/结构统一访问接口-数据信息定义

***********************************************************************/
#ifndef __API_DATA_INFO_H
#define	__API_DATA_INFO_H

/***********************************************************************
                         数据类型相关定义
***********************************************************************/
//DI = DATA+INFO
//---------------------------总体定义---------------------------------
//总体定义:
#define API_DI_TYPE_SHIRT     6  //大类，见定义
#define API_DI_TYPE_MASK      (3 << API_DI_TYPE_SHIRT)

#define API_DI_TYPE_OTHER_MASK   0x3C //由大类决定
//---------------------------大类定义---------------------------------
//长度1Byte型数据(或结构体数据),其它见Byte型数据时定义
#define API_DI_TYPE_B1        (0 << API_DI_TYPE_SHIRT)   
//长度2Byte型数据,其它见Byte型数据时定义
#define API_DI_TYPE_B2        (1 << API_DI_TYPE_SHIRT)   
//长度4Byte型数据,其它见Byte型数据时定义
#define API_DI_TYPE_B4        (2 << API_DI_TYPE_SHIRT)    
//其它类型数据
#define API_DI_TYPE_OTHER     (3 << API_DI_TYPE_SHIRT)    

//---------------------------1/2/4Byte时大类定义---------------------------------
#define API_DI_BX_MASK        0x30  //掩码
//#define API_DI_XXXX         0x30  //保留
#define API_DI_STRUCT         0x20  //结构体标识
#define API_DI_SIGN           0x10  //带符号型数据
#define API_DI_UNSIGN         0x00  //无符号型数据

//====================带符号型或无符号型数据子定义================
#define API_DI_DOT            0x08  //小数点标识，否则无小数点
#define API_DI_DOT_POS_MASK   0x07  //小数点时表示小数点位置
#define API_DI_LEN_MASK       0x07  //无小数点时为数据显示长度,0表示可变长度(前面0不显示) 

//========================结构体时定义==========================
//结构体子长度掩码
#define API_DI_STRUCT_LEN_MASK   0x0f 

//结构体用户定义:1Byte表示 0-15, 2Byte表示16-31, 3Byte表示32-47,结合定义为:
#define API_DI_STRUCT_BASE0   (API_DI_STRUCT | (0 << API_DI_TYPE_SHIRT))
#define API_DI_STRUCT_BASE16  (API_DI_STRUCT | (1 << API_DI_TYPE_SHIRT))
#define API_DI_STRUCT_BASE32  (API_DI_STRUCT | (2 << API_DI_TYPE_SHIRT))
//结构体定义示例:
//<16Byte时:    API_DI_STRUCT_BASE0 | sizeof(struct _???)
//16-31Byte时:  API_DI_STRUCT_BASE16 | sizeof(struct _???)
//32-47Byte时:  API_DI_STRUCT_BASE32 | sizeof(struct _???)
//可用下述函数计算并定义(structLen必须<48Byte):
#define ApiId_StructLen(structLen) \
  (API_DI_STRUCT | (((structLen) & 0x30) << 2) | ((structLen) & 0x0f))

//---------------------------其它类时大类定义---------------------------------
#define API_DI_OTHER_MASK    0x20   //区分大类
#define API_DI_OTHER_STD     0x20   //标准类固定型，否则为用户定义数据类型
#define API_DI_OTHER_USER    0x00   //用户定义数据类型标志(API_DI_OTHER_STD没置位)
    
//========================标准类固定型数据定义========================
//标准数据下述可直接使用:
#define API_DI_STD_MASK     (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x1f)
//字符串型,长度由结束字符决定
#define API_DI_STD_STR      (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x00)   
//单浮点数型,点用4Byte
#define API_DI_STD_SF       (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x01) 
//双浮点数型,点用8Byte
#define API_DI_STD_DF       (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x02)
//3个独立Byte型
#define API_DI_STD_B3       (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x03)
//以下可继续扩展:
    
//标准类型结束位置(不含):     
#define API_DI_STD_END      (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x04)
//没定义(暂不提供支持,254)
#define API_DI_UNDEF        (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x1E)
//不能识别的类型(即255)
#define API_DI_UNUSD        (API_DI_TYPE_OTHER | API_DI_OTHER_STD | 0x1F)

//========================用户数据类型时========================
//区分下述两种类型:
#define API_DI_OTHER_USER_MASK  (API_DI_TYPE_OTHER | API_DI_OTHER_USER | 0x10)   
    
//DAP2324格式的小数点与显示掩码前缀(直接使用)，占用0x0f,长度为2
#define API_DI_DOT_DISP     (API_DI_TYPE_OTHER | API_DI_OTHER_USER | 0) 
//其它用户数据类型前缀(直接使用)
#define API_DI_USER_OTHER   (API_DI_TYPE_OTHER | API_DI_OTHER_USER | 0x10) 
 
/***********************************************************************
                         写数据时，返回结果定义
***********************************************************************/
#define API_DI_WR_MASK     7  //最大定义长度

#define API_DI_WR_WAIT    0  //等待结果(中转是为此状态)
#define API_DI_WR_FINAL   1  //写成功
#define API_DI_WR_ERR_IN  2  //输入信息不全
#define API_DI_WR_ERR_OV  3  //输入数据不符合规则   
#define API_DI_WR_ARY_OV  6  //阵列ID超限
#define API_DI_WR_UNDDEF  7  //未定义

/***********************************************************************
                         执行命令时, 返回结果定义
***********************************************************************/
#define API_DI_CMD_MASK    7  //最大定义长度

#define API_DI_CMD_WAIT    0  //等待结果(中转是为此状态)
#define API_DI_CMD_FINAL   1  //成功
#define API_DI_CMD_ERR_IN  2  //输入信息不全
#define API_DI_CMD_ERR_STATE  6  //状态不满足
#define API_DI_CMD_UNDDEF  7  //未定义

//Api带参数指令参数缓冲,主要用于通讯需要，但设备层不需要此参数的应用场合
struct _ApiCmdParaBuf{
  unsigned short DevId;  //指定设备
  unsigned char FunId;   //功能码
  unsigned char Para[3]; //缓冲的参灵敏
};   
    
/***********************************************************************
                             相关函数
***********************************************************************/

//-------------------------由信息获得数据占位------------------------------
//0表示为字符串需进行字符判断,异常或未定义时返回1
unsigned char ApiDataInfo_GetLen(unsigned char ApiDataInfo);



#endif //#define	__API_DATA_INFO_H

