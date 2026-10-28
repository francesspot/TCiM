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

void timerInit() {
// Tryb CTC (Clear Timer on Compare) - WGM12 = 1
    sbi(TCCR1B, WGM12);
    
    // Preskaler 256 (dla 16MHz daje 62500 taktów na sekundę)
    sbi(TCCR1B, CS12);
    
    // Wartość dla 0.5s: (16MHz / 256 / 2Hz) - 1 = 31249
    OCR1A = 31249;
    
    // Włączenie przerwania Compare Match A
    sbi(TIMSK1, OCIE1A);
}

int main(){
  timerInit();
  sei();

  sbi(DDRD, PD0); // Dioda 1 jako wyjście
  sbi(DDRD, PD1); // Dioda 2 jako wyjście

  while(1) {
    tbi(PORTD, PD0);
    _delay_ms(500);
  }
}

ISR(TIMER1_COMPA_vect) {
  tbi(PORTD, PD1);
}