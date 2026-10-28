#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
 
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
  DDRD = 0xFF;
  PORTD = 0x00;

  cbi(DDRB, PB0);
  sbi(PORTB, PB0);

  cbi(DDRB, PB1);
  sbi(PORTB, PB1);

 uint8_t SW2_swieci = 0;
 
  while (1) {
    // SW1
    if (bit_is_clear(PINB, PB0)) {
      sbi(PORTD, PD0);
    } else {
      cbi(PORTD, PD0);
    }

    // SW2
    if (bit_is_clear(PINB, PB1)) {
      _delay_ms(20);

      if (bit_is_clear(PINB, PB1)) {
        
        if (SW2_swieci == 0) {
          PORTD |= (1<<PD1);
          SW2_swieci = 1;
        } else {
          PORTD &= ~(1<<PD1);
          SW2_swieci = 0;
        }
        while (bit_is_clear(PINB, PB1)) { }
        _delay_ms(20);
      }
    }

}
}
