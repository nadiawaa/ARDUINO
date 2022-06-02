/*
  Master Lora Node
  myIpond

*/

#include <FirebaseESP32.h>
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>

#define FIREBASE_HOST "my-i-pond-default-rtdb.asia-southeast1.firebasedatabase.app"
#define WIFI_SSID "Abhyasa"
#define WIFI_PASSWORD "DRA012108"
#define FIREBASE_Authorization_key "VqaLWbyEY9nEly5jyO3NZjAotLkAXHMfmRzXN0b5"
const char* SERVER_NAME = "http://mountaineerz.000webhostapp.com/sensordata.php";

const char* SERVER_NAME1 = "http://mountaineerz.000webhostapp.com/sensordata1.php";
const char* SERVER_NAME2 = "http://mountaineerz.000webhostapp.com/sensordata2.php";
const char* SERVER_NAME3 = "http://mountaineerz.000webhostapp.com/sensordata3.php";


//-------------------------------------------------------------------
//Send an HTTP POST request every 30 seconds
unsigned long lastMillis = 0;
long interval = 5000;


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
String LoRaData;

int counter = 0; 

FirebaseData firebaseData;
FirebaseJson json;

String PROJECT_API_KEY = "hello world";

int sensorsuhu = 0;
int sensorph = 0;
int sensortbd = 0;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;




  float s2 = 234567; 
  float p2 = 2234567;
  float k2 = 22234567;
  float s3 = 345321; 
  float p3 = 3345321;
  float k3 = 33345321;



  
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup() { 
 //initialize Serial Monitor
  Serial.begin(9600);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();


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

    //-----------------------------------------------------------------
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    if(millis() - lastMillis > interval) {
       //Send an HTTP POST request every interval seconds
       upload_temperature1();
       lastMillis = millis();
    }
  }
  //-----------------------------------------------------------------
  else {
    Serial.println("WiFi Disconnected");
  }
  //-----------------------------------------------------------------


 //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet ");
    
   //print RSSI of packet
   int rssi = LoRa.packetRssi();
   Serial.print(" with RSSI ");    
   Serial.println(rssi);
    
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
  String temperature_data;
  String q = getValue(incoming,',',0);
  String r = getValue(incoming,',',1);
  String s = getValue(incoming,',',2);
  String t = getValue(incoming,',',3);
  
  if( sender == 0XBB ){ 
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("node1");
    Serial.println("node 1");
    display.setCursor(0,10);
    display.print("rssi: ");
    display.print(rssi);
    display.setCursor(0, 20);
    display.print("suhu: "+ q +" C");
    Serial.println("suhu: "+ q +" C");
    Firebase.setString(firebaseData, "dev1/TEMPERATURE", q);
    temperature_data += "&suhu1=" + q;
        
    display.setCursor(0, 30);
    display.print("pH: "+ r);
    Serial.println("pH: "+ r);
    Firebase.setString(firebaseData, "dev1/PH", r);
    temperature_data += "&ph1=" + r;
        
    display.setCursor(0, 40);
    display.print("tbd: " + s);
    Serial.println("tbd: " + s);
    Firebase.setString(firebaseData, "dev1/TBD", s);
    temperature_data += "&kekeruhan1=" + s;
    
    display.setCursor(0, 50);
    display.print("count " + t);
    Serial.println("count " + t);
    Firebase.setString(firebaseData, "dev1/COUNT", t);
    printLocalTime();
//  Firebase.setString(firebaseData, "dev1/time", &timeinfo;


      //HTTP POST request data
  String temperature_data1;
  temperature_data1 = "api_key="+PROJECT_API_KEY;
  temperature_data1 += "&suhu1="+q;
  temperature_data1 += "&ph1="+r;
  temperature_data1 += "&kekeruhan1="+s;
  
  WiFiClient client;
  HTTPClient http;
  
  http.begin(client, SERVER_NAME1);
  
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Send HTTP POST request
  int httpResponseCode1 = http.POST(temperature_data1);
  //--------------------------------------------------------------------------------

  Serial.print("HTTP Response code (1): ");
  Serial.println(httpResponseCode1);
    http.end();
  delay (10000);
  
  }
  if( sender == 0XCC ){ 
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("node2");
    Serial.println("node 2");
    display.setCursor(0,10);
    display.print("rssi: ");
    display.print(rssi);
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


  }
  display.display(); 
}




void upload_temperature1()
{
    String suhu2 = String(s2, 2);
  String ph2 = String(p2, 2);
  String kekeruhan2 = String(k2, 2);

  String suhu3 = String(s3, 2);
  String ph3 = String(p3, 2);
  String kekeruhan3 = String(k3, 2);





  
  String temperature_data2;
  temperature_data2 = "api_key="+PROJECT_API_KEY;
  temperature_data2 += "&suhu2="+suhu2;
  temperature_data2 += "&ph2="+ph2;
  temperature_data2 += "&kekeruhan2="+kekeruhan2;

    WiFiClient client;
    HTTPClient http;

  http.begin(client, SERVER_NAME2);
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Send HTTP POST request
  int httpResponseCode2 = http.POST(temperature_data2);
  Serial.print("HTTP Response code (2): ");
  Serial.println(httpResponseCode2);
  


  String temperature_data3;
  temperature_data3 = "api_key="+PROJECT_API_KEY;
  temperature_data3 += "&suhu3="+suhu3;
  temperature_data3 += "&ph3="+ph3;
  temperature_data3 += "&kekeruhan3="+kekeruhan3;

  http.begin(client, SERVER_NAME3);
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Send HTTP POST request
  int httpResponseCode3 = http.POST(temperature_data3);

  Serial.print("HTTP Response code (3): ");
  Serial.println(httpResponseCode3);

  http.end();
  delay (10000);
  
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
