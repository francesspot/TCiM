void setup() {
 Serial.begin(19200);

}

void loop() {
  int rawValue = analogRead(A0);
  float voltage = rawValue * (5.0 / 1023.0);
  Serial.println(voltage);
  delay(50);

}
