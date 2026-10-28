#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>

#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
 
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight(); 
  sensors.begin();

  lcd.setCursor(0,0);
  lcd.print("Termometr:");
}

void loop() {
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  lcd.setCursor(0, 1);
  lcd.print(tempC);
  lcd.print(" C ");
  delay(1000);
}
