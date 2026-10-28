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
  sbi(TCCR1B, WGM12);
  //Wybranie trybu pracy CTC z TOP OCR1A
  DDRD = 0x0F;
  
  //Wybranie dzielnika czestotliwosci - częstotliwość Atmega328: 16 MHz
  sbi(TCCR1B, CS12);

 
  //Zapisanie do OCR1A wartosci odpowiadajacej 0,5s
  OCR1A = 31250; // (16 000 000 / 256 / 2)
 
  //Uruchomienie przerwania OCIE1A
  sbi(TIMSK1, OCIE1A);
}


//volatile -informacja dla kompilatora aby nie optymalizował zmiennej, 
//zmienna bezpieczna do wykorzystania w przerwaniach
volatile int zmienna=0; 



int main() {
  timerInit();
  sei(); //Uruchamia przerwania globalne
  sbi(DDRD,PD0);
  sbi(DDRD,PD1); 
 
  while (1) {
    tbi(PORTD, PD0);
    _delay_ms(500);
  }
}

ISR(TIMER1_COMPA_vect) {
    tbi(PORTD, PD1);
}
