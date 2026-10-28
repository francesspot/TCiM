#include <IRremote.h>

#define IR_RECEIVE_PIN 3 

void setup() {
  Serial.begin(19200);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Gotowy do odbioru kodow IR...");
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.print("Odebrany kod (Command): 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX); 
    
    IrReceiver.resume(); 
  }
}
