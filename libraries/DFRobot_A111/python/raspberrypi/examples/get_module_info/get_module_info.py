# -*- coding: utf-8 -*
'''!
  @file get_module_info.py
  @brief read all the register data from the sensor
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2021-07-23
  @url  https://github.com/DFRobot/DFRobot_A111
'''
from __future__ import print_function
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))

from DFRobot_A111 import *

sensor = DFRobot_A111(addr = 0x000E)

def print_measure_data(number, distance, intensity):
  print("target: %d distance: %d:" %(number, distance), end='')
  for i in range(1, distance, 20):
    print("-", end='')
  print()

  print("target: %d intensity: %d:" %(number, intensity), end='')
  for i in range(1, intensity, 20):
    print("+", end='')
  print()


def setup():
  while (sensor.begin() == False):
    print ('Please check that the device is properly connected')
    time.sleep(3)
  print("sensor begin successfully!!!")


def loop():
  print()
  '''
    read the module basic information
    return the read data list
      the first element: module PID
      the second element: module VID
      the third element: module communication address
      the fourth element: module baud rate
      the fifth element: module check bit and stop bit
      the sixth element: firmware revision number
  '''
  buf = sensor.read_basic_info()
  if 0 != len(buf):
    # module PID, the default value is 0x01E9 (the highest two values are used as class to judge 00:SEN、01:DFR、10:TEL, the next 14 numbers as num)(SEN0489)
    print("PID: 0x%x"%buf[0])

    # module VID, the default value is 0x3343 (representative manufacturer is DFRobot)
    print("VID: 0x%x"%buf[1])

    # module communication address, the default value is 0x000E, module device address(1~247)
    print("mailing address: 0x%x"%buf[2])

    # module baud rate, the default value is 0x0009:
    # 0x0001---2400  0x0002---4800  0x0003---9600  0x0004---14400  0x0005---19200
    # 0x0006---38400  0x0007---57600  0x0008---115200  0x0009---1000000
    print("baudrate: 0x%x"%buf[3])

    # module check bit and stop bit, the default value is 0x0001
    # check bit:1 represents none;2 represents even;3 represents odd
    # stop bit:1bit;2bit
    print("check bit: %d"%(buf[4]>>8))
    print("stop bit: %d"%((buf[4]+1)/2))

    # firmware revision number:0x1000 represents V1.0.0.0
    print("versions: 0x%x "%buf[5])
  else:
    print("Failed to read basic information")
  print()

  '''
    read the module measurement parameters currently configured
    return the read data list
      the first element: current set value at measurement start position
      the second element: current set value at measurement stop position
      the third element: current initial threshold set value
      the fourth element: current end threshold set value
      the fifth element: current module sensitivity set value
      the sixth element: current comparison offset set value
  '''
  buf = sensor.read_measurement_config()
  if 0 != len(buf):
    # current measurement set value at start position, 0x0046~0x19C8, the value can't be greater than the set value at stop position
    print("starting position: %d"%buf[0])

    # current measurement set value at stop position, 0x0046~0x19C8, the value can't be less than the set value at start position
    print("stop position: %d"%buf[1])

    # current initial threshold set value
    print("initial threshold: %d"%buf[2])

    # current end threshold set value
    print("end threshold: %d"%buf[3])

    # current module sensitivity set value
    print("module sensitivity: %d"%buf[4])

    # current comparison offset set value
    print("comparison offset: 0x%x "%buf[5])
  else:
    print("Failed to read basic information")
  print()

  '''
    read the module measured data
    return the read data list
      the first element: the number of objects currently detected
      the second element: measured distance of the first object; the third element: measured intensity of the first object
      the fourth element: measured distance of the second object; the fifth element: measured intensity of the second object
      the sixth element: measured distance of the third object; the seventh element: measured intensity of the third object
      the eighth element: measured distance of the fourth object; the ninth element: measured intensity of the fourth object
      the tenth element: measured distance of the fifth object; the eleventh element: measured intensity of the fifth object
  '''
  buf_data = sensor.read_measurement_data()
  if 0 != len(buf_data):
    ''' the number of objects currently detected '''
    print("target amount: %d" %buf_data[0])

    ''' measured data '''
    for i in range(1, 6):
      print_measure_data(i, buf_data[i*2-1], buf_data[i*2])
  else:
    print("Failed to read measurement data!!!")
  print()

  time.sleep(3)

if __name__ == "__main__":
  setup()
  while True:
    loop()
