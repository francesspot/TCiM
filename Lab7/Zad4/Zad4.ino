#include <LiquidCrystal_I2C.h> 
#include <OneWire.h> 
#include <DallasTemperature.h> 
#include <IRremote.h>

#define ONE_WIRE_BUS 2    
#define IR_RECEIVE_PIN 3  
#define PHOTO_PIN A0      // Fotorezystor (ADC0)
#define LED_COOLING 9     
#define LED_EXTRA 11      

LiquidCrystal_I2C lcd(0x27, 16, 2); 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire); 

// KODY PILOTA IR
#define IR_PWR        0x45 
#define IR_TEMP_UP    0x40 
#define IR_TEMP_DN    0x19 
#define IR_LIGHT_UP   0x09
#define IR_LIGHT_DN   0x07 
#define IR_EXTRA_LED  0x15 

bool trybCzuwania = true;
float tempRzeczywista = 0.0;
float tempZadana = 22.0;    
int jasnosc = 0;
int progJasnosc = 500;      
unsigned long lastUpdate = 0;

void setup() {
  sensors.begin(); 
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  
  lcd.init(); 
  lcd.backlight(); 
  
  pinMode(LED_COOLING, OUTPUT);
  pinMode(LED_EXTRA, OUTPUT);
  
  uruchomTrybCzuwania();
}

void uruchomTrybCzuwania() {
  trybCzuwania = true;
  digitalWrite(LED_COOLING, LOW);
  digitalWrite(LED_EXTRA, LOW);
  lcd.clear();
  lcd.noBacklight();
}

void loop() {
  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    
    if (trybCzuwania) {
      if (command == IR_PWR) {
        trybCzuwania = false;
        lcd.backlight();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Witaj!");
        delay(3000);
        lcd.clear();
      }
    } else {
      if (command == IR_PWR) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zakonczono prace");
        delay(2000);
        uruchomTrybCzuwania();
      } else if (command == IR_TEMP_UP) {
        tempZadana += 0.5;
      } else if (command == IR_TEMP_DN) {
        tempZadana -= 0.5;
      } else if (command == IR_LIGHT_UP) {
        progJasnosc += 50;
      } else if (command == IR_LIGHT_DN) {
        progJasnosc -= 50;
      } else if (command == IR_EXTRA_LED) {
        digitalWrite(LED_EXTRA, !digitalRead(LED_EXTRA));
      }
    }
    IrReceiver.resume();
  }

  if (!trybCzuwania) {
    if (millis() - lastUpdate >= 1000) {
      sensors.requestTemperatures();
      tempRzeczywista = sensors.getTempCByIndex(0);
      jasnosc = analogRead(PHOTO_PIN); 
      lastUpdate = millis();

      lcd.setCursor(0, 0); 
      lcd.print("T:"); lcd.print(tempRzeczywista, 1);
      lcd.print(" Z:"); lcd.print(tempZadana, 1);

      lcd.setCursor(0, 1); 
      if (jasnosc < progJasnosc) {
        lcd.print("Zbyt ciemno-STOP"); 
        digitalWrite(LED_COOLING, LOW); 
      } else {
        lcd.print("Sw:"); lcd.print(jasnosc);
        lcd.print(" Pr:"); lcd.print(progJasnosc);
        lcd.print("   "); 
        
        if (tempRzeczywista > tempZadana) {
          digitalWrite(LED_COOLING, HIGH);
        } else if (tempRzeczywista <= tempZadana - 0.5) {
          digitalWrite(LED_COOLING, LOW);
        }
      }
    }
  }
}