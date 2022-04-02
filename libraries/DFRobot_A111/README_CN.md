# DFRobot_A111
* [English Version](./README.md)

毫米波雷达测距传感器采用A111雷达芯片。
A111 60GHZ雷达系统是一种基于脉冲相干雷达(PCR)技术的雷达系统,能以超低功耗实现毫米级精度。
60GHz非许可的ISM频带具有高稳健性，不受例如噪声、灰尘、颜色、直射或散射光线等任何自然干扰源的影响，易于集成且不需要孔眼。
A111把脉冲雷达低功耗的优势与高精度的相干雷达相结合，并采用AiP封装技术把包括天线在内的所有组件集成于一颗面积仅29mm²的芯片，以此成为超低功耗雷达的全球领先产品。
A111可检测紧密范围内的多个对象，支持单次测量以及连续扫描，扫描频率最高达1500 Hz。
此外，A111特性可支持材料识别和运动检测，因而适用于尖端检测应用。

![产品实物图](./resources/images/A111.png)


## 产品链接 (链接到英文商城)
    SKU: SEN0489


## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)


## 概述

这个库用使用RTU通信协议, 依赖于DFRobot_RTU这个库, 安装运行此库必须安装他的依赖库。
这个库能够使用雷达A111, 可以获取雷达模块的基本信息、测量参数、测量数据, 也可以修改其基本信息和测量参数。


## 库安装

要使用这个库, 首先下载库文件( https://github.com/DFRobot/DFRobot_A111 )和依赖文件( https://github.com/DFRobot/DFRobot_RTU ), 将其粘贴到\Arduino\libraries目录中, 然后打开示例文件夹并在文件夹中运行演示。


## 方法

```C++

  /**
   * @fn begin
   * @brief 初始化函数
   * @param _serial 通信所需串口, 支持硬串口和软串口
   * @return int类型, 表示返回初始化的状态
   * @retval 0 NO_ERROR
   * @retval -1 ERR_DATA_BUS
   * @retval -2 ERR_IC_VERSION
   */
  int begin(Stream *_serial);

  /**
   * @fn refreshBasicInfo
   * @brief 重新从传感器获取其基本信息, 并缓存到存储信息的结构体basicInfo里面
   * @note basicInfo结构体的成员:
   * @n      PID: 模块的PID
   * @n      VID: 模块的VID
   * @n      modbusAddr: 模块的通信地址
   * @n      baudrate: 模块的波特率
   * @n      checkbit: 校验位
   * @n      stopbit: 停止位
   * @n      versions: 固件版本号
   * @return 返回0表示读取成功
   */
  int refreshBasicInfo(void);

  /**
   * @fn refreshMeasurementData
   * @brief 重新从传感器读取其测量数据, 并缓存到存储信息的数组dataBuf[11]里面
   * @note dataBuf[11]数组的成员:
   * @n      dataBuf[0]: 当前检测到的目标数量
   * @n      dataBuf[1]: 测量的第一个目标的距离; dataBuf[2]: 测量的第一个目标的强度
   * @n      dataBuf[3]: 测量的第二个目标的距离; dataBuf[4]: 测量的第二个目标的强度
   * @n      dataBuf[5]: 测量的第三个目标的距离; dataBuf[6]: 测量的第三个目标的强度
   * @n      dataBuf[7]: 测量的第四个目标的距离; dataBuf[8]: 测量的第四个目标的强度
   * @n      dataBuf[9]: 测量的第五个目标的距离; dataBuf[10]: 测量的第五个目标的强度
   * @return 返回0表示读取成功
   */
  int refreshMeasurementData(void);

  /**
   * @fn refreshMeasurementConfig
   * @brief 重新从传感器获取当前配置的测量参数, 并缓存到存储信息的结构体measurementConfig里面
   * @note measurementConfig结构体的成员:
   * @n      startPosition: 当前测量起始位置设置值
   * @n      stopPosition: 当前测量结束位置设置值
   * @n      initialThreshold: 当前起始阈值设置值
   * @n      endThreshold: 当前结束阈值设置值
   * @n      moduleSensitivity: 当前模块灵敏度设置值
   * @n      comparisonOffset: 当前比较偏移值设置值
   * @return 返回0表示读取成功
   */
  int refreshMeasurementConfig(void);

  /**
   * @fn setADDR
   * @brief 设置模块的通信地址
   * @param addr 要设置的设备地址, (1~247即0x0001~0x00F7)
   * @return None
   */
  void setADDR(uint16_t addr);

  /**
   * @fn setBaudrateMode
   * @brief 设置模块的波特率, 掉电重启后生效
   * @param mode 要设置的波特率:
   * @n     eBaudrate2400---2400, eBaudrate4800---4800, eBaudrate9600---9600, 
   * @n     eBaudrate14400---14400, eBaudrate19200---19200, eBaudrate38400---38400, 
   * @n     eBaudrate57600---57600, eBaudrate115200---115200, eBaudrate_1000000---1000000
   */
  void setBaudrateMode(eBaudrateMode_t mode);

  /**
   * @fn setCheckbitStopbit
   * @brief 设置模块的校验位和停止位
   * @param mode 要设置的模式, 下列模式经过或运算后得到mode:
   * @n     校验位:
   * @n           eCheckBitNone
   * @n           eCheckBitEven
   * @n           eCheckBitOdd
   * @n     停止位:
   * @n           eStopBit1
   * @n           eStopBit2
   */
  void setCheckbitStopbit(uint16_t mode);

  /**
   * @fn setAllMeasurementParameters
   * @brief 配置测量起始位置值, 配置测量结束位置值,
   * @n     配置起始阈值, 配置结束阈值,
   * @n     配置模块灵敏度, 配置比较偏移值
   * @param startingPosition 起始位置值,70~6600(0x0046~0x19C8)
   * @param stopPosition 结束位置值,70~6600(0x0046~0x19C8)
   * @param initialThreshold 起始阈值,100~10000(0x0064~0x2710)
   * @param endThreshold 结束阈值,100~10000(0x0064~0x2710)
   * @param moduleSensitivity 模块灵敏度,0x0000~0x0004
   * @param comparisonOffset 比较偏移值,-32768~32767(0~0xFFFF)
   */
  void setAllMeasurementParameters(uint16_t startingPosition, uint16_t stopPosition,
                                   uint16_t initialThreshold, uint16_t endThreshold,
                                   uint16_t moduleSensitivity, uint16_t comparisonOffset);

  /**
   * @fn restoreFactorySetting
   * @brief 恢复出厂设置
   */
  void restoreFactorySetting(void);

```


## 兼容性

* DFRobot_RTU

MCU                | SoftwareSerial | HardwareSerial |
------------------ | :------------: | :------------: |
Arduino Uno        |       √        |       X        |
Mega2560           |       √        |       √        |
Leonardo           |       √        |       √        |
ESP32              |       X        |       √        |
ESP8266            |       √        |       X        |
micro:bit          |       X        |       X        |
FireBeetle M0      |       X        |       √        |
raspberry          |       X        |       √        |

* DFRobot_A111

MCU                |  Work Well   |  Work Wrong  |  Untested   | Remarks
------------------ | :----------: | :----------: | :---------: | :---:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |


## 历史

- 2021/07/08 - 1.0.0 版本


## 创作者

Written by qsjhyy(yihuan.huang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

