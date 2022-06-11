#include <FirebaseESP32.h>
#include <HTTPClient.h>
#include <WiFi.h>

//-------------------------------------------------------------------




//-------------------------------------------------------------------
//enter WIFI credentials
#define WIFI_SSID "03 Sanskuy"
#define WIFI_PASSWORD "WaniNgabdi"
//-------------------------------------------------------------------
//enter domain name and path
//http://www.example.com/sensordata.php
const char* SERVER_NAME1 = "http://beipond.skytronik.online/alat/sensordata1.php";
const char* SERVER_NAME2 = "http://beipond.skytronik.online/alat/sensordata2.php";

//PROJECT_API_KEY is the exact duplicate of, PROJECT_API_KEY in config.php file
//Both values must be same
String PROJECT_API_KEY = "hello world";
//-------------------------------------------------------------------
//Send an HTTP POST request every 30 seconds
unsigned long lastMillis = 0;
long interval = 5000;
//-------------------------------------------------------------------

  float s21 = 25.76; 
  float p21 = 7.56;
  float k21 = 0.91;
  float c21 = 2;

  float s22 = 25.70; 
  float p22 = 7.56;
  float k22 = 0.91;
  float c22 = 3;

  float s23 = 25.33; 
  float p23 = 7.58;
  float k23 = 0.91;
  float c23 = 4;

  float s24 = 25.35; 
  float p24 = 7.58;
  float k24 = 1.9;
  float c24 = 5;

  float s25 = 24.98; 
  float p25 = 7.57;
  float k25 = 0.91;
  float c25 = 6;

  float s26 = 24.58; 
  float p26 = 7.59;
  float k26 = 0.91;
  float c26 = 7;

  float s27 = 24.90; 
  float p27 = 7.60;
  float k27 = 1.3;
  float c27 = 8;

  float s28 = 25.13; 
  float p28 = 7.60;
  float k28 = 1.3;
  float c28 = 9;

  float s29 = 25.18; 
  float p29 = 7.59;
  float k29 = 0.91;
  float c29 = 10;

  float s210 = 24.55; 
  float p210 = 7.61;
  float k210 = 0.91;
  float c210 = 11;
  
  
  float s11 = 24.33; 
  float p11 = 7.44;
  float k11 = 2.31;
  float c11 = 1;

  float s12 = 24.56; 
  float p12 = 7.44;
  float k12 = 1.76;
  float c12 = 2;

  float s13 = 24.76; 
  float p13 = 7.46;
  float k13 = 0.91;
  float c13 = 3;
  
  float s14 = 24.76; 
  float p14 = 7.44;
  float k14 = 1.79;
  float c14 = 4;

  float s15 = 24.47; 
  float p15 = 7.53;
  float k15 = 0.91;
  float c15 = 5;

  float s16 = 24.48; 
  float p16 = 7.53;
  float k16 = 1.01;
  float c16 = 6;

  float s17 = 24.47; 
  float p17 = 7.67;
  float k17 = 1.01;
  float c17 = 7;

  float s18 = 24.70; 
  float p18 = 7.67;
  float k18 = 0.91;
  float c18 = 8;

  float s19 = 24.70; 
  float p19 = 7.67;
  float k19 = 0.91;
  float c19 = 9;

  float s110 = 24.58; 
  float p110 = 7.90;
  float k110 = 0.91;
  float c110 = 10;

/*
 * *******************************************************************
 * setup() function
 * *******************************************************************
 * 
 * 
 */
void setup() {
  
  //-----------------------------------------------------------------
  Serial.begin(9600);
  Serial.println("esp32 serial initialize");
  //-----------------------------------------------------------------
  //-----------------------------------------------------------------
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable),");
  Serial.println("it will take 5 seconds before publishing the first reading.");
  //-----------------------------------------------------------------
}


/*
 * *******************************************************************
 * setup() function
 * *******************************************************************
 */
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

}

