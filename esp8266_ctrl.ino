void esp_rst() {
  digitalWrite(esp8266_ret, LOW);
  delay(100);
  digitalWrite(esp8266_ret, HIGH);
  delay(1000);
}

void send_ssid() {
  int send_count = 0;
  ESP_Serial.write('$');
  while (1) {
    if (ssid[send_count] != '\0') {
      ESP_Serial.write((char)ssid[send_count]);
      send_count++;
    } else {
      ESP_Serial.write('$');
      break;
    }
  }
}


void send_password() {
  int send_count = 0;
  ESP_Serial.write('%');
  while (1) {
    if (password[send_count] != '\0') {
      ESP_Serial.write((char)password[send_count]);
      send_count++;
    } else {
      ESP_Serial.write('%');
      break;
    }
  }
}
