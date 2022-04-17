/*
  Lora Node2 
  myIpond
*/
//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Library D18B20
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 13 

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 915E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels


#define SensorPin 35            //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;



int Turbidity_Sensor_Pin = 34;
float Turbidity_Sensor_Voltage;
int samples = 600;
float ntu;


// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xFF;     
byte Node2 = 0xCC;

float sensorsuhu = 0;
float sensorph = 0;
float sensortbd = 0;


void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("pH meter experiment!");

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  pinMode(Turbidity_Sensor_Pin, INPUT);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER 2");
  display.display();
  
  Serial.println("LoRa Sender Test 2");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(msgCount);

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());

  //sensor suhu_____________________________________
  sensors.requestTemperatures(); 
  Serial.print("Temperature is: "); 
  Serial.println(sensors.getTempCByIndex(0));
  delay(500);


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

  //sensor turbidity_______________________________________________
  Turbidity_Sensor_Voltage = 0;
    for(int i=0; i<samples; i++)
    {
        Turbidity_Sensor_Voltage += ((float)analogRead(Turbidity_Sensor_Pin)/4880)*5;
    }
    
    Turbidity_Sensor_Voltage = Turbidity_Sensor_Voltage/samples; 
    Turbidity_Sensor_Voltage = round_to_dp(Turbidity_Sensor_Voltage,2);
    if(Turbidity_Sensor_Voltage < 2.5){
      ntu = 3000;
    }else{
      ntu = -1120.4*sq(Turbidity_Sensor_Voltage)+ 5742.3*Turbidity_Sensor_Voltage - 4352.9; 
    }

    Serial.print(Turbidity_Sensor_Voltage);
    Serial.println(" V");
 
    Serial.print(ntu,3);
    Serial.println(" NTU");
    delay(1000);

  
  Serial.println("____________________________________________________________________");
  delay(1000);
  
  //loop________________________________________________
  sensorsuhu = sensors.getTempCByIndex(0),2;
  delay(10);
  sensorph = ("%.2f", phValue);
  delay(10);
  sensortbd = ("%.2f", ntu);
  delay(10);
  
  //display OLED_______________________________________
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER 2");
  display.setCursor(0,10);
  display.print("LoRa packet sent.");
  display.setCursor(0,20);
  display.print("Counter:");
  display.setCursor(50,20);
  display.print(msgCount);  
  display.setCursor(0,30);
  display.print("suhu:");
  display.print(sensorsuhu);     
  display.setCursor(0,40);
  display.print("pH:");
  display.print(sensorph); 
  display.setCursor(0,50);
  display.print("tbd:");
  display.print(sensortbd); 
  display.display();
}

void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);               // add destination address
  LoRa.write(Node2);                    // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node2 && recipient != MasterNode) {
    Serial.println("This message is not for me.");
    return;
  }
    Serial.print("incoming: ");
    Serial.println(incoming);
    int Val = incoming.toInt();
    if(Val == 55)
    { 
    String node2message; 
    LoRa.print(msgCount);
    node2message = node2message + sensorsuhu + "," + sensorph + "," + sensortbd;
    sendMessage(node2message,MasterNode,Node2);
    delay(100);
    }
  
}

float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
