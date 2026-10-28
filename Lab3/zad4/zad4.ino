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

volatile int liczba = 0;
volatile uint8_t aktualny_wyswietlacz = 0;

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

void displayInit() {
  // Ustawienie pinów wyświetlacza jako wyjścia
  DDRB = 0xFF; // Segmenty
  DDRC |= 0x0F; // Piny wyboru cyfry (PC0, PC1, PC2, PC3)

  // Konfiguracja Timer0 do multipleksowania
  // Tryb Normalny, Preskaler 64 -> sbi na CS01 i CS00
  sbi(TCCR0B, CS01);
  sbi(TCCR0B, CS00);
    
  // Włączenie przerwania od przepełnienia Timera 0
  sbi(TIMSK0, TOIE0);
}

ISR(TIMER0_OVF_vect) {
  PORTC |= 0x0F;

  int temp = liczba;
  uint8_t d1 = temp / 1000;
  uint8_t d2 = (temp / 100) % 10;
  uint8_t d3 = (temp / 10) % 10;
  uint8_t d4 = temp % 10;

  switch(aktualny_wyswietlacz) {
    case 0:
      PORTB = tab[d4];
      cbi(PORTC, PC0); // Aktywacja jedności
      break;
    case 1:
      // Wygaszenie zera wiodącego dla liczb mniejszych niż 10
      if (temp > 9) {
        PORTB = tab[d3];
      } else {
        PORTB = 0x00;
      }
      cbi(PORTC, PC1); // Aktywacja dziesiątek
      break;
    case 2:
      if (temp > 99) {
        PORTB = tab[d2];
      } else {
        PORTB = 0x00;
      }
      cbi(PORTC, PC2); // Aktywacja setek
      break;
    case 3:
      if (temp > 999) {
        PORTB = tab[d1];
      } else {
        PORTB = 0x00;
      }
      cbi(PORTC, PC3); // Aktywacja tysięcy
      break;
  }
  // 4. Inkrementacja licznika wyświetlaczy (0 -> 1 -> 2 -> 3 -> 0)
  aktualny_wyswietlacz++;
  if(aktualny_wyswietlacz > 3) {
    aktualny_wyswietlacz = 0;
  }
}

int main() {
  displayInit();
  sei(); // Globalne włączenie przerwań

  while (1) {
    // Licznik liczący od 0 do 250
    for (int i = 0; i <= 250; i++) {
      liczba = i;
      _delay_ms(1000); 
    }
  }
}