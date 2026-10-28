#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  Serial.begin(19200); 
  
  lcd.init();
  lcd.backlight();
  sensors.begin();
  
  lcd.setCursor(0, 0);
  lcd.print("Czekam na temp.");
}

void loop() {
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0); 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print(" C");

  Serial.println(tempC);

  delay(1000); 
}
