#include <stdio.h>
#include <ArduinoJson.h>
#include "my_library.h"
#include "WiFi_default.h"
#include "oled_default.h"
#include <SPI.h>
#include <Wire.h>
#include "Json_default.h"
#include "Firebase_default.h"
#include "GPS_default.h"

String output2Cloud = "";
String p_output2Cloud = "";
String cleanUp = "";

unsigned long startMillis;
unsigned long currentMillis;
int Display_mode = 4;
char c;
float tempDHT22, humiDHT22;
char CMD = 0;
float receive_DATA,DATA = 0;


class Oled{
  public:
    int Display;
    //Oled(){}
    
    void oled_setup()
      { // Make sure to use the correct I2C address. Address 0x3C for 128x64
      // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
      if(!myOled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("SSD1306 allocation failed");
        for(;;);} // Don't proceed, loop forever
      }
    void displayTextOLED(){
      int jj;
      myOled.clearDisplay();
      myOled.setTextSize(1);
      myOled.setTextColor(SSD1306_WHITE);
      myOled.setCursor(0, 0);
      for (jj=1; jj<=8; jj++) { 
        myOled.println(oledline[jj]);
        }
      myOled.display();
      }
     void show(){ 
          //*************************************//
          //*** Start OLED Display **************//
          //*************************************//
        if (currentMillis - startMillis >= 1000 && Display == 1) //Display every second
          { 
            oledline[1] = "Weather Forecast";
            oledline[2] = "by using IoT & iCloud";
            oledline[3] = "";
            oledline[4] = "";
            oledline[5] = "";
            oledline[6] = "";
            oledline[7] = "Author:";
            oledline[8] = "Henry Chen";
        
            //displayTextOLED(oledline);
            displayTextOLED();
            startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
          //
          //*************************************//
          //***  End OLED Display ***************//
          //*************************************//
          }
           if (currentMillis - startMillis >= 1000 && Display == 2) //Display every second
          { 
            oledline[1] = "DHT22:";
            oledline[2] = "Temp in C: " + String(tempDHT22) + " C";
            oledline[3] = "Humidity: " + String(humiDHT22) + " RH%";
            oledline[4] = "=====================";
            oledline[5] = "Open Weather: " + String(city);
            oledline[6] = "Temp: " + String(temperture) + " C";
            oledline[7] = "Humi: " + String(humidity) + " %";
            oledline[8] = "Desc.:" + String(weatherDescription);
            displayTextOLED();
            startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
            //servRes = "";
          //
          //*************************************//
          //***  End OLED Display ***************//
          //*************************************//
          }
          if (currentMillis - startMillis >= 1000 && Display == 3) //Display every second
          { 
            oledline[1] = String(locationForecastP3Time);
            oledline[2] = "Temp:" + String(locationForecastP3Temp) + "C,Humi:" + String(locationForecastP3Humi)+"%";
            oledline[3] = "Desc.:" + String(locationForecastP3Descrip);
            oledline[4] = "";
            oledline[5] = "=====================";
            oledline[6] = String(locationForecastP6Time);
            oledline[7] = "Temp:" + String(locationForecastP6Temp) + "C,Humi:" + String(locationForecastP6Humi)+"%";
            oledline[8] = "Desc.:"+ String(locationForecastP6Descrip);
        
            displayTextOLED();
            startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
          //
          //*************************************//
          //***  End OLED Display ***************//
          //*************************************//
          }
          if (currentMillis - startMillis >= 1000 && Display == 4) //Display every second
          { 
            oledline[1] = "GPS Location";
            oledline[2] = "lat:" + String(cur_lat,5);
            oledline[3] = "lon:" + String(cur_lng,5);
            oledline[4] = "";
            oledline[5] = "";
            oledline[6] = "";
            oledline[7] = "Date:" + String(Date);
            oledline[8] = "Time:" + String(Time); 
        
            displayTextOLED();
            startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
          //
          //*************************************//
          //***  End OLED Display ***************//
          //*************************************//
          }
     }  
};
Oled OledcennectCenter;

