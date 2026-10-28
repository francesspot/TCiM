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

int main(){
  sbi(DDRD, PD0);

  while(1) {
    tbi(PORTD, PD0);
    _delay_ms(500);
  }
}
