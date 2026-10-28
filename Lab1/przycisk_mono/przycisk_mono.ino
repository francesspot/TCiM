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
  cbi(DDRB, PB0);
  sbi(PORTB, PB0);
  
  while(1){
    if(bit_is_clear(PINB, PB0)) {
      PORTD |= (1<<PD0);
     _delay_ms(50);
    } else {
      PORTD &= ~(1<<PD0);
    _delay_ms(50);
    }

  }
}
