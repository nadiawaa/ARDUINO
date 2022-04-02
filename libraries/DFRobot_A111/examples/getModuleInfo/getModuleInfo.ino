/*!
 * @file  getModuleInfo.ino
 * @brief  read all the register data from the sensor
 * @details  experimental phenomenon: the serial port has printed the sensor basic information, measure parameters and measured data
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author   [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-07-06
 * @url   https://github.com/DFRobot/DFRobot_A111
 */
#include <DFRobot_A111.h>
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  #include <SoftwareSerial.h>
#endif

#define DEFAULT_DEVICE_ADDRESS 0x000E
/**
 * DFRobot_A111_RS485 constructor
 * addr: modbus slave address(range1~247)or broadcast address(0x00)
 * if it's configured a broadcast address, send a broadcast packet, and all slaves on bus will process it but not respond
 */
DFRobot_A111 sensor(/*addr =*/DEFAULT_DEVICE_ADDRESS);

#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  SoftwareSerial mySerial(/*rx =*/4, /*tx =*/5);
#endif


/* measured data printing function */
void print_measure_data(int number, uint16_t distance, uint16_t intensity)
{
  Serial.print("target ");
  Serial.print((number + 1) / 2);
  Serial.print(" distance: ");
  Serial.print(distance);
  Serial.print("  ");
  for(uint16_t i=distance/10; i>0; i--)
    Serial.print("-");
  Serial.println();

  Serial.print("target ");
  Serial.print((number + 1) / 2);
  Serial.print(" intensity: ");
  Serial.print(intensity);
  Serial.print("  ");
  for(uint16_t i=intensity/10; i>0; i--)
    Serial.print("+");
  Serial.println();
}


void setup(void)
{
  Serial.begin(115200);
  Stream *_serial;
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  mySerial.begin(115200);   // excessive baud rate of UNO soft serial port will makes communication unstable, 9600 is recommended
  _serial = &mySerial;
#elif defined(ESP32)
  Serial1.begin(115200, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
  _serial = &Serial1;
#else
  Serial1.begin(115200);
  _serial = &Serial1;
#endif

  /**
   * initialization function
   * _serial serial ports for communication, supporting hard and soft serial ports
   * returning 0 means read succeeds
   */
  while( NO_ERROR != sensor.begin(/*s =*/_serial) ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
  Serial.println();
}

void loop()
{
  Serial.println("-----------------read module basic information---------------------");
  /**
   * retrieve the basic information from the sensor and buffer it into the basicInfo structure that stores information:
   * basicInfo structure members:
   *     PID: module PID
   *     VID: module VID
   *     modbusAddr: module communication address
   *     baudrate: module baud rate
   *     checkbit: check bit
   *     stopbit: stop bit
   *     versions: firmware revision number
   * returning 0 means read succeeds
   */
  if(0 == sensor.refreshBasicInfo()){
    /* module PID, the default value is 0x01E9 (the highest two values are used as class to judge 00: SEN、01: DFR、10: TEL, the next 14 numbers as num)(SEN0489) */
    Serial.print("PID: 0x");
    Serial.println(sensor.basicInfo.PID, HEX);

    /* module VID, the default value is 0x3343(representative manufacturer is DFRobot) */
    Serial.print("VID: 0x");
    Serial.println(sensor.basicInfo.VID, HEX);

    /* module communication address, the default value is 0x000E, module device address(1~247) */
    Serial.print("mailing address: ");
    Serial.println(sensor.basicInfo.modbusAddr);

    /* module baud rate, the default value is 0x0009:
     * 0x0001---2400  0x0002---4800  0x0003---9600  0x0004---14400  0x0005---19200
     * 0x0006---38400  0x0007---57600  0x0008---115200  0x0009---1000000 */
    Serial.print("baudrate: 0x");
    Serial.println(sensor.basicInfo.baudrate, HEX);

    /* module check bit and stop bit, the default value is 0x0001
     * check bit: 0 is none; 1 is even; 2 is odd
     * stop bit: 1bit; 2bit */
    Serial.print("check bit: ");
    Serial.println(sensor.basicInfo.checkbit);
    Serial.print("stop bit: ");
    Serial.println(((sensor.basicInfo.stopbit & 0x00FF) + 1)/2);

    /* firmware revision number: 0x1000 represents V1.0.0.0 */
    Serial.print("versions: 0x");
    Serial.println(sensor.basicInfo.versions, HEX);

  }else{
    Serial.println("Failed to read basic information!!!");
  }
  Serial.println();

  Serial.println("-----------------read module measurement config--------------------");
  /**
   * retrieve the currently configured parameters from the sensor and buffer it into the structure measurementConfig that stores information:
   * measurementConfig structure members:
   *     startPosition: current set value at measurement start position
   *     stopPosition: current set value at measurement stop position
   *     initialThreshold: current initial threshold set value
   *     endThreshold: current end threshold set value
   *     moduleSensitivity: current module sensitivity set value
   *     comparisonOffset: current comparison offset set value
   * returning 0 means read succeeds
   */
  if(0 == sensor.refreshMeasurementConfig()){
    /* current set value at measurement start position, 0x0046~0x19C8, the value can't be greater than the set value at stop position */
    Serial.print("start position :  ");
    Serial.println(sensor.measurementConfig.startPosition);

    /* current set value at measurement stop position, 0x0046~0x19C8, the value can't be less than the set value at start position */
    Serial.print("stop position :  ");
    Serial.println(sensor.measurementConfig.stopPosition);

    /* current initial threshold set value */
    Serial.print("initial threshold : ");
    Serial.println(sensor.measurementConfig.initialThreshold);

    /* current end threshold set value */
    Serial.print("end threshold : ");
    Serial.println(sensor.measurementConfig.endThreshold);

    /* current module sensitivity set value */
    Serial.print("module sensitivity : ");
    Serial.println(sensor.measurementConfig.moduleSensitivity);

    /* current comparison offset set value */
    Serial.print("comparison offset : ");
    Serial.println(sensor.measurementConfig.comparisonOffset);

  }else{
    Serial.println("Failed to read measurement config!!!");
  }
  Serial.println();

  Serial.println("------------------Read module measurement data---------------------");
  /**
   * re-read the measured data from the sensor and buffer it into the array dataBuf[11] that stores information:
   *    dataBuf[0]: the number of objects currently detected
   *    dataBuf[1]: measured distance of the first object; dataBuf[2]: measured intensity of the first object
   *    dataBuf[3]: measured distance of the second object; dataBuf[4]: measured intensity of the second object
   *    dataBuf[5]: measured distance of the third object; dataBuf[6]: measured intensity of the third object
   *    dataBuf[7]: measured distance of the fourth object; dataBuf[8]: measured intensity of the fourth object
   *    dataBuf[9]: measured distance of the fifth object; dataBuf[10]: measured intensity of the fifth object
   * returning 0 means read succeeds
   */
  if(0 == sensor.refreshMeasurementData()){
    /*the number of objects currently detected*/
    Serial.print("target amount:  ");
    Serial.println(sensor.dataBuf[0]);
    Serial.println();

    /*measured data*/
    for(int i=1; i<11; i+=2){
      print_measure_data(i, sensor.dataBuf[i], sensor.dataBuf[i+1]);
    }

  }else{
    Serial.println("Failed to read measurement data!!!");
  }

  Serial.println("\n");
  delay(3000);
}
