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

#define   bit_is_set(sfr, bit)   (_SFR_BYTE(sfr) & _BV(bit))
#define   bit_is_clear(sfr, bit)   (!(_SFR_BYTE(sfr) & _BV(bit)))

int main() {
  LCD_I2C Lcd = LCD_I2C(0x27);
  _delay_ms(100);
  Lcd.backLightOn();

  cbi(DDRB, PB0);
  sbi(PORTB, PB0);
  cbi(DDRB, PB1); 
  sbi(PORTB, PB1);

  int8_t licznik = 0;
  char text[20];

  Lcd.clear();
  Lcd.writeText((char*)"Liczba: 0");

  while (1) {
    if (bit_is_clear(PINB, PB0)) { 
      _delay_ms(30);
      if (bit_is_clear(PINB, PB0)) {
        if (licznik < 100) licznik++;
        
        sprintf(text, "Liczba: %-4d", licznik);
        Lcd.goTo(0, 0);
        Lcd.writeText(text);

        while(bit_is_clear(PINB, PB0));
      }
    }

    if (bit_is_clear(PINB, PB1)) {
      _delay_ms(30);
      if (bit_is_clear(PINB, PB1)) {
        if (licznik > -100) licznik--;
        
        sprintf(text, "Liczba: %-4d", licznik);
        Lcd.goTo(0, 0);
        Lcd.writeText(text);

        while(bit_is_clear(PINB, PB1));
      }
    }
  }
}
