#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#ifndef bit_macros
#define _BV(bit)  (1<<(bit))
#define sbi(reg, bit) reg |= (_BV(bit))
#define cbi(reg, bit) reg &= ~(_BV(bit))
#define tbi(reg, bit) reg ^= (_BV(bit))
#define bit_is_set(sfr, bit)  ((sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!((sfr) & _BV(bit)))
#endif

// Konfiguracja pinów - dostosuj do swojej płytki
#define LED1_PIN 0 // PORTB
#define LED2_PIN 1 // PORTB

int main(void) {
    // Ustawienie pinów diod jako wyjścia
    sbi(DDRB, LED1_PIN);
    sbi(DDRB, LED2_PIN);

    uint16_t timer_1hz = 0;
    uint16_t timer_3hz = 0;

    while (1) {
        _delay_ms(1); // Podstawa czasu dla pętli
        timer_1hz++;
        timer_3hz++;

        // LED1 - 1Hz (zmiana stanu co 500ms)
        if (timer_1hz >= 500) {
            tbi(PORTB, LED1_PIN);
            timer_1hz = 0;
        }

        // LED2 - 3Hz (zmiana stanu co ok. 166ms)
        if (timer_3hz >= 166) {
            tbi(PORTB, LED2_PIN);
            timer_3hz = 0;
        }
    }
}