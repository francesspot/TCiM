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
}

void setColor(int r, int g, int b) {
  analogWrite(LED_R, 255-r);
  analogWrite(LED_G, 255-g);
  analogWrite(LED_B, 255-b);
}

void loop() {
//  // Czerwony
  setColor(255, 0, 0);
  delay(500);

//  // Pomarańczowy
  setColor(255, 165, 0);
  delay(500);

//  // Żółty
  setColor(255, 255, 0);
  delay(500);

  // Zielony
  setColor(0, 128, 0);
  delay(500);

  //Niebieski
  setColor(0, 0, 255);
  delay(500);
  
  //Indigo
  setColor(75, 0, 130);
  delay(500);

 // Fioletowy
  setColor(143, 0, 255);
  delay(500);
}
