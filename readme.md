
## 组件层-其它相关模块接口及其实现，含
  ### API:  为抽化程序结构提供的，可用于通讯的应用程序接口及相关定义(含示例)
  
  ### DefStdType:  U8 S8等定义
  
  ### Delay 延时函数标准接口，及在各种硬件中的实现。

  ### InfoBase EEPROM空间分配模块，含大容量EEPROM空间动态分配时的辅助函数

  ### math math.h数学库扩展函数集

  ### MemMng 简单的内存分配模块(只分配不释放)

  ### Restore 重要参数区的备份模块

  ### SoftQ 软件实现的FIFO队列

  ### SysTick 各种硬件下的Tick 时钟实现 (ms us二选一)

  ### SysTimer 时间段计时器(依赖于SysTick)

  ### Test 测试用倒计时器

  ### ZipTime 将RTC值压给到unsigned long型中的压缩时间格式编解码， 及其显示等(提供压缩实时时间)。






