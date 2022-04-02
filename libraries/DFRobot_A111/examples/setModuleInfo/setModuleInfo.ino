/*!
 * @file  setModuleInfo.ino
 * @brief  configure the basic information and measurement parameters of the sensor
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
 * @brief DFRobot_A111_RS485 constructor
 * @param addr: modbus slave address(range1~247)or broadcast address(0x00)
 * @n     if it's configured a broadcast address, send a broadcast packet, and all slaves on bus will process it but not respond
 */
DFRobot_A111 sensor(/*addr =*/DEFAULT_DEVICE_ADDRESS);

#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  SoftwareSerial mySerial(/*rx =*/4, /*tx =*/5);
#endif


/*measured data printing function*/
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
   * @brief initialization function
   * @param _serial serial ports for communication, supporting hard and soft serial ports
   * @return returning 0 means read succeeds
   */
  while( NO_ERROR != sensor.begin(/*s =*/_serial) ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
  Serial.println("Initializing...");

/***************** sensor basic information configuration ******************************/

  /**
   * set the module communication address, the default address is 0x000E
   * addr the device address to be set,(1~247 is 0x0001~0x00F7)
   */
  sensor.setADDR(0x000E);

  /**
   *  set the module baud rate, the setting takes effect after power fail and restart, the default value is 115200
   *  addr the baud rate to be set：
   *  eBaudrate2400---2400 , eBaudrate4800---4800 , eBaudrate9600---9600 , 
   *  eBaudrate14400---14400 ,eBaudrate19200---19200 , eBaudrate38400---38400 , 
   *  eBaudrate57600---57600 , eBaudrate115200---115200 ,eBaudrate_1000000---1000000
   */
  sensor.setBaudrateMode(sensor.eBaudrate115200);

  /**
   * set check bit and stop bit of the module
   * mode the mode to be set：
   * check bit：
   *       eCheckBitNone
   *       eCheckBitEven
   *       eCheckBitOdd
   * stop bit：
   *       eStopBit1
   *       eStopBit2
   */
  sensor.setCheckbitStopbit(sensor.eCheckBitNone | sensor.eStopBit1);

/***************** sensor measurement parameters configuration ******************************/

  /**
   * ---------------------------------------------
   * startingPosition：
   * configure the value at measurement start position, the default value is 200(0x00C8),
   * value value at start position,70~6600, can't be greater than the value at stop position
   * ---------------------------------------------
   * stopPosition：
   * configure the value at measurement stop position, the default value is 6000(0x1770),
   * value value at stop position,70~6600, can't be less than the value at start position
   * ---------------------------------------------
   * initialThreshold：
   * configure the initial threshold, the default value is 400(0x0190),
   * value the initial threshold,100~10000。
   * ---------------------------------------------
   * endThreshold：
   * configure the end threshold, the default value is 400(0x0190),
   * value the end threshold,100~10000。
   * ---------------------------------------------
   * moduleSensitivity：
   * configure the module sensitivity, the default value is 0x0002,
   * value the module sensitivity,0x0000~0x0004。
   * ---------------------------------------------
   * comparisonOffset：
   * configure the comparison offset, the default value is 0(0x0000),
   * value the comparison offset,-32768~32767
   * ---------------------------------------------
   */
  sensor.setAllMeasurementParameters(/*startingPosition=*/500, /*stopPosition=*/1500,
                                      /*initialThreshold=*/400, /*endThreshold=*/200,
                                      /*moduleSensitivity=*/0x0002, /*comparisonOffset=*/-100);

  /**
   * restore factory setting
   */
  //sensor.restoreFactorySetting();

  Serial.println();
  delay(1000);
}

void loop()
{
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
  delay(1000);
}
