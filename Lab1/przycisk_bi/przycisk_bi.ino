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
  cbi(DDRB, PB1);
  sbi(PORTB, PB1);

  uint8_t diody_swieca = 0;
  
  while(1){
    if(bit_is_clear(PINB, PB1)) {
      _delay_ms(20);
      
      if (bit_is_clear(PINB, PB1)) {
        
        if (diody_swieca == 0) {
          PORTD = 0xFF;
          diody_swieca = 1;
        } else {
          PORTD = 0x00;
          diody_swieca = 0;
        }
         while (bit_is_clear(PINB, PB1)) { }
        _delay_ms(20);
      }
    }
  }
}
