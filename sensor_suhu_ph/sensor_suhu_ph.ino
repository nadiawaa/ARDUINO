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


#define SensorPin 35            //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

void setup(){
  //initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("pH meter experiment!");
}



void loop(){
  
  //sensor ph_____________________________________
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







//kalibrasi turbidity

//void setup() {
//  Serial.begin(9600);
// 
//}
//void loop() {
//  int sensorValue = analogRead(A0);
//  float voltage = sensorValue * (5.0 / 1024.0);
// 
//  Serial.println ("Sensor Output (V):");
//  Serial.println (voltage);
//  Serial.println();
//  delay(1000);
//}


//#include <Wire.h> 

 
//int sensorPin = A0;
//float volt;
//float ntu;
// 
//void setup()
//{
//  Serial.begin(9600);
//}
// 
//void loop()
//{
//    
//    volt = 0;
//    for(int i=0; i<800; i++)
//    {
//        volt += ((float)analogRead(sensorPin)/1023)*5;
//    }
//    volt = volt/800;
//    volt = round_to_dp(volt,2);
//    if(volt < 2.5){
//      ntu = 3000;
//    }else{
//      ntu = -1120.4*sq(volt)+5742.3*volt-4353.8; 
//    }
//    Serial.print(volt);
//    Serial.println(" V");
// 
//
//    Serial.print(ntu);
//    Serial.println(" NTU");
//    delay(1000);
//}
 
//float round_to_dp( float in_value, int decimal_place )
//{
//  float multiplier = powf( 10.0f, decimal_place );
//  in_value = roundf( in_value * multiplier ) / multiplier;
//  return in_value;
//}



//#include <Wire.h> 
//
// 
//int Turbidity_Sensor_Pin = 34;
//float Turbidity_Sensor_Voltage;
//int samples = 600;
//float ntu; // Nephelometric Turbidity Units
//
// 
//void setup()
//{
//  Serial.begin(9600); // for debugging purposes
//  pinMode(Turbidity_Sensor_Pin, INPUT);
//
//}
// 
//void loop()
//{
//    
//    Turbidity_Sensor_Voltage = 0;
//    /* its good to take some samples and take the average value. This can be quite handy 
//     *  in situations when the values fluctuates a bit. This way you can take the average value
//     *  i am going to take 600 samples
//     */
//    for(int i=0; i<samples; i++)
//    {
//        Turbidity_Sensor_Voltage += ((float)analogRead(Turbidity_Sensor_Pin)/4880)*5;
//    }
//    
//    Turbidity_Sensor_Voltage = Turbidity_Sensor_Voltage/samples;
//    // uncomment the following two statments to check the voltage. 
//    // if you see any variations, take necessary steps to correct them
//    // once you are satisfied with the voltage value then again you can comment these lines
//    
////    Serial.println("Voltage:");
////    Serial.println(Turbidity_Sensor_Voltage);
//     
//    Turbidity_Sensor_Voltage = round_to_dp(Turbidity_Sensor_Voltage,2);
//    if(Turbidity_Sensor_Voltage < 2.5){
//      ntu = 3000;
//    }else{
//      ntu = -1120.4*sq(Turbidity_Sensor_Voltage)+ 5742.3*Turbidity_Sensor_Voltage - 4352.9; 
//    }
//
//    Serial.print(Turbidity_Sensor_Voltage,3);
//    Serial.println(" V");
// 
//    Serial.print(ntu);
//    Serial.println(" NTU");
//    delay(1000);
//}
// 
//float round_to_dp( float in_value, int decimal_place )
//{
//  float multiplier = powf( 10.0f, decimal_place );
//  in_value = roundf( in_value * multiplier ) / multiplier;
//  return in_value;
//}
