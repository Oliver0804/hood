
void read_data2buff() {
  read_1();
  delay(10);
  temp_1 = mlx.convKtoC(mlx.readTemp(MLX90614::MLX90614_SRCO1, MLX90614::MLX90614_TK));
  read_2();
  delay(10);
  temp_2 = mlx.convKtoC(mlx.readTemp(MLX90614::MLX90614_SRCO1, MLX90614::MLX90614_TK));
  read_3();
  delay(10);
  temp_3 = mlx.convKtoC(mlx.readTemp(MLX90614::MLX90614_SRCO1, MLX90614::MLX90614_TK));
  read_4();
  delay(10);
  temp_4 = mlx.convKtoC(mlx.readTemp(MLX90614::MLX90614_SRCO1, MLX90614::MLX90614_TK));
  delay(10);
}

void printlnTemp(double temp, char src) {
  char str[20];

  if (mlx.rwError) Serial.print(F("No valid temperatures                              "));
  else {
    if (src == 'A') Serial.print(F("Ambient temperature"));
    else Serial.print(F("Object  temperature"));
    printf(" = %sK ", floatToStr(str, temp));
    printf("%sC ",    floatToStr(str, mlx.convKtoC(temp)));
    printf("%sF    ", floatToStr(str, mlx.convCtoF(mlx.convKtoC(temp))));
  }
  printCRC(mlx.crc8, mlx.pec);
  printErrStr(mlx.rwError);
  Serial.println("");
}

void dumpEEProm() {

  Serial.println(F("EEProm Dump"));
  for (uint8_t j = 0; j < 8; j++) {
    for (uint8_t i = 0; i < 4; i++) printf("%02Xh-%04Xh    ", j * 4 + i, mlx.readEEProm(j * 4 + i));
    printCRC(mlx.crc8, mlx.pec);
    printErrStr(mlx.rwError);
    Serial.println("");
  }
}

// Utility to stringify a float
char* floatToStr(char *str, double val) {

  sprintf(str, "%4d.%02u", int(val), int(val * 100) % 100);
  return str;
}

// Just print the crc and pec
void printCRC(uint8_t crc, uint8_t pec) {
  printf("crc=%02Xh pec=%02Xh", crc, pec);
}

// Convert error flags to diagnostic strings and print
void printErrStr(uint8_t err) {

  Serial.print(F("  *** "));
  if (err == MLX90614_SUCCESS) Serial.print(F("RW Success"));
  else {
    Serial.print(F("Errors: "));
    if (err &  MLX90614_DATATOOLONG) Serial.print(F("Data too long / "));
    if (err &  MLX90614_TXADDRNACK)  Serial.print(F("TX addr NACK / "));
    if (err &  MLX90614_TXDATANACK)  Serial.print(F("TX data NACK / "));
    if (err &  MLX90614_TXOTHER)     Serial.print(F("Unknown / "));
    if (err &  MLX90614_RXCRC)       Serial.print(F("RX CRC / "));
    if (err &  MLX90614_INVALIDATA)  Serial.print(F("Invalid data / "));
    if (err &  MLX90614_EECORRUPT)   Serial.print(F("EEPROM / "));
    if (err &  MLX90614_RFLGERR)     Serial.print(F("RFlags / "));
  }
}

// Set EEPROM memory contents to factory default values.
// A device with default adress must not be on the bus.
// Only user allowed memory locations are written.

// Default EEPROM data
const struct defaultEEPromData {
  uint8_t  address;
  uint16_t data;
} eDat[] =  {{0x20, 0x9993}, {0x21, 0x62E3}, {0x22, 0x0201},
  {0x23, 0xF71C}, {0x24, 0xFFFF}, {0x25, 0x9FB4},
  {0x2E, 0xBE5A}, {0x2F, 0x0000}, {0x39, 0x0000}
};

void setEEPromDefaults(void) {

  for (uint8_t i = 0; i < sizeof(eDat) / sizeof(defaultEEPromData),
       !mlx.rwError; i++) {
    mlx.writeEEProm(eDat[i].address, eDat[i].data);
  }
}
