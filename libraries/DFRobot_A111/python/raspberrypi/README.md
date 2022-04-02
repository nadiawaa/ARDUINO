# DFRobot_A111
* [中文版](./README_CN.md)

The millimeter wave radar ranging sensor employs A111 radar chip.
The A111 60GHZ radar system is based on pulsed coherent radar (PCR) technology enabling millimeter accuracy with very low power consumption.
By operating in the 60GHz unlicensed ISM radio band, the radar sensor provides robust performance without interference from noise, dust, color nor direct or indirect light. Radio technology makes the sensor easy to integrate without the need for an aperture.
The A111 radar sensor is optimized for ultra-low power and high precision and fully integrated into a small 29mm² chip. It is a one-package solution with integrated Antenna in Package (AiP). This will make it be a leading global product with ultra-low power radar.
The A111 radar sensor can detect multiple objects at close range with single measurements as well as continuous sweeps set to any frequency up to 1500 Hz.
The unique characteristics of the A111 radar sensor make it possible to identify materials and detect motion in advanced sensor applications.

![产品实物图](../../resources/images/A111.png)


## Product Link (链接到英文商城)
    SKU:SEN0489


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

The library uses the RTU communication protocol and relies on the modbus_tk library. If you want to install and run the library you have to install its dependent libraries.
The library, with radar A111, can get the basic information, measurement parameters and measured data of the radar module, and can also change the basic information and measurement parameters.


## Installation

This library uses the RTU communication protocol, which requires the Raspberry Pi serial port. For serial port configuration, refer to the product wiki.

When using the library, modbus_tk is needed. Check whether Raspberry Pi has successfully imported modbus_tk before using the library. If the import fails, run the following command to install modbus_tk library.
python2: pip install modbus_tk
python3: pip3 install modbus_tk

Download the library file before use, paste it into the specified directory, then open the Examples folder and run the demo in the folder.


## Methods

```python

    '''!
      @brief Initialize sensor
      @return  return initialization status
      @retval True indicate initialization succeed
      @retval False indicate failed
    '''
    def begin(self):

    '''!
      @brief read the device basic information
      @return list: 
      @retval  the first element: the module PID
      @retval  the second element: the module VID
      @retval  the third element: the module communication address
      @retval  the fourth element: the module baud rate
      @retval  the fifth element: the module check bit and stop bit
      @retval  the sixth element: firmware revision number
    '''
    def refreshBasicInfo(self):

    '''!
      @brief read the module measured data
      @return list: 
      @retval  the first element: the number of objects currently detected
      @retval  the second element: measured distance of the first object；the third element: measured intensity of the first object
      @retval  the fourth element: measured distance of the second object；the fifth element: measured intensity of the second object
      @retval  the sixth element: measured distance of the third object；the seventh element: measured intensity of the third object
      @retval  the eighth element: measured distance of the fourth object；the ninth element: measured intensity of the fourth object
      @retval  the tenth element: measured distance of the fifth object；the eleventh element: measured intensity of the fifth object
    '''
    def refreshMeasurementData(self):

    '''!
      @brief read the module measurement parameters currently configured
      @return list: 
      @retval  the first element: current set value at measurement start position 
      @retval  the second element: current set value at measurement stop position 
      @retval  the third element: current initial threshold set value
      @retval  the fourth element: current end threshold set value
      @retval  the fifth element: current module sensitivity set value
      @retval  the sixth element: current comparison offset set value
    '''
    def refreshMeasurementConfig(self):

    '''!
      @brief set the module communication address
      @param addr the device address to be set, (1~247 is 0x0001~0x00F7)
    '''
    def setADDR(self, addr):

    '''!
      @brief set the module baud rate, power off to save the settings, and restart for the settings to take effect
      @param mode the baud rate to be set:
      @n     E_BAUDRATE_2400---2400, E_BAUDRATE_4800---4800, E_BAUDRATE_9600---9600, 
      @n     E_BAUDRATE_14400---14400, E_BAUDRATE_19200---19200, E_BAUDRATE_38400---38400, 
      @n     E_BAUDRATE_57600---57600, E_BAUDRATE_115200---115200
    '''
    def setBaudrateMode(self, mode):

    '''!
      @brief set check bit and stop bit of the module
      @param mode the mode to be set, the following patterns constitute mode:
      @n     check bit:
      @n          E_CHECKBIT_NONE
      @n          E_CHECKBIT_EVEN
      @n          E_CHECKBIT_ODD
      @n     stop bit:
      @n          E_STOPBIT_1
      @n          E_STOPBIT_2
    '''
    def setCheckbitStopbit(self, mode):

    '''!
      @brief configure the value at measurement start position, configure the value at measurement stop position, 
      @n     configure the initial threshold, configure the end threshold, 
      @n     configure the module sensitivity, configure the comparison offset
      @param starting_position the value at start position,0x0046~0x19C8
      @param stop_position the value at stop position,0x0046~0x19C8
      @param initial_threshold the initial threshold,0x0064~0x2710
      @param end_threshold the end threshold,0x0064~0x2710
      @param module_sensitivity the module sensitivity,0x0000~0x0004
      @param comparison_offset the comparison offset,0x0000~0xFFFF
    '''
    def setAllMeasurementParameters(self, startingPosition, stopPosition, 
                                        initialThreshold, endThreshold,
                                        moduleSensitivity, comparisonOffset):

   '''!
     @brief restore factory setting
   '''
    def restoreFactorySetting(self):

```


## Compatibility

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## History

- 2021/07/08 - Version 1.0.0 released.


## Credits

Written by qsjhyy(yihuan.huang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))
