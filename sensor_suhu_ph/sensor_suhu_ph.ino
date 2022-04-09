//#include <OneWire.h>
//#include <DallasTemperature.h>
//#define DS18B20PIN 13
//
///* Create an instance of OneWire */
//OneWire oneWire(DS18B20PIN);
//
//DallasTemperature sensors(&oneWire);
//
//
//#define SensorPin 35            //pH meter Analog output to Arduino Analog Input 0
//#define Offset 0.00            //deviation compensate
//#define samplingInterval 20
//#define printInterval 800
//#define ArrayLenth  40    //times of collection
//int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
//int pHArrayIndex=0;
//void setup(void)
//{
//  Serial.begin(9600);
//  Serial.println("pH meter experiment!");    //Test the serial monitor
//}
//void loop(void)
//{
//
////  Sensor Suhu___________________________________________________________________________
//  
//  sensors.requestTemperatures(); 
//  float tempinC = sensors.getTempCByIndex(0);
//  Serial.print("Temperature = ");
//  Serial.print(tempinC);
//  Serial.println("ºC");
//  delay(1000);
//}


/*
# This sample codes is for testing the pH meter V1.0.
 # Editor : YouYou
 # Date   : 2013.10.12
 # Ver    : 0.1
 # Product: pH meter
 # SKU    : SEN0161
*/

#define SensorPin 35          //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

void setup()
{
  
  Serial.begin(9600);  
  Serial.println("Ready");    //Test the serial monitor
}
void loop()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/6024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
}
