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
  // Napięcie referencyjne (AVCC) i ustawienie bitu REFS0 na 1 w rejestrze ADMUX
  sbi(ADMUX, REFS0);

  // Zegar ADC < 150 kHz. Preskaler 128 (16 Mhz / 128 = 125 kHz)
  // Ustawienie bitów w rejestrze ADCSRA na 1

  sbi(ADCSRA, ADPS0);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS2);

  // Uruchomienie układu przetwornika - ustawienie bitu ADEN
  sbi(ADCSRA, ADEN);
}

uint16_t ADC_10bit() {
  // Pojedynczy pomiar
  sbi(ADCSRA, ADSC);


  loop_until_bit_is_clear(ADCSRA, ADSC);

  // Zwrócenie 10-bitowego wyniku z rejestru ADC
  return ADC;
}

int main() {
  LCD_I2C Lcd = LCD_I2C(0x27);
  _delay_ms(100);
  Lcd.backLightOn();

  // Inicjalizacja ADC
  ADC_init();

  char text[20];

  while(1) {
    // Pobranie wyniku z przetwornika
    uint16_t pomiar = ADC_10bit();
    // V_mV = (pomiar * 5000) / 1023
    // Używamy uint32_t (UL - Unsigned Long), by zapobiec przepełnieniu podczas mnożenia
    uint32_t napiecie_mV = ((uint32_t)pomiar * 5000UL / 1023UL);

    uint8_t volty = napiecie_mV / 1000;
    uint8_t setne = (napiecie_mV % 1000) / 10;

    // Surowa wartość napięcia w pierwszej linii
    sprintf(text, "Wartosc ADC: %-4d", pomiar);
    Lcd.goTo(0, 0); 
    Lcd.writeText(text);

    // Wyświetlanie przeliczonego napięcia w drugiej linii
    // %d - wolty, %02d - ułamki
    sprintf(text, "Napiecie: %d.%02dV  ", volty, setne);
    Lcd.goTo(0, 1); 
    Lcd.writeText(text);

    _delay_ms(150);
  }
}
