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


int main(){
  const uint8_t tab[10] = {
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01101111 //9
  };

  DDRD = 0xFF; // Ustalenie portu D jako wyjścia
  sbi(DDRC, PC0); // PC0 (sterowanie cyfrą 1) jako wyjście
  
  sbi(PORTC, PC0); // Włączenie cyfry 1

  uint8_t cyfra = 0;

  while (1) {
    PORTD = tab[cyfra]; // Wypisanie wzoru poszczególnej cyfry na port

    _delay_ms(1000);

    cyfra++;
    if (cyfra > 9) {
      cyfra = 0;
    }
  }
}
