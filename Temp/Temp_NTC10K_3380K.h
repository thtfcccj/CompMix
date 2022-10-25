/*******************************************************************************

     温度模块硬件相关定义-使用外部10KNTC, 3380K(上接3.3V，下拉1K%电阻)参数定义

*******************************************************************************/
#ifndef __TEMP_NTC10K_3380K_H
#define __TEMP_NTC10K_3380K_H
#ifdef SUPPORT_EX_PREINCLUDE//不支持Preinlude時
  #include "Preinclude.h"
#endif
/*******************************************************************************
                            相关计算 
*******************************************************************************/
//----------------------NTC10K_3380K内部温度传感器计算说明------------------------------
  //根据资料
  //https://item.szlcsc.com/735777.html
  //http://www.thermistors.cn/news/294.html
  //  以25度>10k, 0度27K, 60度3K计算, 3.3V下拉1K%电阻， AD参考3.3V， 12位AD计算
  //公式： 温度 = (当前AD - 零点AD) * 增益：
  //25度  (64 + 25) =  (2048 - Z) * G
  //0度   (64 + 0 ) =  (1107 - Z) * G
  //计算得到 G=871>>15, Z=-1300,以60度3K验证为：AD3150->(118 - 64) = 54度
  //建议增加温线校正曲线

/*******************************************************************************
                            硬件相关定义 
*******************************************************************************/
//根据硬件特性，支持多种不同的滤波方式:

//支持温补曲线时开启(此应用里可开始)
#define SUPPORT_TEMP_LUT

//支持滤波时,同时定义缓冲个数, <255
//#define SUPPORT_TEMP_FILETER  10 

//支持简单平均
//#define SUPPORT_TEMP_FILETER_AVERVGE   

//支持平均并去除最大最小值，同时定义前后去除个数,<(SUPPORT_TEMP_FILETER / 3)
//#define SUPPORT_TEMP_FILETER_AVERVGE_MID    3

//增益使用的Q值
#define TEMP_GAIN_Q  15

//默认温度零点AD值
#ifndef TEMP_DEFAULT_ZERO
  #define TEMP_DEFAULT_ZERO   -499  //3.3V 10K分压，3.3V参考AD时试验结果
#endif  
                                     
//默认温度增益值 Q值(样品试验结果)
#ifndef TEMP_DEFAULT_GAIN
  #define TEMP_DEFAULT_GAIN   1089  //3.3V 10K分压，3.3V参考AD时试验结果
#endif

//温差标定时，最小温差值
#ifndef TEMP_DIFF_TEPM_MIN
  #define TEMP_DIFF_TEPM_MIN   10 
#endif 

//温差标定时，最小信号值
#ifndef TEMP_DIFF_SIGAL_MIN
  #define TEMP_DIFF_SIGAL_MIN  100   
#endif

#endif






