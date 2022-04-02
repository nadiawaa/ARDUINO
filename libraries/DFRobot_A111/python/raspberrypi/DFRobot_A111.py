# -*- coding: utf-8 -*
'''!
  @file  DFRobot_A111.py
  @brief  Define the infrastructure of DFRobot_A111 class
  @details  get and configure the sensor basic information and measurement parameters, and get the sensor measurement information
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2021-07-23
  @url  https://github.com/DFRobot/DFRobot_A111
'''
import sys
import time
import serial

import modbus_tk
import modbus_tk.defines as cst
from modbus_tk import modbus_rtu

import logging
from ctypes import *


logger = logging.getLogger()
#logger.setLevel(logging.INFO)   # Display all print information
logger.setLevel(logging.FATAL)   # If you want to only display print errors, please use this option
ph = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s - [%(filename)s %(funcName)s]:%(lineno)d - %(levelname)s: %(message)s")
ph.setFormatter(formatter) 
logger.addHandler(ph)

## module PID (the highest two values are used as class to judge 00:SEN、01:DFR、10:TEL, the next 14 numbers as num)(SEN0489)
A111_PID                  = 0x01E9

# A111 register address for basic information
## module PID memory register, the default value is 0x01E9 (the highest two values are used as class to judge 00:SEN、01:DFR、10:TEL, the next 14 numbers as num)(SEN0489)
A111_PID_REG              = 0x0000
## module VID memory register, the default value is 0x3343 (representative manufacturer is DFRobot)
A111_VID_REG              = 0x0001
## memory register of module communication address, the default value is 0x000E, module device address(1~247)
A111_ADDR_REG             = 0x0002
## module baud rate memory register, the default value is 0x0008
A111_BAUDRATE_REG         = 0x0003
## module check bit and stop bit memory register, the default value is 0x0001
A111_CHECKBIT_STOPBIT_REG = 0x0004
## memory register of firmware revision number:0x1000 represents V1.0.0.0
A111_VERSION_REG          = 0x0005

# A111 register address of measurement data
## detect the current object numbers
A111_TARGETS_NUMBER       = 0x0006
## distance of object 1
A111_DISTANCE_TARGET1     = 0x0007
## intensity of object 1
A111_INTENSITY_TARGET1    = 0x0008
## distance of object 2
A111_DISTANCE_TARGET2     = 0x0009
## intensity of object 2
A111_INTENSITY_TARGET2    = 0x000A
## distance of object 3
A111_DISTANCE_TARGET3     = 0x000B
## intensity of object 3
A111_INTENSITY_TARGET3    = 0x000C
## distance of object 4
A111_DISTANCE_TARGET4     = 0x000D
## intensity of object 4
A111_INTENSITY_TARGET4    = 0x000E
## distance of object 5
A111_DISTANCE_TARGET5     = 0x000F
## intensity of object 5
A111_INTENSITY_TARGET5    = 0x0010

# A111 configure register address
## configure register at measurement start position, the default value is 0x00C8
MEASUREMENT_START_POSITION   = 0x0011
## configure register at measurement stop position, the default value is 0x1770
MEASUREMENT_END_POSITION       = 0x0012
## configure register for the initial threshold, the default value is 0x0190
A111_START_THRESHOLD          = 0x0013
## configure register for the end threshold, the default value is 0x0190
A111_END_THRESHOLD              = 0x0014
## configure register for the module sensitivity, the default value is 0x0002
A111_MODULE_SENSITIVITY         = 0x0015
## configure register for the comparison offset, the default value is 0x0000
A111_COMPARISON_OFFSET          = 0x0016
## restore factory setting
A111_RESET_FACTORY             = 0x0017


