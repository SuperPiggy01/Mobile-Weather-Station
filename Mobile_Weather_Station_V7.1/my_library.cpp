#include "my_library.h"
#include <WiFiNINA.h>
#include <time.h>
#include <SimpleDHT.h>
#include <ArduinoJson.h>
//===========================================================================================================
//mathematics
//===========================================================================================================
int addTwoInts(int a, int b)
{
  return a + b;
};
//===========================================================================================================
//Time
//===========================================================================================================
void convHHMMSS(unsigned long currSeconds, char *uptimeHHMMSS) 
{
  int hh, mm, ss;
  hh = (currSeconds/3600);
  mm = (currSeconds -(3600*hh))/60;
  ss = (currSeconds -(3600*hh)-(mm*60));
  sprintf(uptimeHHMMSS, "Uptime: %02d:%02d:%02d", hh, mm, ss);
  //sprintf(uptimeDDHHMMSS, "Uptime: %.0f: %02.0f:%02.0f:%02.0f", floor(currSeconds/86400.0), floor(currSeconds/3600.0), floor(fmod(currSeconds,3600.0)/60.0), fmod(currSeconds,60.0));
};

void convDDHHMMSS(unsigned long currSeconds, char *uptimeDDHHMMSS) 
{
  unsigned long ss;
  long yyyy;
  int dd;
  byte hh,mm;
  ss = currSeconds;
  dd = ss / 86400;
  ss %= 86400;
  hh = ss / 3600;
  ss %= 3600;
  mm = ss / 60;
  ss %= 60;
  sprintf(uptimeDDHHMMSS, "Uptime: %02d:%02d:%02d:%02d",dd, hh, mm, ss);
}

