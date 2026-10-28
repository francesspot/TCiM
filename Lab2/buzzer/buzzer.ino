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


void buzzer(){
  sbi(PORTB, PB2); //buzzer włączony
  _delay_ms(50);
  cbi(PORTB, PB2); //buzzer wyłączony
  _delay_ms(50);
}

int main(void) {
  DDRD = 0xFF; // ustawione wyjście LED
  sbi(DDRB, PB2); // ustawione wyjście buzzera

  PORTD = 0x00; // wyczysczone LED
  cbi(PORTB, PB2); // buzzer wyłączony

  uint8_t licznik = 0; // licznik do LED

  while (1)
  {
    if (licznik < 8){
      PORTD |= (1 << licznik); // włącz kolejną diodę
      licznik++;
      buzzer();
      _delay_ms(900);
    } else if (licznik == 8){
      for (int i = 0; i < 5; i++){
        PORTD = 0xFF; // wszystkie diody świecą
        sbi(PORTB, PB2); //  buzzer włączony
        _delay_ms(500);

        PORTD = 0x00; // led wyłączony
        cbi(PORTB, PB2); // buzzer wylączony
        _delay_ms(500);
      }
      licznik++;
    }
  }
}
