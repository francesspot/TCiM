#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h> 

#ifndef bit_macros
#define _BV(bit)  (1<<(bit))
#define sbi(reg, bit) reg |= (_BV(bit))
#define cbi(reg, bit) reg &= ~(_BV(bit))
#define tbi(reg, bit) reg ^= (_BV(bit))
#define bit_is_set(sfr, bit)  ((sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!((sfr) & _BV(bit)))
#endif



void USART0_Transmit( unsigned char data )
{
  while ( !(UCSR0A & _BV(UDRE0)) )
  ;
  UDR0 = data;
}

void text_USART0(char *text) {
  uint8_t i = 0;
  while(text[i]) {
    USART0_Transmit(text[i]);
    i++;
  }
}

int main(){
  sbi(UCSR0B,RXEN0); 
  sbi(UCSR0B,TXEN0);
  cbi(UCSR0C,USBS0);
  UBRR0 = 51;
  volatile char text[32];
  volatile uint16_t counter = 1;
  while (1) {
    sprintf(text, "Hello World: %d", counter);
    text_USART0(text);
    text_USART0("\r\n");

    counter++;
    _delay_ms(1000);
  }
  return 0;
}
