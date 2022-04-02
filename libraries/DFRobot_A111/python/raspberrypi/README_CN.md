# DFRobot_A111
* [English Version](./README.md)

毫米波雷达测距传感器采用A111雷达芯片。
A111 60GHZ雷达系统是一种基于脉冲相干雷达(PCR)技术的雷达系统,能以超低功耗实现毫米级精度。
60GHz非许可的ISM频带具有高稳健性, 不受例如噪声、灰尘、颜色、直射或散射光线等任何自然干扰源的影响, 易于集成且不需要孔眼。
A111把脉冲雷达低功耗的优势与高精度的相干雷达相结合, 并采用AiP封装技术把包括天线在内的所有组件集成于一颗面积仅29mm²的芯片, 以此成为超低功耗雷达的全球领先产品。
A111可检测紧密范围内的多个对象, 支持单次测量以及连续扫描, 扫描频率最高达1500 Hz。
此外, A111特性可支持材料识别和运动检测, 因而适用于尖端检测应用。

![产品实物图](../../resources/images/A111.png)


## 产品链接 (链接到英文商城)
    SKU：SEN0489


## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)


## 概述

这个库用使用RTU通信协议, 依赖于modbus-tk库, 安装运行此库必须安装他的依赖库。
这个库能够使用雷达A111, 可以获取雷达模块的基本信息、测量参数、测量数据, 也可以修改其基本信息和测量参数。


## 库安装

首先本库使用RTU通信协议, 需要使用树莓派的串口, 关于串口相关配置参照产品wiki

本库使用到了modbus_tk, 使用本库前先检测树莓派是否成功导入modbus_tk, 若导入失败, 请通过以下命令安装modbus_tk库
python2: pip install modbus_tk
python3: pip3 install modbus_tk

使用库, 首先下载库文件, 将其粘贴到指定的目录中, 然后打开Examples文件夹并在该文件夹中运行演示。


## 方法

```python

    '''!
      @brief Initialize sensor
      @return  返回初始化状态
      @retval True indicate initialization succeed
      @retval False indicate failed
    '''
    def begin(self):

    '''!
      @brief 读取设备基本信息
      @return list: 
      @retval  第一个元素为：模块的PID
      @retval  第二个元素为：模块的VID
      @retval  第三个元素为：模块的通信地址
      @retval  第四个元素为：模块的波特率
      @retval  第五个元素为：模块校验位和停止位
      @retval  第六个元素为：固件版本号
    '''
    def refreshBasicInfo(self):

    '''!
      @brief 读取模块测量数据
      @return list: 
      @retval  第一个元素为：当前检测到的目标数量
      @retval  第二个元素为：测量的第一个目标的距离；第三个元素为：测量的第一个目标的强度
      @retval  第四个元素为：测量的第二个目标的距离；第五个元素为：测量的第二个目标的强度
      @retval  第六个元素为：测量的第三个目标的距离；第七个元素为：测量的第三个目标的强度
      @retval  第八个元素为：测量的第四个目标的距离；第九个元素为：测量的第四个目标的强度
      @retval  第十个元素为：测量的第五个目标的距离；第十一个元素为：测量的第五个目标的强度
    '''
    def refreshMeasurementData(self):

    '''!
      @brief 读取模块当前配置的测量参数
      @return list: 
      @retval  第一个元素为：当前测量起始位置设置值
      @retval  第二个元素为：当前测量结束位置设置值
      @retval  第三个元素为：当前起始阈值设置值
      @retval  第四个元素为：当前结束阈值设置值
      @retval  第五个元素为：当前模块灵敏度设置值
      @retval  第六个元素为：当前比较偏移值设置值
    '''
    def refreshMeasurementConfig(self):

    '''!
      @brief 设置模块的通信地址
      @param addr 要设置的设备地址, (1~247即0x0001~0x00F7)
    '''
    def setADDR(self, addr):

    '''!
      @brief 设置模块的波特率, 断电保存, 重启后生效
      @param mode 要设置的波特率：
      @n     E_BAUDRATE_2400---2400, E_BAUDRATE_4800---4800, E_BAUDRATE_9600---9600, 
      @n     E_BAUDRATE_14400---14400, E_BAUDRATE_19200---19200, E_BAUDRATE_38400---38400, 
      @n     E_BAUDRATE_57600---57600, E_BAUDRATE_115200---115200
    '''
    def setBaudrateMode(self, mode):

    '''!
      @brief 设置模块的校验位和停止位
      @param mode 要设置的模式, 下列模式经过或运算后得到mode:
      @n     校验位：
      @n          E_CHECKBIT_NONE
      @n          E_CHECKBIT_EVEN
      @n          E_CHECKBIT_ODD
      @n     停止位：
      @n          E_STOPBIT_1
      @n          E_STOPBIT_2
    '''
    def setCheckbitStopbit(self, mode):

    '''!
      @brief 配置测量起始位置值, 配置测量结束位置值, 
      @n     配置起始阈值, 配置结束阈值, 
      @n     配置模块灵敏度, 配置比较偏移值
      @param starting_position 起始位置值,0x0046~0x19C8
      @param stop_position 结束位置值,0x0046~0x19C8
      @param initial_threshold 起始阈值,0x0064~0x2710
      @param end_threshold 结束阈值,0x0064~0x2710
      @param module_sensitivity 模块灵敏度,0x0000~0x0004
      @param comparison_offset 比较偏移值,0x0000~0xFFFF
    '''
    def setAllMeasurementParameters(self, startingPosition, stopPosition, 
                                        initialThreshold, endThreshold,
                                        moduleSensitivity, comparisonOffset):

   '''!
     @brief 恢复出厂设置
   '''
    def restoreFactorySetting(self):

```


## 兼容性

* RaspberryPi 版本

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python 版本

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## 历史

- 2021/07/08 - 1.0.0 版本


## 创作者

Written by qsjhyy(yihuan.huang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))
