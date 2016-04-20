
void GPIO_INIT() {
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(Buzz, OUTPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  digitalWrite(Buzz, LOW);
  
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(Top_Light, OUTPUT);
  pinMode(Bottom_Light, OUTPUT);
  pinMode(Fan, OUTPUT);
  pinMode(Relay_4, OUTPUT);

  digitalWrite(Top_Light, HIGH);
  digitalWrite(Bottom_Light, HIGH);
  digitalWrite(Fan, HIGH);
  digitalWrite(Relay_4, HIGH);

  pinMode(esp8266_ret, OUTPUT);
  digitalWrite(esp8266_ret, HIGH);
}

void read_1() {
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
}
void read_2() {
  digitalWrite(s0, LOW);
  digitalWrite(s1, HIGH);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
}
void read_3() {
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
}
void read_4() {
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, HIGH);
  digitalWrite(s3, LOW);
}
