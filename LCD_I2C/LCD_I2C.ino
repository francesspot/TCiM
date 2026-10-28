#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "LCDI2C.h"

#ifndef _BV
#define _BV(bit)        (1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit)    reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)    reg &= ~(_BV(bit))
#endif

#ifndef tbi
#define tbi(reg,bit)    reg ^= (_BV(bit))
#endif



int main() {
  //0x27 - adres urzadzenia na I2C
  LCD_I2C Lcd = LCD_I2C(0x27);
  _delay_ms(100);
  Lcd.backLightOn();


  uint8_t i = 0;
  char text[20];
  while (1) {
    i = (i + 1) % 100;
    Lcd.clear();
    Lcd.writeText("Franciszek");
    Lcd.goTo(0, 1);
    Lcd.writeText("Krzysztof");
    Lcd.goTo(13, 1);
    sprintf(text, "%-3d", i);
    Lcd.writeText(text);
    _delay_ms(1000);
  }
}