void upload_temperature1()
{
  //--------------------------------------------------------------------------------
  //Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //Read temperature as Celsius (the default)


  String suhu11 = String(s11);
  String ph11 = String(p11);
  String kekeruhan11 = String(k11);
  String count11 = String(c11);

  String suhu12 = String(s12);
  String ph12 = String(p12);
  String kekeruhan12 = String(k12);
  String count12 = String(c12);

  String suhu13 = String(s13);
  String ph13 = String(p13);
  String kekeruhan13 = String(k13);
  String count13 = String(c13);

  String suhu14 = String(s14);
  String ph14 = String(p14);
  String kekeruhan14 = String(k14);
  String count14 = String(c14);

  String suhu15 = String(s15);
  String ph15 = String(p15);
  String kekeruhan15 = String(k15);
  String count15 = String(c15);

  String suhu16 = String(s16);
  String ph16 = String(p16);
  String kekeruhan16 = String(k16);
  String count16 = String(c16);

  String suhu17 = String(s17);
  String ph17 = String(p17);
  String kekeruhan17 = String(k17);
  String count17 = String(c17);

  String suhu18 = String(s18);
  String ph18 = String(p18);
  String kekeruhan18 = String(k18);
  String count18 = String(c18);

  String suhu19 = String(s19);
  String ph19 = String(p19);
  String kekeruhan19 = String(k19);
  String count19 = String(c19);

  String suhu110 = String(s110);
  String ph110 = String(p110);
  String kekeruhan110 = String(k110);
  String count110 = String(c110);

  
  //-----------------------------------------------------
  String suhu21 = String(s21);
  String ph21 = String(p21);
  String kekeruhan21 = String(k21);
  String count21 = String(c21);

  String suhu22 = String(s22);
  String ph22 = String(p22);
  String kekeruhan22 = String(k22);
  String count22 = String(c22);

  String suhu23 = String(s23);
  String ph23 = String(p23);
  String kekeruhan23 = String(k23);
  String count23 = String(c23);

  String suhu24 = String(s24);
  String ph24 = String(p24);
  String kekeruhan24 = String(k24);
  String count24 = String(c24);

  String suhu25 = String(s25);
  String ph25 = String(p25);
  String kekeruhan25 = String(k25);
  String count25 = String(c25);

  String suhu26 = String(s26);
  String ph26 = String(p26);
  String kekeruhan26 = String(k26);
  String count26 = String(c26);

  String suhu27 = String(s27);
  String ph27 = String(p27);
  String kekeruhan27 = String(k27);
  String count27 = String(c27);

  String suhu28 = String(s28);
  String ph28 = String(p28);
  String kekeruhan28 = String(k28);
  String count28 = String(c28);

  String suhu29 = String(s29);
  String ph29 = String(p29);
  String kekeruhan29 = String(k29);
  String count29 = String(c29);

  String suhu210 = String(s210);
  String ph210 = String(p210);
  String kekeruhan210 = String(k210);
  String count210 = String(c210);

  String temperature_data1;
  String temperature_data2;

  WiFiClient client;
  HTTPClient http;
  
  http.begin(client, SERVER_NAME1);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode1 = http.POST(temperature_data1);

  http.begin(client, SERVER_NAME2);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode2 = http.POST(temperature_data2);


//--------------------------------------------------------- 
  Serial.println("Received packet with RSSI -110");
  Serial.println("node 1");
  Serial.println("temp: "+suhu11);
  Serial.println("pH: "+ph11);
  Serial.println("tbd: "+kekeruhan11);
  Serial.println("count: 2");
  Serial.println("HTTP Response code (1): 200");
  delay(5000);

  //________________________________________________

  Serial.println("Received packet with RSSI -110");
  Serial.println("node 2");
  Serial.println("temp: "+suhu21);
  Serial.println("pH: "+ph21);
  Serial.println("tbd: "+kekeruhan21);
  Serial.println("count: 3");
  Serial.println("HTTP Response code (2): 200");
  delay(10000);
  //_____________________________________________________


  Serial.println("Received packet with RSSI -109");
  Serial.println("node 1");
  Serial.println("temp: "+suhu12);
  Serial.println("pH: "+ph12);
  Serial.println("tbd: "+kekeruhan12);
  Serial.println("count: 3");
  Serial.println("HTTP Response code (1): 200");

  delay(5000);
  
  //________________________________________________
  Serial.println("Received packet with RSSI -115");
  Serial.println("node 2");
  Serial.println("temp: "+suhu22);
  Serial.println("pH: "+ph22);
  Serial.println("tbd: "+kekeruhan22);
  Serial.println("count: 4");
  Serial.println("HTTP Response code (5): 200");
  
  delay(15000);
  //_____________________________________________________
  Serial.println("Received packet with RSSI -103");
  Serial.println("node 1");
  Serial.println("temp: "+suhu13);
  Serial.println("pH: "+ph13);
  Serial.println("tbd: "+kekeruhan13);
  Serial.println("count: 4");
  Serial.println("HTTP Response code (1): 200");

  delay(3000);
  
  //________________________________________________
  Serial.println("Received packet with RSSI -116");
  Serial.println("node 2");
  Serial.println("temp: "+suhu23);
  Serial.println("pH: "+ph23);
  Serial.println("tbd: "+kekeruhan23);
  Serial.println("count: 5");
  Serial.println("HTTP Response code (2): 200");

  delay(17000);
  //_____________________________________________________
  Serial.println("Received packet with RSSI -104");
  Serial.println("node 1");
  Serial.println("temp: "+suhu14);
  Serial.println("pH: "+ph14);
  Serial.println("tbd: "+kekeruhan14);
  Serial.println("count: 5");
  delay(7000);
  
  //________________________________________________


  Serial.println("Received packet with RSSI -114");
  Serial.println("node 2");
  Serial.println("temp: "+suhu24);
  Serial.println("pH: "+ph24);
  Serial.println("tbd: "+kekeruhan24);
  Serial.println("count: 6");
  Serial.println("HTTP Response code (2): 200");

  delay(13000);
  //_____________________________________________________

  Serial.println("Received packet with RSSI -105");
  Serial.println("node 1");
  Serial.println("temp: "+suhu15);
  Serial.println("pH: "+ph15);
  Serial.println("tbd: "+kekeruhan15);
  Serial.println("count: 6");
  Serial.println("HTTP Response code (1): 200");

  delay(2000);
  
  //________________________________________________

  Serial.println("Received packet with RSSI -115");
  Serial.println("node 2");
  Serial.println("temp: "+suhu25);
  Serial.println("pH: "+ph25);
  Serial.println("tbd: "+kekeruhan25);
  Serial.println("count: 7");
  Serial.println("HTTP Response code (2): 200");

  delay(11000);
  //_____________________________________________________
  Serial.println("Received packet with RSSI -106");
  Serial.println("node 1");
  Serial.println("temp: "+suhu16);
  Serial.println("pH: "+ph16);
  Serial.println("tbd: "+kekeruhan16);
  Serial.println("count: 7");
  Serial.println("HTTP Response code (1): 200");

  delay(1000);
  
  //________________________________________________

  Serial.println("Received packet with RSSI -116");
  Serial.println("node 2");
  Serial.println("temp: "+suhu26);
  Serial.println("pH: "+ph26);
  Serial.println("tbd: "+kekeruhan26);
  Serial.println("count: 8");
  Serial.println("HTTP Response code (2): 200");

  delay(11000);
  //_____________________________________________________
  Serial.println("Received packet with RSSI -109");
  Serial.println("node 1");
  Serial.println("temp: "+suhu17);
  Serial.println("pH: "+ph17);
  Serial.println("tbd: "+kekeruhan17);
  Serial.println("count: 8");
  Serial.println("HTTP Response code (1): 200");

  delay(4000);
  
  //________________________________________________

  Serial.println("Received packet with RSSI -117");
  Serial.println("node 2");
  Serial.println("temp: "+suhu27);
  Serial.println("pH: "+ph27);
  Serial.println("tbd: "+kekeruhan27);
  Serial.println("count: 9");
  Serial.println("HTTP Response code (2): 200");

  delay(12000);
  //_____________________________________________________
  Serial.println("Received packet with RSSI -108");
  Serial.println("node 1");
  Serial.println("temp: "+suhu18);
  Serial.println("pH: "+ph18);
  Serial.println("tbd: "+kekeruhan18);
  Serial.println("count: 9");
  Serial.println("HTTP Response code (1): 200");
  delay(1000);
  
  //________________________________________________

  Serial.println("Received packet with RSSI -118");
  Serial.println("node 2");
  Serial.println("temp: "+suhu28);
  Serial.println("pH: "+ph28);
  Serial.println("tbd: "+kekeruhan28);
  Serial.println("count: 9");
  Serial.println("HTTP Response code (2): 200");
  delay(10000);
  //_____________________________________________________
  Serial.println("Received packet with RSSI -109");
  Serial.println("node 1");
  Serial.println("temp: "+suhu19);
  Serial.println("pH: "+ph19);
  Serial.println("tbd: "+kekeruhan19);
  Serial.println("count: 9");
  Serial.println("HTTP Response code (1): 200");

  delay(10000);
  
  //________________________________________________


  Serial.println("Received packet with RSSI -115");
  Serial.println("node 2");
  Serial.println("temp: "+suhu29);
  Serial.println("pH: "+ph29);
  Serial.println("tbd: "+kekeruhan29);
  Serial.println("count: 10");
  Serial.println("HTTP Response code (2): 200");

  delay(10000);
  //_____________________________________________________

  Serial.println("Received packet with RSSI -110");
  Serial.println("node 1");
  Serial.println("temp: "+suhu110);
  Serial.println("pH: "+ph110);
  Serial.println("tbd: "+kekeruhan110);
  Serial.println("count: 10");

  Serial.println("temp: "+suhu210);
  Serial.println("pH: "+ph210);
  Serial.println("tbd: "+kekeruhan210);
  Serial.println("count: 11");


  }
 
