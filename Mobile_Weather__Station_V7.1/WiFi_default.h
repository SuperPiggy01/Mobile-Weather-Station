#include <WiFiNINA.h>
#define SECRET_SSID "THAC"//wifi name
#define SECRET_PASS "greatgiant538"//wifi password
#define SECRET_APIKEY1 "ef215954b56d2d38ec811b8bf2b72d27"
int keyIndex = 0;
int status = WL_IDLE_STATUS;
char server[] = "";
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
String apiKey1= SECRET_APIKEY1;
WiFiClient client;
char lcdBuffer[24];
String oledline[9];
//String location= "torino,IT";
String location= "fairfax,US";
char rssi[4], ip[20], MAC_Address[24], WiFi_Time[24], eqoch[20], savedata[20];
uint32_t ptr=0 ;
String readString, readString1;
String servRes = "";
String serverEpoch = "";
