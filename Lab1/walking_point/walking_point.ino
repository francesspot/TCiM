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

void zad4_2(){
  DDRD = 0b11111111;
  while(1){
    for(uint8_t i = 0 ; i<8; i++){
     PORTD=_BV(i);
    _delay_ms(500);
    }
    for(uint8_t i = 6 ; i>0; i--){
     PORTD=_BV(i);
    _delay_ms(500);
    }
  }
}

int main() {
 
zad4_2();


}
