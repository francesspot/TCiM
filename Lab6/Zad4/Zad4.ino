#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>

#define ONE_WIRE_BUS 2
#define LED_R 9
#define LED_G 10
#define LED_B 11

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
 
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  Serial.begin(19200);
  lcd.init();
  lcd.backlight(); 
  sensors.begin();
}

void setColor(int r, int g, int b) {
  analogWrite(LED_R, 255-r);
  analogWrite(LED_G, 255-g);
  analogWrite(LED_B, 255-b);
}

void loop() {
  int potValue = analogRead(A0);
  int tempC = map(potValue, 0, 1023, 20, 100);

  lcd.setCursor(0,0);
  lcd.print("Nagrzewanie");
  lcd.setCursor(0,1);
  lcd.print(tempC);
  lcd.print("C  ");


  if (tempC < 50) {
    setColor(0, 255, 0); // zimna
      lcd.print("zimna");
  } else if (tempC > 50 && tempC < 70) {
    setColor(0, 0, 255); // ciepła
      lcd.print("ciepla ");
  } else if (tempC > 70 && tempC < 80) {
    setColor(143, 0, 255); // cieplejsza
      lcd.print("goraca  ");
  } else if (tempC > 80 && tempC < 90) {
    setColor(255, 255, 0); // gorąca
      lcd.print("goretsza  ");
  } else if (tempC > 90) { 
    setColor(255, 0, 0);// wrzątek
      lcd.print("wrzatek  ");
  }
   delay(20);
}
