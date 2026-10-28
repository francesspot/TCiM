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

// Inicjalizacja przetwornika ADC
void ADC_init(void) {
    sbi(ADMUX, REFS0);
    cbi(ADMUX, REFS1);
    
    sbi(ADCSRA, ADEN);
    sbi(ADCSRA, ADPS2);
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
}

int read_ADC(uint8_t channel) {
    // Czyszczenie bitów wyboru kanału i ustawienie nowego kanału
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    
    // Start konwersji
    sbi(ADCSRA, ADSC);
    
    // Oczekiwanie na zakończenie konwersji 
    while (bit_is_set(ADCSRA, ADSC));
    
    return ADC;
}

int adcChannel0(void) { 
  return read_ADC(0); 
}

int adcChannel1(void) { 
  return read_ADC(1); 
}

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

int main() {
  // Inicjalizacja UART 
  sbi(UCSR0B, RXEN0); 
  sbi(UCSR0B, TXEN0);
  cbi(UCSR0C, USBS0);
  UBRR0 = 51;

  ADC_init();
  DDRB = 0xFF; // Port B jako wyjście

  char text[32];
  int histereza = 50;

  while(1) {
    int prog = adcChannel0();    // Potencjometr - czerwony (prog)
    int swiatlo = adcChannel1(); // Fotorezystor - niebieski (swiatlo)

    // Logika włącznika z histerezą - zielony
    if (swiatlo < (prog - histereza)) {
      cbi(PORTB, PB0); // Dzień
    } else if (swiatlo > (prog + histereza)) {
      sbi(PORTB, PB0); // Noc
    }

    // Odczyt stanu włącznika do narysowania na wykresie
    int przebieg_binarny = 0; // zielony
    if (bit_is_set(PORTB, PB0)) {
        przebieg_binarny = 1023; 
    }

    // Przygotowanie paczki danych rozdzielonych przecinkiem do Kreślarki
    sprintf(text, "%d,%d,%d\r\n", swiatlo, prog, przebieg_binarny);
    text_USART0(text);

    _delay_ms(50);
  }
  return 0;
}
