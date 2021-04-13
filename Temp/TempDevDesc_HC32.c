/*******************************************************************************

      温度设备模块Desc实例，在H32内部温度传感器中的实现

*******************************************************************************/
#include "TempDev.h"
#include <string.h>

//----------------------HC32内部温度传感器计算说明------------------------------
  //根据资料，计算公式为：环境温度 = 25 + 0.0803 × Vref × ( AdcValue – Trim)
  //这里偏移为SEN_TEMP_ZERO即274, 则:
  //环境偏移温度 = 25 + 0.0803 × Vref × ( AdcValue – Trim) + 274
  //             ->299 + 0.0803 × Vref × ( AdcValue – Trim)       
  //这里固定使用内部 2.5V参考则存主位置为：0x00100C36  ±3℃精度
  //将上述转换为这里的计算公式 环境温度 = (AdcValue - ZERO) * GAIN，
  //需进行体系转换, 结论推导:
  //  ->                    299 + (0.0803 × 2.5) × ( AdcValue – Trim)
  //  ->                    299 + 0.20075 × ( AdcValue – Trim)
  //  -> (0.20075 * 1489.415) + 0.20075 × (AdcValue – Trim)
  //  ->  0.20075 X ((AdcValue – Trim) + 1489.415)
  //  ->  0.20075 X (AdcValue - (Trim - 1489.415))  故:
  //默认温度零点AD值   ZERO = (Trim - 1489.415),取整=(Trim - 1489.415)假定不为负:
  #define _DEFAULT_ADJ_ZERO ((*(unsigned short*)(0x00100C36) - 1489))  
  //默认温度AD值 Q值:  GAIN = 0.20075 *  (1 << 12) = 822.3
  #define _DEFAULT_ADJ_GAIN   8223  //再扩大10倍即为0.1度为单位

//----------------------恢复出厂设置实现------------------------------
static void _InfoToDefault(struct _TempDev *pDev)
{
  pDev->Info.Zero = _DEFAULT_ADJ_ZERO;
  pDev->Info.Gain = _DEFAULT_ADJ_GAIN;  
}

//---------------------------Desc结构实现------------------------------
//(暂无.h，外部需extern实现)
const struct _TempDevDesc TempDevDesc_HC32 = {
  12,  //增益使用的Q值
  0,   //曲线补偿表容量,这里不需要曲线补偿
  40,  //温度不稳定时滤波缓冲个数,0时不滤波
  (TEMP_DEV_FILETER_AVERVGE_MID << 5) | 10,//滤波类型与参数->使用前后去极值方案
  _InfoToDefault,//将设备Info恢复出厂设置
  NULL, //曲线补偿表阵列, NULL无此功能
  NULL, //附加校正,
  NULL, //反附加校正，标定用
  200, //标定时，允许的最小温差值
  20,  //标定时，允许的最小信号量,这里默认0.20075 *  200=40,
};



