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

// Funkcja odczytująca pełną liczbę wpisaną w konsoli
int odbierz_rok() {
  char buf[20];
  uint8_t index = 0;
  
  while(1) {
    char c = USART0_Receive();
    
    if (c == '\n' || c == '\r') {
      if (index > 0) { // Zabezpieczenie przed samymi Enterami
        buf[index] = '\0';
        return atoi(buf);
      }
    } else if (c >= '0' && c <= '9') {
      if (index < 9) {
      buf[index] = c;
      index++;
    }
  }
  }
}

int main() {
  sbi(UCSR0B, RXEN0); 
  sbi(UCSR0B, TXEN0);
  cbi(UCSR0C, USBS0);
  UBRR0 = 51;

  char text[64];
  const uint16_t OBECNY_ROK = 2026;

  text_USART0("--- Kalkulator Wieku ---\r\n");

  while(1) {
    text_USART0("\r\nPodaj swoj rok urodzenia i wcisnij Enter:\r\n");
    
    uint16_t rok = odbierz_rok();

    if (rok <= 1900 || rok > OBECNY_ROK) {
      text_USART0("Blad! Podano zly rok urodzenia.\r\n");
    } else {
      uint16_t wiek = OBECNY_ROK - rok;

      if (wiek < 18) {
        sprintf(text, "Masz %d lat, Nauka Nauka Nauka.\r\n", wiek);
      } 
      else if (wiek >= 18 && wiek < 25) {
        sprintf(text, "Masz %d lat, Piwo Woda Polibuda!\r\n", wiek);
      } 
      else if (wiek >= 25 && wiek < 65) {
        sprintf(text, "Masz %d lat, pracuj ciezko na ZUS.\r\n", wiek);
      } 
      else {
        sprintf(text, "Masz %d lat, Starosc nie radosc, mlodosc nie wiecznosc!\r\n", wiek);
      }

      text_USART0(text);
    }
  }
  return 0;
}
