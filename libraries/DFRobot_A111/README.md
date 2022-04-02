# DFRobot_A111
* [中文版](./README_CN.md)

The millimeter wave radar ranging sensor employs A111 radar chip.
The A111 60GHZ radar system is based on pulsed coherent radar (PCR) technology enabling millimeter accuracy with very low power consumption.
By operating in the 60GHz unlicensed ISM radio band, the radar sensor provides robust performance without interference from noise, dust, color nor direct or indirect light. Radio technology makes the sensor easy to integrate without the need for an aperture.
The A111 radar sensor is optimized for ultra-low power and high precision and fully integrated into a small 29mm² chip. It is a one-package solution with integrated Antenna in Package (AiP). This will make it be a leading global product with ultra-low power radar.
The A111 radar sensor can detect multiple objects at close range with single measurements as well as continuous sweeps set to any frequency up to 1500 Hz.
The unique characteristics of the A111 radar sensor make it possible to identify materials and detect motion in advanced sensor applications.

![产品实物图](./resources/images/A111.png)


## Product Link (链接到英文商城)
    SKU：SEN0489


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

The library uses the RTU communication protocol and relies on the DFRobot_RTU library. If you want to install and run the library you have to install its dependent libraries.
The library, with radar A111, can get the basic information, measure parameters and measure data of the radar module, and can also change the basic information and measure parameters.


## Installation

To use this library, first download the library file(https://github.com/DFRobot/DFRobot_A111) and dependent file(https://github.com/DFRobot/DFRobot_RTU), 
paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.


## Methods

```C++

  /**
   * @fn begin
   * @brief initialization function
   * @param _serial serial ports for communication, supporting hard and soft serial ports
   * @return int type, means returning initialization status
   * @retval 0 NO_ERROR
   * @retval -1 ERR_DATA_BUS
   * @retval -2 ERR_IC_VERSION
   */
  int begin(Stream *_serial);

  /**
   * @fn refreshBasicInfo
   * @brief retrieve the basic information from the sensor and buffer it into basicInfo, the structure that stores information
   * @note basicInfo structure members:
   * @n      PID: module PID
   * @n      VID: module VID
   * @n      modbusAddr: module communication address
   * @n      baudrate: module baud rate
   * @n      checkbit: check bit
   * @n      stopbit: stop bit
   * @n      versions: firmware revision number
   * @return returning 0 means read succeeds
   */
  int refreshBasicInfo(void);

  /**
   * @fn refreshMeasurementData
   * @brief re-read the measured data from the sensor and buffer it into the array dataBuf[11] that stores information
   * @note dataBuf[11] array members:
   * @n      dataBuf[0]: the number of objects currently detected
   * @n      dataBuf[1]: measured distance of the first object; dataBuf[2]: measured intensity of the first object
   * @n      dataBuf[3]: measured distance of the second object; dataBuf[4]: measured intensity of the second object
   * @n      dataBuf[5]: measured distance of the third object; dataBuf[6]: measured intensity of the third object
   * @n      dataBuf[7]: measured distance of the fourth object; dataBuf[8]: measured intensity of the fourth object
   * @n      dataBuf[9]: measured distance of the fifth object; dataBuf[10]: measured intensity of the fifth object
   * @return returning 0 means read succeeds
   */
  int refreshMeasurementData(void);

  /**
   * @fn refreshMeasurementConfig
   * @brief retrieve the currently configured parameters from the sensor and buffer it into the structure measurementConfig that stores information
   * @note measurementConfig structure members:
   * @n      startPosition: current set value at measurement start position
   * @n      stopPosition: current set value at measurement stop position
   * @n      initialThreshold: current initial threshold set value
   * @n      endThreshold: current end threshold set value
   * @n      moduleSensitivity: current module sensitivity set value
   * @n      comparisonOffset: current comparison offset set value
   * @return returning 0 means read succeeds
   */
  int refreshMeasurementConfig(void);

  /**
   * @fn setADDR
   * @brief set the module communication address
   * @param addr the device address to be set, (1~247即0x0001~0x00F7)
   * @return None
   */
  void setADDR(uint16_t addr);

  /**
   * @fn setBaudrateMode
   * @brief set the module baud rate, the setting takes effect after power fail and restart
   * @param mode the baud rate to be set:
   * @n     eBaudrate2400---2400, eBaudrate4800---4800, eBaudrate9600---9600, 
   * @n     eBaudrate14400---14400, eBaudrate19200---19200, eBaudrate38400---38400, 
   * @n     eBaudrate57600---57600, eBaudrate115200---115200, eBaudrate_1000000---1000000
   */
  void setBaudrateMode(eBaudrateMode_t mode);

  /**
   * @fn setCheckbitStopbit
   * @brief set check bit and stop bit of the module
   * @param mode the mode to be set, the following patterns constitute mode::
   * @n     check bit:
   * @n           eCheckBitNone
   * @n           eCheckBitEven
   * @n           eCheckBitOdd
   * @n     stop bit:
   * @n           eStopBit1
   * @n           eStopBit2
   */
  void setCheckbitStopbit(uint16_t mode);

  /**
   * @fn setAllMeasurementParameters
   * @brief configure the value at measurement start position, configure the value at measurement stop position,
   * @n     configure the initial threshold, configure the end threshold,
   * @n     configure the module sensitivity, configure the comparison offset
   * @param startingPosition start position value,70~6600(0x0046~0x19C8)
   * @param stopPosition stop position value,70~6600(0x0046~0x19C8)
   * @param initialThreshold initial threshold,100~10000(0x0064~0x2710)
   * @param endThreshold end threshold,100~10000(0x0064~0x2710)
   * @param moduleSensitivity module sensitivity,0x0000~0x0004
   * @param comparisonOffset comparison offset,-32768~32767(0~0xFFFF)
   */
  void setAllMeasurementParameters(uint16_t startingPosition, uint16_t stopPosition,
                                   uint16_t initialThreshold, uint16_t endThreshold,
                                   uint16_t moduleSensitivity, uint16_t comparisonOffset);

  /**
   * @fn restoreFactorySetting
   * @brief restore factory setting
   */
  void restoreFactorySetting(void);

```


## Compatibility

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

MCU                | Work Well    | Work Wrong   |  Untested   | Remarks
------------------ | :----------: | :----------: | :---------: | :---:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |


## History

- 2021/07/08 - Version 1.0.0 released.


## Credits

Written by qsjhyy(yihuan.huang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