class DFRobot_A111(object):
    '''!
      @brief define DFRobot_A111 class
      @details to drive a111 radar
    '''

    ## baud rate 2400
    E_BAUDRATE_2400 = 0x0001
    ## baud rate 4800
    E_BAUDRATE_4800 = 0x0002
    ## baud rate 9600
    E_BAUDRATE_9600 = 0x0003
    ## baud rate 14400
    E_BAUDRATE_14400 = 0x0004
    ## baud rate 19200
    E_BAUDRATE_19200 = 0x0005
    ## baud rate 38400
    E_BAUDRATE_38400 = 0x0006
    ## baud rate 57600
    E_BAUDRATE_57600 = 0x0007
    ## baud rate 115200
    E_BAUDRATE_115200 = 0x0008

    ## check bit NONE
    E_CHECKBIT_NONE = 0x0000<<8
    ## check bit EVEN
    E_CHECKBIT_EVEN = 0x0001<<8
    ## check bit ODD
    E_CHECKBIT_ODD = 0x0002<<8

    ## stop bit 1bit
    E_STOPBIT_1 = 0x0001
    ## stop bit 2bit
    E_STOPBIT_2 = 0x0003

    def __init__(self, addr, port="/dev/ttyAMA0", baud = 115200, bytesize = 8, parity = 'N', stopbit = 1, xonxoff=0):
        '''!
          @brief Module RTU communication init
          @param addr modbus communication address
          @param port modbus communication serial port
          @param baud modbus communication baud rate
          @param bytesize modbus communication data bits
          @param parity modbus communication check bit
          @param stopbit modbus communication stop bit
          @param xonxoff modbus communication synchronous and asynchronous setting
        '''
        self._a111_addr = addr

        self._DFRobot_RTU = modbus_rtu.RtuMaster(
            serial.Serial(port, baud, bytesize, parity, stopbit, xonxoff)
        )
        self._DFRobot_RTU.set_timeout(0.5)
        self._DFRobot_RTU.set_verbose(True)
        self.reg_value_buf = [0, 0, 0, 0, 0, 0]
        self.reg_value_buf[0] = 0x00C8   # the default measurement start position 200
        self.reg_value_buf[1] = 0x1770   # the default measurement stop position 6000
        self.reg_value_buf[2] = 0x0190   # the default initial threshold 400
        self.reg_value_buf[3] = 0x0190   # the default end threshold 400
        self.reg_value_buf[4] = 0x0002   # the default measurement sensitivity 2
        self.reg_value_buf[5] = 0x0000   # the default comparison offset 0

    def begin(self):
        '''!
          @brief Initialize sensor
          @return  return initialization status
          @retval True indicate initialization succeed
          @retval False indicate initialization failed
        '''
        time.sleep(1)
        ret = True
        if(self._a111_addr >0xF7) and (self._a111_addr < 1):
            ret = False

        if A111_PID != self._read_reg(A111_PID_REG, 1)[0]:
            ret = False

        return ret

    def read_basic_info(self):
        '''!
          @brief read the device basic information
          @return list: 
          @retval  the first element: module PID
          @retval  the second element: module VID
          @retval  the third element: the module communication address
          @retval  the fourth element: the module baud rate
          @retval  the fifth element: the module check bit and stop bit
          @retval  the sixth element: firmware revision number
        '''
        return self._read_reg(A111_PID_REG, 6)

    def read_measurement_data(self):
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
        return self._read_reg(A111_TARGETS_NUMBER, 11)

    def read_measurement_config(self):
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
        return self._read_reg(MEASUREMENT_START_POSITION, 6)

    def set_ADDR(self, addr):
        '''!
          @brief set the module communication address
          @param addr the device address to be set, (1~247 is 0x0001~0x00F7)
        '''
        if 0x0001 < addr < 0x00F7:
            if 0 != len(self._write_reg(A111_ADDR_REG, [addr])):
                self._a111_addr = addr
            else:
                logger.info("Set addr failed!")

    def set_baudrate_mode(self, mode):
        '''!
          @brief set the module baud rate, power off to save the settings, and restart for the settings to take effect
          @param mode the baud rate to be set:
          @n     E_BAUDRATE_2400---2400, E_BAUDRATE_4800---4800, E_BAUDRATE_9600---9600, 
          @n     E_BAUDRATE_14400---14400, E_BAUDRATE_19200---19200, E_BAUDRATE_38400---38400, 
          @n     E_BAUDRATE_57600---57600, E_BAUDRATE_115200---115200
        '''
        if 0 != len(self._write_reg(A111_BAUDRATE_REG, [mode])):
            time.sleep(0.5)
        else:
            logger.info("Set baudrate failed!")

    def set_checkbit_stopbit(self, mode):
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
        if 0 == len(self._write_reg(A111_CHECKBIT_STOPBIT_REG, [mode])):
            logger.info("Set checkbit and stopbit failed!")

    def set_all_measurement_parameters(self, starting_position, stop_position, 
                                        initial_threshold, end_threshold,
                                        module_sensitivity, comparison_offset):
        '''!
          @brief configure the value at measurement start position, configure the value at measurement stop position, 
          @n     configure the initial threshold, configure the end threshold, 
          @n     configure the module sensitivity, configure the comparison offset
          @param starting_position value at start position,0x0046~0x19C8
          @param stop_position value at stop position,0x0046~0x19C8
          @param initial_threshold initial threshold,0x0064~0x2710
          @param end_threshold end threshold,0x0064~0x2710
          @param module_sensitivity module sensitivity,0x0000~0x0004
          @param comparison_offset comparison offset,0x0000~0xFFFF
        '''
        self.reg_value_buf = self._read_reg(MEASUREMENT_START_POSITION, 6)
        if 0 == len(self.reg_value_buf):
            logger.info("read all measurement parameters failed!")
        else:
            if 0x0046 <= starting_position <= self.reg_value_buf[1]:
                self.reg_value_buf[0] = starting_position
            if self.reg_value_buf[0] <= stop_position <= 0x19C8:
                self.reg_value_buf[1] = stop_position
            if 0x0064 <= initial_threshold <= 0x2710 and 0 < initial_threshold + self.reg_value_buf[5]:
                self.reg_value_buf[2] = initial_threshold
            if 0x0064 <= end_threshold <= 0x2710 and 0 < end_threshold + self.reg_value_buf[5]:
                self.reg_value_buf[3] = end_threshold
            if 0x0000 <= module_sensitivity <= 0x0004:
                self.reg_value_buf[4] = module_sensitivity
            if (0 < self.reg_value_buf[2] + comparison_offset) and (0 < comparison_offset + self.reg_value_buf[3]):
                self.reg_value_buf[5] = comparison_offset

            if 0 == len(self._write_reg(MEASUREMENT_START_POSITION, self.reg_value_buf)):
                logger.info("set all measurement parameters failed!")
            time.sleep(1)

    def restore_factory_setting(self):
        '''!
          @brief restore factory setting
        '''
        if 0 != self._write_reg(A111_RESET_FACTORY, 0x0000):
            logger.info("restore factory setting failed!")

    def _write_reg(self, reg, data):
        '''!
          @brief writes data to a register
          @param reg register address
                 data written data
          @return Write register address, and write length
        '''
        # Low level register writing, not implemented in base class
        if isinstance(data, int):
            data = [data]
        ret = self._DFRobot_RTU.execute(self._a111_addr, cst.WRITE_MULTIPLE_REGISTERS, reg, output_value=data)
        logger.info(ret)
        return ret

    def _read_reg(self, reg, length):
        '''!
          @brief read the data from the register
          @param reg register address
                 length read data length
          @return list: The value list of the holding register.
        '''
        # Low level register writing, not implemented in base class
        return list(self._DFRobot_RTU.execute(self._a111_addr, cst.READ_HOLDING_REGISTERS, reg, length))
