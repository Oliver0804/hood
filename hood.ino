//test ok
#define MELEXISTEST_C
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#define HMI_Serial Serial1
#define ESP_Serial Serial2

#include <Arduino.h>
#include <Wire.h>
#include <MLX90614.h>
#include "MelexisTest.h"
#include "printf.h"
#include "define.h"

String mode = "";
int temp_1, temp_2, temp_3, temp_4;
int now_page = 0;
char ssid[16] = { '\0' };
char password[30] = { '\0' };
char ip[16] = { '\0' };
char hmiTemp[50] = { '\0' };
byte rx_buff[30] = { '\0' };
byte esp8266_buff[50] = { '\0' };
int esp8266_buff_count = 0;
int rx_buff_count = 1;
int wifi_connect = 0;
MLX90614 mlx = MLX90614(MLX90614_BROADCASTADDR);      // *** must be only one device on bus ***

/*********************************************************************************************/
/* PROGRAM SETUP                                                                             */
/*********************************************************************************************/

void setup(void) {

  Serial.begin(115200);//debug
  HMI_Serial.begin(9600);//HMI
  ESP_Serial.begin(115200);//ESP8266

  printf_begin();
  mlx.begin();
  GPIO_INIT();
  esp_rst();
  delay(3000);
  while (Serial2.available() > 0) {
    char s = (char)Serial2.read();
    Serial.print(s);
  }

  HMI_CMD("rest");
  HMI_CMD("rest");
  HMI_CMD("rest");
  delay(1000);
/*
  Serial2.print("PandoraBox_4ED15A$"); delay(100);
  Serial2.print("0982983012%"); delay(100);
  Serial2.print("^");
*/
  Serial.print("ok");
  delay(3000);
  while (ESP_Serial.available() > 0) {
    char s = (char)ESP_Serial.read();
    Serial.print(s);
  }
  //Serial2.print("&");
  
}

/*********************************************************************************************/
/* MAIN PROCESSING LOOP                                                                      */
/*********************************************************************************************/

void loop(void) {
HMI_TEMP_MAX();
  esp8266_buff_count = 0;
  while (ESP_Serial.available() > 0) {
    esp8266_buff[esp8266_buff_count] = (char)ESP_Serial.read();
    esp8266_buff[esp8266_buff_count + 1] = '\0';
    //Serial.print((char)esp8266_buff[esp8266_buff_count]);
    esp8266_buff_count++;
    for (int x = 0; x <= esp8266_buff_count; x++) {
      if (esp8266_buff[x] != '\0') {
        ip[x] = esp8266_buff[x];
      } else {
        /*
        Serial.print("IP:");
        sprintf(hmiTemp, "t_ip.txt=\"%s\"\0", ip);
        Serial.println(hmiTemp);
        HMI_CMD(hmiTemp);
        */
        break;
      }
    }
  }

  while (Serial.available() > 0) {
    char s = (char)Serial.read();
    //Serial.print(s);
    if (s == '?') {
      Serial.print("ssid : "); Serial.println(ssid);
      Serial.print("pawd : "); Serial.println(password);
    }
  }
  read_data2buff();//將讀出的溫度放入Buff
  HMI_SEND_TEMP();
  if (wifi_connect== 0) {
    sprintf(hmiTemp, "@%02d,%02d,%02d,%02d,", temp_1, temp_2, temp_3, temp_4);
    ESP_Serial.println(hmiTemp);
    delay(1000);
  }
  HMI_MSG();
}


