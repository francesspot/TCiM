#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "LCDI2C.h"

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

#define   bit_is_set(sfr, bit)   (_SFR_BYTE(sfr) & _BV(bit))
#define   bit_is_clear(sfr, bit)   (!(_SFR_BYTE(sfr) & _BV(bit)))

// Inicjalizacja przetwornika ADC
void ADC_init() {
  // Ustawienie bitu REFS0 na 1 w rejestrze ADMUX
  sbi(ADMUX, REFS0);

  // Preskaler 128 (16 Mhz / 128 = 125 kHz)
  // Ustawienie bitów w rejestrze ADCSRA na 1

  sbi(ADCSRA, ADPS0);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS2);

  // Ustawienie bitu ADEN
  sbi(ADCSRA, ADEN);
}

// Funkcja odczytująca kanał 0 (ADC0 / PC0)
uint16_t adcChannel0() {
  ADMUX = (ADMUX & 0xF0);

  sbi(ADCSRA, ADSC); // Start konwersji
  loop_until_bit_is_clear(ADCSRA, ADSC); // Oczekiwanie na koniec
  return ADC;
}

// Funkcja odczytująca kanał 1 (ADC1 / PC1) - Fotorezystor
uint16_t adcChannel1() {
  ADMUX = (ADMUX & 0xF0) | 0x01;
  
  sbi(ADCSRA, ADSC); // Start konwersji
  loop_until_bit_is_clear(ADCSRA, ADSC); // Oczekiwanie na koniec
  return ADC;
}

int main() {
  LCD_I2C Lcd = LCD_I2C(0x27);
  _delay_ms(100);
  Lcd.backLightOn();
  DDRB = 0xFF;

  // Inicjalizacja ADC
  ADC_init();

  char text[20];

  // Wartość histerezy
  int histereza = 50;

  while(1) {
    int prog = adcChannel0();     // Próg ustawiany potencjometrem
    int swiatlo = adcChannel1();  // Aktualne natężenie światła (fotorezystor)

  // ZADANIE I: Wyświetlanie w pierwszej linii pomiarów z dwóch kanałów
    sprintf(text, "P:%-4d S:%-4d ", prog, swiatlo);
    Lcd.goTo(0, 0); 
    Lcd.writeText(text);

  // ZADANIE II: Automatyczny włącznik zmierzchowy z histerezą
    Lcd.goTo(0, 1);
  if (swiatlo < (prog - histereza)) {
    // Włączenie diody
    cbi(PORTB, PB0);
    Lcd.writeText("LED: OFF (DZIEN)");
          
  } else if (swiatlo > (prog + histereza)) {
    // Wyłączenie diody
    sbi(PORTB, PB0);
    Lcd.writeText("LED: ON  (NOC)  ");
  } else {
    // Strefa histerezy
    // Aktualny stan pinu
    if (bit_is_set(PORTB, PB0)) {
      Lcd.writeText("LED: ON  (NOC)  ");
    } else {
      Lcd.writeText("LED: OFF (DZIEN)");
    }
  }

  _delay_ms(150);
  }
}
