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

void USART0_Init( unsigned int ubrr )
{
  cbi(UCSR0C,UPM01); 
  cbi(UCSR0C,UPM00);
  cbi(UCSR0C,USBS0);
  cbi(UCSR0C,UCSZ02); 
  sbi(UCSR0C,UCSZ01); 
  cbi(UCSR0C,UCSZ00);
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  sbi(UCSR0B,RXEN0); 
  sbi(UCSR0B,TXEN0);
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART0_Transmit( unsigned char data )
{
  while ( !(UCSR0A & _BV(UDRE0)) )
  ;
  UDR0 = data;
}

unsigned char USART0_Receive( void )
{
  while ( !(UCSR0A & _BV(RXC0)) )
  ;
  return UDR0;
}

void display_USART0(char *to_display) {
  uint8_t i = 0;
  while(to_display[i]) {
    USART0_Transmit(to_display[i]);
    i++;
  }
}

volatile char text[32];
volatile uint16_t counter = 1;

int main(void) {
  USART0_Init(51);
  
  while(1) {
    sprintf(text, "Hello World: %d", counter);
    
    display_USART0(text);
    display_USART0("\r\n");
    
    counter++;
    _delay_ms(1000);
  }
  
  return 0;
}
