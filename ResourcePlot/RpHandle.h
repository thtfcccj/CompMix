/******************************************************************************

                     Rp句柄Handle定义与说明

******************************************************************************/
#ifndef __RP_HANDLE_H 
#define __RP_HANDLE_H 

//--------------------------------Handle定义----------------------------------
//Handle作为唯一的ID号与应用层交互，按位分为以下几部分：

//实时刷新调用标志,不需要实时部分,在可取图片与字符资源时，可返回异常以停止刷新
#define RP_HANDLE_REAL_REFRESH 0x80000000

//主类，固定分配用于大类识别
#define RP_HANDLE_MTYPE_MASK   0x7f000000
//子类，在RP_AryArea回调获得(某类设备)
#define RP_HANDLE_STYPE_MASK   0x00ff0000

#define RP_HANDLE_TYPE_MASK   (RP_HANDLE_MTYPE_MASK | RP_HANDLE_STYPE_MASK)

//阵列ID号(某个设备)
#define RP_HANDLE_ARY_MASK     0x00ffffff   



#endif  //#ifndef __RP_HANDLE_H 