class CtoS{
  public:
    void WifiSetup() 
        {// check for the WiFi module:
        if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        while (true);}// don't continue
        oledline[1] = "Connect to Wifi..."; 
        OledcennectCenter.displayTextOLED();
        while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        delay(500);}// wait 1 seconds for connection:
        Serial.println("Connected to wifi");
        scanWiFi(rssi,MAC_Address,WiFi_Time,ip);
        Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
        Firebase.reconnectWiFi(true);
        }      
    void setCtoWeather(){
      Serial.println("\nStarting connection to server...");
        char server[] = "api.openweathermap.org";
        if (client.connect(server, 80)) {
        Serial.println("connected to server");
        //Make a HTTP request:
        client.print("GET /data/2.5/weather?");
        client.print("q="+location);
        client.print("&appid="+apiKey1);
        //client.print("&cnt=1");
        client.println("&units=metric");
        client.println("Host: "+ String(server));
        client.println("Connection: close");
        client.println();
        }}
      void setCtoForcast(){
        Serial.println("\nStarting connection to server...");
        char server[] = "api.openweathermap.org";
        if (client.connect(server, 80)) {
        Serial.println("connected to server");
        //Make a HTTP request:
        client.print("GET /data/2.5/forecast?");
        client.print("q="+location);
        client.print("&appid="+apiKey1);
        client.print("&cnt=2");
        client.println("&units=metric");
        client.println("Host: "+ String(server));
        client.println("Connection: close");
        client.println();
        }}
      void sendToFirebase(){
        if (Firebase.setJSON(firebaseData, "/IoT33/Received", output2Cloud)){
            Serial.println("PASSED");
            Serial.println("PATH: " + firebaseData.dataPath());
            Serial.print("PUSH NAME: ");
            Serial.println(firebaseData.pushName());
            Serial.println("------------------------------------");
            Serial.println();
          }
          else{
            Serial.println("FAILED");
            Serial.println("REASON: " + firebaseData.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
          }
          if (Firebase.pushJSON(firebaseData, "/IoT33/Record", output2Cloud)){
            //Serial.println("PASSED");
            //Serial.println("PATH: " + firebaseData.dataPath());
            //Serial.print("PUSH NAME: ");
            //Serial.println(firebaseData.pushName());
            //Serial.println("------------------------------------");
            //Serial.println();
          }
          else{
            Serial.println("FAILED");
            Serial.println("REASON: " + firebaseData.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
          }
   
}
};
CtoS WiFicennectCenter;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  OledcennectCenter.oled_setup();
  WiFicennectCenter.WifiSetup();
  startMillis = millis();  //initial start time
}

void loop() {

    currentMillis = millis();
    scanWiFi(rssi,MAC_Address,WiFi_Time,ip);
    sprintf(Date,"%c%c%c%c%c%c%c%c%c%c", WiFi_Time[0], WiFi_Time[1], WiFi_Time[2], WiFi_Time[3],WiFi_Time[4], WiFi_Time[5], WiFi_Time[6], WiFi_Time[7], WiFi_Time[8], WiFi_Time[9]);
    sprintf(Time,"%c%c%c%c%c%c%c%c%c%c", WiFi_Time[11], WiFi_Time[12], WiFi_Time[13],WiFi_Time[14], WiFi_Time[15], WiFi_Time[16], WiFi_Time[17], WiFi_Time[18], WiFi_Time[19]);
    DHT22( &tempDHT22, &humiDHT22);
    GPS();
    
    while (Serial.available() > 0){
      CMD = Serial.read();
      receive_DATA = Serial.parseFloat();

      if (Serial.read() == '\n') {
        Serial.println("Received...CMD:" + String(CMD) + " Data:" + String(receive_DATA));
        }

    if (CMD == 'p') {
       Display_mode = receive_DATA;}
    else {
      Serial.println("CMD error");
      }
  } 


    WiFicennectCenter.setCtoWeather();
    delay(200);
    while(client.available()) 
    { c = client.read();
     //Serial.write(c);
      servRes += c;}
      //delay(1000);

      //Serial.println(servRes);
      getweather(servRes, &lo, &la, &temperture, &humidity, weatherDescription, city, country);
      servRes = cleanUp;
    
    WiFicennectCenter.setCtoForcast();
    delay(200);
    while(client.available()) 
    { c = client.read();
     //Serial.write(c);
      servRes += c;}
     //Serial.println(servRes);
     getforecast(servRes, locationForecastP3Time, &locationForecastP3Temp, &locationForecastP3Humi, locationForecastP3Descrip, locationForecastP6Time, &locationForecastP6Temp, &locationForecastP6Humi, locationForecastP6Descrip);
     servRes = cleanUp;
  
  if (!client.connected()) {
  Serial.println();
  Serial.println("disconnecting from server.");
  }
  
    OledcennectCenter.Display = Display_mode;
    OledcennectCenter.show();
    
    
    combineData2Json();
    if (p_output2Cloud != output2Cloud){
    //============================================= 
    //CLouddataupload place
    WiFicennectCenter.sendToFirebase();
    }
   //============================================== 
    p_output2Cloud = output2Cloud; 
    output2Cloud = cleanUp;
      
}

void combineData2Json(){
   StaticJsonDocument<400> doc;
   doc["country"] = String(country);
   doc["city"] = String(city);
   doc["Weather Description"] = String(weatherDescription);
   doc["temperture"] = tempDHT22;
   doc["humidity"] = humiDHT22;
   doc["GPS"]["lat"] = cur_lat;
   doc["GPS"]["lon"] = cur_lng;
   doc["Date"] = Date;
   doc["Time"] = Time;
   //serializeJson(doc, Serial);
   serializeJson(doc, output2Cloud);
   //Serial.println();
   //serializeJsonPretty(doc, Serial);  
   //Serial.println(output2Cloud);
}
//===========================================================================================================
//GPS
//===========================================================================================================
void GPS(){
  Serial1.begin(9600); 
  if(gps.location.isValid()>0)
  {cur_lat = gps.location.lat();
   cur_lng = gps.location.lng();
  //Serial.println(cur_lat,5);
  //Serial.println(cur_lng,5);
  }
  smartDelay(1000);
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}
