void HMI_CMD(char *p) {
  HMI_Serial.write(p);
  HMI_Serial.write(0xFF);
  HMI_Serial.write(0xFF);
  HMI_Serial.write(0xFF);
}
void HMI_TEMP_MAX() {
  if (temp_1 > set_max_temp || temp_2 > set_max_temp || temp_3 > set_max_temp || temp_4 > set_max_temp) {
    if (now_page == 0) {
      //Serial.print("TEMP So HIGH");
      HMI_CMD("page 1");
      now_page = 1;
      sprintf(hmiTemp, "@%02d,%02d,%02d,%02d,", temp_1, temp_2, temp_3, temp_4);
      ESP_Serial.println(hmiTemp);
      for (int x = 0; x < 10; x++) {
      HMI_SEND_TEMP();
        tone(Buzz, 770);
        delay(250);
        tone(Buzz, 440);
        delay(250);
      }
      noTone(Buzz);
    } else {
      HMI_CMD("page 0");
      now_page = 0;
    }
  } else if (temp_1 > set_warning_temp || temp_2 > set_warning_temp || temp_3 > set_warning_temp || temp_4 > set_warning_temp) {
    if (now_page == 0) {
      //Serial.print("TEMP So Warning");
      HMI_CMD("page 1");
      now_page = 1;
      sprintf(hmiTemp, "@%02d,%02d,%02d,%02d,", temp_1, temp_2, temp_3, temp_4);
      ESP_Serial.println(hmiTemp);
      for (int x = 0; x < 5; x++) {
      HMI_SEND_TEMP();
        tone(Buzz, 770);
        delay(500);
        tone(Buzz, 440);
        delay(500);
      }
      noTone(Buzz);
    } else {
      HMI_CMD("page 0");
      now_page = 0;
    }
  }
  else {
    if (now_page == 1) {
      HMI_CMD("page 0");
      now_page = 0;
    }
  }

}
void HMI_SEND_TEMP() {
  sprintf(hmiTemp, "n0.val=%02d\0", temp_1);
  HMI_CMD(hmiTemp);
  sprintf(hmiTemp, "n1.val=%02d\0", temp_2);
  HMI_CMD(hmiTemp);
  sprintf(hmiTemp, "n2.val=%02d\0", temp_3);
  HMI_CMD(hmiTemp);
  sprintf(hmiTemp, "n3.val=%02d\0", temp_4);
  HMI_CMD(hmiTemp);
}
void HMI_MSG() {
  while (HMI_Serial.available() > 0) {
    rx_buff[rx_buff_count] = (byte) HMI_Serial.read();
    //Serial.print(rx_buff[rx_buff_count], HEX);
    rx_buff_count++;

  }
  for (int n = 0; n < rx_buff_count; n++)
  {
    switch (rx_buff[n])
    {
      case HMI_VARIABLE_INVALID:
        //Serial.println("HMI_VARIABLE_INVALID");
        break;
      case HMI_VALUE_INVALID:
        Serial.println("HMI_VALUE_INVALID");
        break;
      case HMI_INVALID_CMD:
        break;
      case HMI_TOUCH_EVENT:
        if ((rx_buff[n + 4] == 0xff) && (rx_buff[n + 5] == 0xff) && (rx_buff[n + 6] == 0xff))
        {
          Serial.print("HMI_TOUCH_EVENT=");
          Serial.print(rx_buff[n + 1]); Serial.print(",");
          Serial.print(rx_buff[n + 2]); Serial.print(",");
          Serial.print(rx_buff[n + 3]); Serial.print(",");
          Serial.print(rx_buff[n + 7]);
          Serial.println();
          if (rx_buff[n + 1] == 0 && rx_buff[n + 2] == 5 ) {
            Serial.println("Set SSID");
            mode = "SSID";
            HMI_CMD("page 2");
            HMI_CMD("t1.txt=\"SSID\"");
          }
          if (rx_buff[n + 1] == 0 && rx_buff[n + 2] == 6 ) {
            Serial.println("Set PASSWORD");
            mode = "PASSWORD";
            HMI_CMD("page 2");
            HMI_CMD("t1.txt=\"PASSWORD\"");
          }
          if (rx_buff[n + 1] == 2 && rx_buff[n + 2] == 40 ) {
            Serial.println("ENTER");
            HMI_CMD("page 0");
            delay(100);
            if (mode == "SSID") {
              Serial.println("Read ssid");
              HMI_CMD("get page0.ssid.txt");
              //send_ssid();
            }
            else if (mode == "PASSWORD")
            {
              Serial.println("Read password");
              HMI_CMD("get page0.password.txt");
              //send_password();
            }
          }

        }
        if (rx_buff[n + 1] == 0 && rx_buff[n + 2] == 17 ) {
          Serial.println("Show IP");
          if (ssid != "" && password != "") {
            ESP_Serial.print("?");
            sprintf(hmiTemp, "t_ip.txt=\"%s\"\0", ip);
            Serial.println(hmiTemp);
            HMI_CMD(hmiTemp);
          } else {

          }

        }
        if (rx_buff[n + 1] == 0 && rx_buff[n + 2] == 21 ) {

          ESP_Serial.print("^");
          delay(1000);
          int count_test = 0;
          while (ESP_Serial.available() > 0) {
            if (ESP_Serial.read() != '.') {
              count_test++;
            }
            if (count_test >= 1) {
              wifi_connect = 1;
              break;
            }
          }
        }
        if (rx_buff[n + 1] == 0 && rx_buff[n + 2] == 1 ) {
          Serial.print("Top Light :");
          Serial.println(rx_buff[n + 7]);
          digitalWrite(Top_Light, !rx_buff[n + 7]);
        }
        if (rx_buff[n + 1] == 0 && rx_buff[n + 2] == 2 ) {
          Serial.print("Bottom_Light :");
          Serial.println(rx_buff[n + 7]);
          digitalWrite(Bottom_Light, ! rx_buff[n + 7]);
        }
        if (rx_buff[n + 1] == 0 && rx_buff[n + 2] == 12 ) {
          Serial.print("Fan  :");
          Serial.println(rx_buff[n + 7]);
          digitalWrite(Fan, !rx_buff[n + 7]);
        }
        if (rx_buff[n + 1] == 0 && rx_buff[n + 2] == 16 ) {
          Serial.print("Relay_4  :");
          Serial.println(rx_buff[n + 7]);
          digitalWrite(Relay_4, ! rx_buff[n + 7]);
        }

        if (rx_buff[n + 1] == 3 && rx_buff[n + 2] == 5 && rx_buff[n + 3] == 0 ) {
          ESP_Serial.println("*");
          while (ESP_Serial.available() > 0) {
            char s = (char)ESP_Serial.read();
            Serial.print(s);
          }
        }
        if (rx_buff[n + 1] == 3 && rx_buff[n + 2] == 1 && rx_buff[n + 3] == 0 ) {
          Serial.println("Select SSID 1");
        }
        if (rx_buff[n + 1] == 3 && rx_buff[n + 2] == 2 && rx_buff[n + 3] == 0) {
          Serial.println("Select SSID 2");
        }
        if (rx_buff[n + 1] == 3 && rx_buff[n + 2] == 3 && rx_buff[n + 3] == 0 ) {
          Serial.println("Select SSID 3");
        }
        if (rx_buff[n + 1] == 3 && rx_buff[n + 2] == 4 && rx_buff[n + 3] == 0 ) {
          Serial.println("Select SSID 4");
        }
        break;

      case HMI_PAGE_ID:
        if ((rx_buff[n + 2] == 0xFF) && (rx_buff[n + 3] == 0xFF) && (rx_buff[n + 4] == 0xFF)) {
          Serial.print("HMI_PAGE_ID=");
          Serial.print(rx_buff[n + 1]);
          Serial.println();
        }
        break;
      case HMI_TOUCH_XY:
        if ((rx_buff[n + 6] == 0xFF) && (rx_buff[n + 7] == 0xFF) && (rx_buff[n + 8] == 0xFF)) {
          Serial.print("HMI_TOUCH_XY=");
          Serial.print(rx_buff[n + 1]); Serial.print(",");
          Serial.print(rx_buff[n + 2]); Serial.print(",");
          Serial.print(rx_buff[n + 3]); Serial.print(",");
          Serial.print(rx_buff[n + 4]); Serial.print(",");
          Serial.print(rx_buff[n + 5]);
          Serial.println();

          n += 8;
        }
        break;
      case HMI_SLEEP_MODE_TOUCH:
        if ((rx_buff[n + 6] == 0xFF) && (rx_buff[n + 7] == 0xFF) && (rx_buff[n + 8] == 0xFF)) {
          Serial.print("HMI_SLEEP_MODE_TOUCH=");
          Serial.print(rx_buff[n + 1]); Serial.print(",");
          Serial.print(rx_buff[n + 2]); Serial.print(",");
          Serial.print(rx_buff[n + 3]); Serial.print(",");
          Serial.print(rx_buff[n + 4]); Serial.print(",");
          Serial.print(rx_buff[n + 5]);
          Serial.println();
          n += 8;
        }
        break;
      case HMI_TEXT:
        for (int x = 0; x < rx_buff_count; x++) {
          if ((rx_buff[x + 1] == 0xFF) && (rx_buff[x + 2] == 0xFF) && (rx_buff[x + 3] == 0xFF)) {
            Serial.print("Word:");
            Serial.println(x);
            if (mode == "SSID") {
              ssid[16] = { '\0' };
              for (int n = 0; n < x; n++) {
                ssid[n] = rx_buff[n + 1];
                //Serial.print(rx_buff[n+1]);
                ssid[n + 1] = '\0';
              }
              Serial.print("ssid:");

              for (int n = 0; n < 16; n++) {
                if (ssid[n] != '\0') {
                  Serial.print(ssid[n]);
                } else {
                  break;
                }
              }
              send_ssid();
            }
            else if (mode == "PASSWORD") {
              for (int n = 0; n < x; n++) {
                password[n] = rx_buff[n + 1];
                //Serial.print(rx_buff[n+1]);
                password[n + 1] = '\0';
              }
              Serial.print("password:");
              for (int n = 0; n < 16; n++) {
                if (password[n] != '\0') {
                  Serial.print(password[n]);
                } else {
                  break;
                }
              }
              send_password();
            }

            Serial.println();
            break;
          }
        }
        break;
      case HMI_VALUE:
        if ((rx_buff[n + 5] == 0xFF) && (rx_buff[n + 6] == 0xFF) && (rx_buff[n + 7] == 0xFF)) {
          Serial.print("HMI_VALUE=");
          long value = rx_buff[n + 1] + rx_buff[n + 2] * 256 + rx_buff[n + 3] * 256 * 256 + rx_buff[n + 3] * 256 * 256 * 256;
          Serial.println(value);
          n += 7;
        }
        break;
      case HMI_DEVICE_SLEEP:
        if ((rx_buff[n + 1] == 0xFF) && (rx_buff[n + 2] == 0xFF) && (rx_buff[n + 3] == 0xFF)) {
          Serial.println("HMI_DEVICE_SLEEP");
          n += 3;
        }
        break;
      case HMI_DEVICE_WAKE_UP:
        if ((rx_buff[n + 1] == 0xFF) && (rx_buff[n + 2] == 0xFF) && (rx_buff[n + 3] == 0xFF)) {
          Serial.println("HMI_DEVICE_WAKE_UP");
          n += 3;
        }
        break;
      case HMI_SYS_START_UP: Serial.println("HMI_SYS_START_UP"); break;
      case HMI_SD_CARD_UPDATE: Serial.println("HMI_SD_CARD_UPDATE"); break;
    }
  }
  rx_buff_count = 0;
}
