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

// Funkcja wysyłająca pojedynczy znak
void USART0_Transmit( unsigned char data ) {
  while ( !(UCSR0A & _BV(UDRE0)) );
  UDR0 = data;
}

// Funkcja odbierająca pojedynczy znak
unsigned char USART0_Receive( void ) {
  while ( !(UCSR0A & _BV(RXC0)) );
  return UDR0;
}

// Funkcja wysyłająca cały tekst
void text_USART0(char *text) {
  uint8_t i = 0;
  while(text[i]) {
    USART0_Transmit(text[i]);
    i++;
  }
}

volatile uint32_t sekundy = 0; // Zmienna licząca czas w tle

// przerwanie TIMER1
ISR(TIMER1_COMPA_vect) {
  sekundy++;
}

void Timer1_init() {
  sbi(TCCR1B, WGM12); // Tryb CTC
  sbi(TCCR1B, CS12); sbi(TCCR1B, CS10); // Preskaler 1024
  OCR1A = 15624;                        
  sbi(TIMSK1, OCIE1A);                 
}

int main() {
  // Inicjalizacja UART
  sbi(UCSR0B, RXEN0); 
  sbi(UCSR0B, TXEN0);
  cbi(UCSR0C, USBS0);
  UBRR0 = 51;
  DDRB = 0xFF;
  
  Timer1_init(); 

  // Konfiguracja pinów 
  sbi(DDRB, PB0);  // PB0 jako WYJŚCIE 
  cbi(DDRD, PD2);  // PD2 jako WEJŚCIE 
  sbi(PORTD, PD2); // PULL-UP dla przycisku

  sei();

  char text[64];
  text_USART0("Podaj komendy: a, s, d, f\r\n");

  while(1) {
    char znak = USART0_Receive(); 

    if (znak == 'a') {
      sbi(PORTB, PB0); // Stan wysoki
      text_USART0("LED1 ON\r\n");
    } 
    else if (znak == 's') {
      cbi(PORTB, PB0); // Stan niski
      text_USART0("LED1 OFF\r\n");
    } 
    else if (znak == 'd') {
      if (bit_is_clear(PIND, PD2)) {
        text_USART0("Przycisk wcisniety\r\n");
      } else {
        text_USART0("Przycisk zwolniony\r\n");
      }
    } 
    else if (znak == 'f') {
      sprintf(text, "Sekund od uruchomienia: %lu\r\n", sekundy);
      text_USART0(text);
    }
  }
  return 0;
}
