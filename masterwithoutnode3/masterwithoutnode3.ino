/*
  Master Lora Node
  myIpond

*/

#include <FirebaseESP32.h>
#include <WiFi.h>

#define FIREBASE_HOST "my-i-pond-default-rtdb.asia-southeast1.firebasedatabase.app"
#define WIFI_SSID "Abhyasa"
#define WIFI_PASSWORD "DRA012108"
#define FIREBASE_Authorization_key "VqaLWbyEY9nEly5jyO3NZjAotLkAXHMfmRzXN0b5"

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
#define SCREEN_WIDTH 128      // OLED display width, in pixels
#define SCREEN_HEIGHT 64      // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
 
byte MasterNode = 0xFF;     
byte Node1 = 0xBB;
byte Node2 = 0xCC; 

String SenderNode = "";
String outgoing;             // outgoing message
String LoRaData;

byte msgCount = 0;           // count of outgoing messages

// Tracks the time since last event fired
unsigned long previousMillis=0;
unsigned long int previoussecs = 0; 
unsigned long int currentsecs = 0; 
unsigned long currentMillis = 0;
int interval= 1 ; // updated every 1 second
int Secs = 0; 

FirebaseData firebaseData;
FirebaseJson json;

int sensorsuhu = 0;
int sensorph = 0;
int sensortbd = 0;
int rssi = LoRa.packetRssi();

void setup() {
 //initialize Serial Monitor
  Serial.begin(9600);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);
  
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
  display.print("LORA RECEIVER ");
  display.display();
  
  Serial.println("LoRa Receiver");
  
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
  display.println("LoRa Initializing OK!");
  display.display();  
}

void loop() {
  currentMillis = millis();
     currentsecs = currentMillis / 1000; 
      if ((unsigned long)(currentsecs - previoussecs) >= interval) {
        Secs = Secs + 1;
        //Serial.println(Secs);
        if ( Secs >= 11 ) {
          Secs = 0; 
        }
        if ((Secs >= 1) && (Secs <= 5)) {
          String message = "34"; 
          sendMessage(message,MasterNode, Node1);
        }
        if ((Secs >= 6 ) && (Secs <= 10)) {
          String message = "55"; 
          sendMessage(message,MasterNode, Node2);
        }
        previoussecs = currentsecs;
      }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());

  //read packet
  while (LoRa.available()) {
   LoRaData = LoRa.readString();
   Serial.print(LoRaData);
  } 
   
}

void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(otherNode);                // add destination address
  LoRa.write(MasterNode);               // add sender address
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
  if( sender == 0XBB )
    SenderNode = "Node1:";
  if( sender == 0XCC )
    SenderNode = "Node2:";
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node1 && recipient != MasterNode) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  //clear display
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print(SenderNode);
  display.setCursor(0,10);
  display.print(msgCount);

  String q = getValue(incoming,',',0);
  String r = getValue(incoming,',',1);
  String s = getValue(incoming,',',2);
  String t = getValue(incoming,',',3);
  
  if( sender == 0XBB ){ 
    Serial.println("node 1");
    display.setCursor(50,10);
    display.print("RSSI:" + rssi);
    Serial.print("RSSI: " + rssi);
    display.setCursor(0, 20);
    display.print("suhu: "+ q +" C");
    Serial.println("suhu: "+ q +" C");
    Firebase.setString(firebaseData, "dev1/TEMPERATURE", q);
    display.setCursor(0, 30);
    display.print("pH: "+ r);
    Serial.println("pH: "+ r);
    Firebase.setString(firebaseData, "dev1/PH", r);
    display.setCursor(0, 40);
    display.print("tbd: " + s);
    Serial.println("tbd: " + s);
    Firebase.setString(firebaseData, "dev1/TBD", s);
    display.setCursor(0, 50);
    display.print("count " + t);
    Serial.println("count " + t);
    Firebase.setString(firebaseData, "dev1/COUNT", t);
  }
  if( sender == 0XCC ){ 
    Serial.println("node 2");
    display.setCursor(50,10);
    display.print("RSSI:" + rssi);
    Serial.println("RSSI: " + rssi);
    display.setCursor(0, 20);
    display.print("suhu: "+ q +" C");
    Serial.println("suhu: "+ q +" C");
    Firebase.setString(firebaseData, "dev2/TEMPERATURE", q);
    display.setCursor(0, 30);
    display.print("pH: " + r);
    Serial.println("pH: " + r);
    Firebase.setString(firebaseData, "dev2/PH", r);
    display.setCursor(0, 40);
    display.print("tbd: "+ s);
    Serial.println("tbd: "+ s);
    Firebase.setString(firebaseData, "dev2/TBD", s);
    display.setCursor(0, 50);
    display.print("count "+ t);
    Serial.println("count "+ t);
    Firebase.setString(firebaseData, "dev2/COUNT", t);
  }
  display.display(); 
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
