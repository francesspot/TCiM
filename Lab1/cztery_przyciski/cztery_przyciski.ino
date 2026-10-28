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

  DDRB &= ~(_BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3)); 
  PORTB |= (_BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3));

  uint8_t liczba_sw_diod = 0;
  bool sw1_wcisniete = false;
  bool sw2_wcisniete = false;
  bool sw3_wcisniete = false;
  bool sw4_wcisniete = false;
  
  while (1) {
    // SW1 (PB0) - Zmniejszenie liczby diod
    if (bit_is_clear(PINB, PB0)) {
      if (!sw1_wcisniete) {
        _delay_ms(20);
        if (liczba_sw_diod > 0) {
          liczba_sw_diod--;
          cbi(PORTD, liczba_sw_diod);
        }
        sw1_wcisniete = true;
      }
    } else {
      sw1_wcisniete = false;
    }

    // SW2 (PB1) - Zwiększenie liczby diod
    if (bit_is_clear(PINB, PB1)) {
      if (!sw2_wcisniete) {
        _delay_ms(20);
        if (liczba_sw_diod < 8) {
          sbi(PORTD, liczba_sw_diod);
          liczba_sw_diod++;
        }
        sw2_wcisniete = true;
      }
    } else {
      sw2_wcisniete = false;
    }

    // SW3 (PB2) - Zgaszenie wszystkich diod
    if (bit_is_clear(PINB, PB2)) {
      if (!sw3_wcisniete) {
        _delay_ms(20);
        PORTD = 0x00;
        liczba_sw_diod = 0;
        sw3_wcisniete = true;
      }
    } else {
      sw3_wcisniete = false;
    }

    // SW4 (PB3) - Zapalenie wszystkich diod
    if (bit_is_clear(PINB, PB3)) {
      if (!sw4_wcisniete) {
        _delay_ms(20);
        PORTD = 0xFF;
        liczba_sw_diod = 8;
        sw4_wcisniete = true;
      }
    } else {
      sw4_wcisniete = false;
    }
  }
}
