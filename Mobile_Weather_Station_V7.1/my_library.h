#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include <Arduino.h>

int addTwoInts(int a, int b);
void convHHMMSS(unsigned long currSeconds, char *uptimeHHMMSS);
void convDDHHMMSS(unsigned long currSeconds, char *uptimeDDHHMMSS);
void convyyyyDDHHMMSS(unsigned long currSeconds, char *uptimeYYYYDDHHMMSS) ;
void convCurrentTimeET(unsigned long currSeconds, char *currentTimeET);
void scanWiFi(char *rssi, char *WiFi_MAC_Address,char *WiFi_Time,char *WiFi_LocalIP);
void DHT22(float *T, float *H);
void IMU_begin(char *AX,char *AY,char *AZ,char *GX,char *GY,char *GZ,char *AXAYAZ,char *GXGYGZ);
void getforecast(String text, char* L_0_d_txt, float *list_0_main_temp, int *list_0_main_humidity,char* L_0_W_D, char* L_1_d_txt, float *list_1_main_temp, int *list_1_main_humidity, char* L_1_W_D);
void getweather(String text,float *coord_lon,float *coord_lat,float *main_temp, int *main_humidity, char *L_0_D, char * L_city , char *L_country);
#endif
