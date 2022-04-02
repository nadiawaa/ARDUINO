/*
  Lora Node2 
  https://www.electroniclinic.com/

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

// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 13 

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


//packet counter
int counter = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
 float Celcius=0;
 float Fahrenheit=0;

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xFF;     
byte Node2 = 0xCC;


void setup() {
 //initialize Serial Monitor
  Serial.begin(9600);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

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
  Serial.println(counter);

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER 2");
  display.setCursor(0,20);
  display.print("LoRa packet sent.");
  display.setCursor(0,30);
  display.print("Counter:");
  display.setCursor(50,30);
  display.print(counter);  
  display.setCursor(0,40);
  display.print(sensors.getTempCByIndex(0));     
  display.display();
}

void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination address
  LoRa.write(Node2);             // add sender address
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
   // Serial.println("error: message length does not match length");
   ;
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node2 && recipient != MasterNode) {
    //Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
    Serial.println(incoming);
    int Val = incoming.toInt();
    if(Val == 55)
    { 
    myTemp(); 
    String message = String(Celcius); 
    LoRa.print(counter);
    LoRa.print(sensors.getTempCByIndex(0));
    sendMessage(message,MasterNode,Node2);
    delay(100);
    }
  
}

void myTemp()
{
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print("Temperature is: "); 
  Serial.print(sensors.getTempCByIndex(0));
  delay(1000);

  if ( Celcius > 40 )
  {
    String message = "Warning.."; 
    sendMessage(message,MasterNode,Node2);
  }

}
