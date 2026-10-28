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

  DDRD = 0xFF; // segmenty wyświetlacza
  DDRC != 0x0F; // PC0-PC3 jako wyjścia (sterowanie cyframi)

  // przyciski jako wejścia (PB0-PB3)
  cbi(DDRB, PB0);
  cbi(DDRB, PB1);
  cbi(DDRB, PB2);
  cbi(DDRB, PB3);

  // Pull-up dla przycisków
  sbi(PORTB, PB0);
  sbi(PORTB, PB1);
  sbi(PORTB, PB2);
  sbi(PORTB, PB3);

  // Buzzer jako wyjście PB4
  sbi(DDRB, PB4);
  cbi(PORTB, PB4); // Buzzer wyłączony

  uint16_t czas = 0;
  uint8_t stan_odliczania = 0;
  uint8_t licznik_cykli = 0;
  uint16_t czas_buzzera = 0; 

  while(1) {
    uint8_t d1 = czas / 1000;
    uint8_t d2 = (czas / 100) % 10;
    uint8_t d3 = (czas / 10) % 10;
    uint8_t d4 = czas % 1000;

    //1. Wyświetlanie zmiennej czas
    // 1 cyfra
    PORTC &= ~0x0F;
    PORTD = tab[d1];
    sbi(PORTC, PC0)
    _delay_ms(5);    
    
    // 2 cyfra
    PORTC &= ~0x0F;
    PORTD = tab[d2];
    sbi(PORTC, PC1)
    _delay_ms(5);    
    
    // 3 cyfra
    PORTC &= ~0x0F;
    PORTD = tab[d3];
    sbi(PORTC, PC2)
    _delay_ms(5);
    
    // 4 cyfra
    PORTC &= ~0x0F;
    PORTD = tab[d4];
    sbi(PORTC, PC3)
    _delay_ms(5);

    // razem 20ms

    //2. Odliczanie czasu (1 sekunda = 50 cykli * 20ms)
    if (stan_odliczania == 1) {
      licznik_cykli++;
      if (licznik_cykli >= 50) { // 1 sekunda
        licznik_cykli = 0;
        if (czas > 0) {
          czas--;
        } else {
          // koniec czasu!
          stan_odliczania = 0;
          czas_dzwonienia = 100; // 2 sekundy = 100 * 2ms
          sbi(PORTB, PB4);
        }
      }
    }

    // WyŁaczanie buzzera po 2 sekundach
    if (czas_dzwonienia > 0) {
      czas_dzwonienia--;
      if (czas_dzwonienia == 0) {
        cbi(PORTB, PB4); // Buzzer wyłączony
      }
    }

    // 3. Obsługiwanie przycisków
    if (czas_dzwonienia == 0) {

      // SW4 (Start odliczania)
      if (bit_is_clear(PINB, PB3) && czas > 0) {
        stan_odliczania = 1;
      }

      // SW3 (STOP / Kasowanie)
      if (bit_is_clear(PINB, PB2)) {
        stan_odliczania = 0;
        czas = 0;
      }

      if (stan_odliczzania == 0) {
        //SW1
        if (bit_is_clear(PINB, PB0)) {
          czas += 1;
          if (czas > 9999)
          {
            czas == 9999;
          }
        }
        //SW2
        if (bit_is_clear(PINB, PB1)) {
          czas += 10;
          if (czas > 9999)
          {
            czas == 9999;
          }
      }
    }
  }
}