void convYYYYDDHHMMSS(unsigned long currSeconds, char *uptimeYYYYDDHHMMSS) 
{
  unsigned long ss;
  long yyyy;
  int dd;
  byte hh,mm;
  ss = currSeconds;
  yyyy = ss/31536000;
  ss %= 31536000;
  dd = ss / 86400;
  ss %= 86400;
  hh = ss / 3600;
  ss %= 3600;
  mm = ss / 60;
  ss %= 60;
  sprintf(uptimeYYYYDDHHMMSS, "Uptime: %04d:%03d:%02d:%02d:%02d", yyyy, dd, hh, mm, ss);
  //sprintf(uptimeDDHHMMSS, "Uptime: %.0f %02.0f:%02.0f:%02.0f", floor(currSeconds/86400.0), floor(currSeconds/3600.0), floor(fmod(currSeconds,3600.0)/60.0), fmod(currSeconds,60.0));

}
void convCurrentTimeET(unsigned long currSeconds, char *currentTimeET) 
{
    time_t rawtime = currSeconds - 14400;
    struct tm  ts;
    char buf[70];
    ts = *localtime(&rawtime);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
    sprintf(currentTimeET, buf);
}
//===========================================================================================================
//Function-WiFi
//===========================================================================================================
void scanWiFi(char *Rssi, char *WiFi_MAC_Address,char *WiFi_Time,char *WiFi_LocalIP) {// print the SSID of the network you're attached to:
byte mac[6];
int rssi;
//Serial.print("SSID: ");
//Serial.println(WiFi.SSID());
IPAddress ip = WiFi.localIP();
sprintf(WiFi_LocalIP, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
//Serial.print("IP Address: ");
//Serial.println(WiFi_LocalIP);// print your WiFi shield's IP address:
rssi = WiFi.RSSI();
//Serial.print(" signal strength (RSSI):");// print the received signal strength:
sprintf(Rssi,"%d",rssi);
//Serial.print(Rssi);
//Serial.println(" dBm");
WiFi.macAddress(mac);
//Serial.print(" macAddress: ");
sprintf(WiFi_MAC_Address,"MAC %02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
//Serial.println(WiFi_MAC_Address);
//Serial.print(" WiFi_Time: ");
convCurrentTimeET(WiFi.getTime(), WiFi_Time);
//Serial.println(WiFi_Time);
//Serial.println("");
}
//===========================================================================================================
//Function-other
//===========================================================================================================
void DHT22(float *T, float *H)
{ int pinDHT22 = 2;
  SimpleDHT22 dht22(pinDHT22);
  float temperature = 0;
  float humidity = 0;
  int errDHT22 = SimpleDHTErrSuccess;
  errDHT22 = dht22.read2(&temperature, &humidity, NULL);
  if (errDHT22 != 0) {
    temperature = (-1 - 32)/1.8;
    humidity = -1;
  }
  char temp[5], humi[5];
  //temperature = 1.8*temperature + 32;
  sprintf(temp, "%.2f", (float)temperature);
  sprintf(humi, "%.2f", (float)humidity);
  *T = atof(temp);
  *H = atof(humi);
  //Serial.println("================");
  //Serial.println(*T);
  //Serial.println(*H);
  //Serial.println("");
}
//===========================================================================================================
//Json
//===========================================================================================================
void getweather(String text,float *coord_lon,float *coord_lat,float *main_temp, int *main_humidity, char *L_0_D, char * L_city , char *L_country){

       int text_len = text.length() + 1;
       char text_c[text_len];
       text.toCharArray (text_c, text_len);
       
      StaticJsonDocument<768> doc;
      deserializeJson(doc, text_c, text_len);

       *coord_lon = doc["coord"]["lon"]; // -77.3497
       *coord_lat = doc["coord"]["lat"]; // 38.8501

      JsonObject weather_0 = doc["weather"][0];
      int weather_0_id = weather_0["id"]; // 803
      const char* weather_0_main = weather_0["main"]; // "Clouds"
      const char* weather_0_description = weather_0["description"]; // "broken clouds"
      strcpy( L_0_D, (const char*)weather_0_description);
      const char* weather_0_icon = weather_0["icon"]; // "04d"

      const char* base = doc["base"]; // "stations"

      JsonObject main = doc["main"];
           *main_temp = main["temp"]; // 21.88
      float main_feels_like = main["feels_like"]; // 20.75
      float main_temp_min = main["temp_min"]; // 21.11
      float main_temp_max = main["temp_max"]; // 22.78
      int main_pressure = main["pressure"]; // 1015
          *main_humidity = main["humidity"]; // 24

      int visibility = doc["visibility"]; // 10000

      float wind_speed = doc["wind"]["speed"]; // 3.6
      int wind_deg = doc["wind"]["deg"]; // 320

      int clouds_all = doc["clouds"]["all"]; // 75

      long dt = doc["dt"]; // 1617657363

      JsonObject sys = doc["sys"];
      int sys_type = sys["type"]; // 1
      int sys_id = sys["id"]; // 4481
      const char* sys_country = sys["country"]; // "US"
      strcpy( L_country, (const char*)sys_country);
      long sys_sunrise = sys["sunrise"]; // 1617619638
      long sys_sunset = sys["sunset"]; // 1617665778

      int timezone = doc["timezone"]; // -14400
      long id = doc["id"]; // 4758041
      const char* name = doc["name"]; // "Fairfax"
      strcpy( L_city, (const char*)name);
      int cod = doc["cod"]; // 200

  }
void getforecast(String text, char* L_0_d_txt, float *list_0_main_temp, int *list_0_main_humidity,char* L_0_W_D, char* L_1_d_txt, float *list_1_main_temp, int *list_1_main_humidity, char* L_1_W_D){

       int text_len = text.length() + 1;
       char text_c[text_len];
       text.toCharArray (text_c, text_len);
       
      StaticJsonDocument<1024> doc;
      deserializeJson(doc, text_c, text_len);
  
const char* cod = doc["cod"]; // "200"
int message = doc["message"]; // 0
int cnt = doc["cnt"]; // 11

JsonArray list = doc["list"];

JsonObject list_0 = list[0];
long list_0_dt = list_0["dt"]; // 1617775200

JsonObject list_0_main = list_0["main"];
      *list_0_main_temp = list_0_main["temp"]; // 13.28
float list_0_main_feels_like = list_0_main["feels_like"]; // 12.67
float list_0_main_temp_min = list_0_main["temp_min"]; // 13.27
float list_0_main_temp_max = list_0_main["temp_max"]; // 13.28
int list_0_main_pressure = list_0_main["pressure"]; // 1015
int list_0_main_sea_level = list_0_main["sea_level"]; // 1015
int list_0_main_grnd_level = list_0_main["grnd_level"]; // 1000
    *list_0_main_humidity = list_0_main["humidity"]; // 77
float list_0_main_temp_kf = list_0_main["temp_kf"]; // 0.01

JsonObject list_0_weather_0 = list_0["weather"][0];
int list_0_weather_0_id = list_0_weather_0["id"]; // 500
const char* list_0_weather_0_main = list_0_weather_0["main"]; // "Rain"
const char* list_0_weather_0_description = list_0_weather_0["description"]; // "light rain"
strcpy( L_0_W_D, (const char*)list_0_weather_0_description);
const char* list_0_weather_0_icon = list_0_weather_0["icon"]; // "10n"

int list_0_clouds_all = list_0["clouds"]["all"]; // 31

float list_0_wind_speed = list_0["wind"]["speed"]; // 1.02
int list_0_wind_deg = list_0["wind"]["deg"]; // 78

int list_0_visibility = list_0["visibility"]; // 10000
float list_0_pop = list_0["pop"]; // 0.48

float list_0_rain_3h = list_0["rain"]["3h"]; // 0.47

const char* list_0_sys_pod = list_0["sys"]["pod"]; // "n"

const char* list_0_dt_txt = list_0["dt_txt"]; // "2021-04-07 06:00:00"
strcpy( L_0_d_txt, (const char*)list_0_dt_txt);

JsonObject list_1 = list[1];
long list_1_dt = list_1["dt"]; // 1617786000

JsonObject list_1_main = list_1["main"];
      *list_1_main_temp = list_1_main["temp"]; // 12.38
float list_1_main_feels_like = list_1_main["feels_like"]; // 11.87
float list_1_main_temp_min = list_1_main["temp_min"]; // 12.13
float list_1_main_temp_max = list_1_main["temp_max"]; // 12.38
int list_1_main_pressure = list_1_main["pressure"]; // 1015
int list_1_main_sea_level = list_1_main["sea_level"]; // 1015
int list_1_main_grnd_level = list_1_main["grnd_level"]; // 1000
    *list_1_main_humidity = list_1_main["humidity"]; // 84
float list_1_main_temp_kf = list_1_main["temp_kf"]; // 0.25

JsonObject list_1_weather_0 = list_1["weather"][0];
int list_1_weather_0_id = list_1_weather_0["id"]; // 801
const char* list_1_weather_0_main = list_1_weather_0["main"]; // "Clouds"
const char* list_1_weather_0_description = list_1_weather_0["description"]; // "few clouds"
strcpy( L_1_W_D, (const char*)list_1_weather_0_description);
const char* list_1_weather_0_icon = list_1_weather_0["icon"]; // "02n"

int list_1_clouds_all = list_1["clouds"]["all"]; // 22

float list_1_wind_speed = list_1["wind"]["speed"]; // 0.77
int list_1_wind_deg = list_1["wind"]["deg"]; // 130

int list_1_visibility = list_1["visibility"]; // 10000
float list_1_pop = list_1["pop"]; // 0.07

const char* list_1_sys_pod = list_1["sys"]["pod"]; // "n"

const char* list_1_dt_txt = list_1["dt_txt"]; // "2021-04-07 09:00:00"
strcpy( L_1_d_txt, (const char*)list_1_dt_txt);


JsonObject city = doc["city"];
long city_id = city["id"]; // 4758041
const char* city_name = city["name"]; // "Fairfax"

float city_coord_lat = city["coord"]["lat"]; // 38.8501
float city_coord_lon = city["coord"]["lon"]; // -77.3497

const char* city_country = city["country"]; // "US"
long city_population = city["population"]; // 1081726
int city_timezone = city["timezone"]; // -14400
long city_sunrise = city["sunrise"]; // 1617792255
long city_sunset = city["sunset"]; // 1617838692
  
  }
