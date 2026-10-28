#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h> // NIEZBĘDNE DLA sprintf!

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
  cbi(UCSR0C,UPM01); cbi(UCSR0C,UPM00);
  cbi(UCSR0C,USBS0);
  cbi(UCSR0C,UCSZ02); sbi(UCSR0C,UCSZ01); cbi(UCSR0C,UCSZ00);
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  sbi(UCSR0B,RXEN0); sbi(UCSR0B,TXEN0);
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


volatile char text[32]; // Bufor na sklejony tekst

void ADC_Init() {
    sbi(ADMUX, REFS0); // Napięcie referencyjne AVCC
    sbi(ADCSRA, ADPS2); sbi(ADCSRA, ADPS1); sbi(ADCSRA, ADPS0); // Preskaler 128
    sbi(ADCSRA, ADEN); // Włączenie ADC
}

uint16_t ADC_Read(uint8_t channel) {
    channel &= 0b00000111; // Zabezpieczenie wyboru kanału
    ADMUX = (ADMUX & 0xF8) | channel;
    
    sbi(ADCSRA, ADSC); // Start konwersji
    while (ADCSRA & _BV(ADSC)); // Czekamy na zakończenie (pętla kręci się dopóki bit ADSC to 1)
    
    return (ADC); // Zwraca 10-bitowy wynik (0-1023)
}
// ---------------------------

int main(void) {
    USART0_Init(51); // Inicjalizacja UART (19200 bps)
    ADC_Init();      // Inicjalizacja ADC

    while (1) {
        uint16_t wartoscFoto = ADC_Read(0); // Odczyt pinu A0
        uint16_t wartoscPot = ADC_Read(1);  // Odczyt pinu A1
        
        uint16_t przebiegBinarny = 0;
        if (wartoscFoto < wartoscPot) {
            przebiegBinarny = 1023; // Logiczne '1' rozciągnięte dla czytelności wykresu
        }

        sprintf(text, "%d,%d,%d\r\n", wartoscFoto, wartoscPot, przebiegBinarny);
        
        // Wysłanie całej paczki danych do Kreślarki
        display_USART0(text);

        _delay_ms(50);
    }
    return 0;
}