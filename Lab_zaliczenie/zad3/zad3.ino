#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
// #include "lcd.h" // Własna biblioteka LCD z zajęć

#ifndef bit_macros
// ... (makra) ...
#define _BV(bit)  (1<<(bit))
#define sbi(reg, bit) reg |= (_BV(bit))
#define cbi(reg, bit) reg &= ~(_BV(bit))
#define tbi(reg, bit) reg ^= (_BV(bit))
#define bit_is_set(sfr, bit)  ((sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!((sfr) & _BV(bit)))
#endif

volatile uint16_t ms_counter = 0;
volatile uint8_t seconds = 32, minutes = 5, hours = 12;
volatile int8_t countdown = -1; // -1 oznacza wyłączony
volatile uint8_t stop_flag = 0;

// Konfiguracja Timera (np. Timer0 dla przerwań co 1ms przy 16MHz)
void Timer0_Init() {
    // Tryb CTC
    sbi(TCCR0A, WGM01);
    // Preskaler 64 -> 16MHz/64 = 250kHz. Przerwanie co 1ms -> OCR0A = 249
    sbi(TCCR0B, CS01); 
    sbi(TCCR0B, CS00);
    OCR0A = 249;
    sbi(TIMSK0, OCIE0A); // Włącz przerwanie
}

// Obsługa przerwania z Timera 0 (wykonuje się co 1ms)
ISR(TIMER0_COMPA_vect) {
    ms_counter++;
    
    // LED1 - 1Hz (zmiana co 500ms)
    if (ms_counter % 500 == 0) {
        tbi(PORTB, 0); // Zmień stan LED1
    }
    
    // Precyzyjny zegar (1 sekunda = 1000ms)
    if (ms_counter >= 1000) {
        ms_counter = 0;
        seconds++;
        
        if(countdown > 0) {
            countdown--;
        } else if (countdown == 0) {
            stop_flag = 1;
            countdown = -1; // zatrzymaj odliczanie
        }

        if (seconds >= 60) { seconds = 0; minutes++; }
        if (minutes >= 60) { minutes = 0; hours++; }
        if (hours >= 24)   { hours = 0; }
    }
}

// Konfiguracja przycisku na przerwaniu zewnętrznym INT0 (PD2)
void INT0_Init() {
    cbi(DDRD, 2); sbi(PORTD, 2); // Wejście z pull-up
    sbi(EICRA, ISC01); // Reaguj na opadające zbocze
    sbi(EIMSK, INT0);  // Włącz przerwanie INT0
}

ISR(INT0_vect) {
    countdown = 10; // Rozpocznij odliczanie
    stop_flag = 0;
    cbi(PORTB, 1);  // Wyłącz LED2 (jeśli migała po poprzednim STOP)
}

int main(void) {
    sbi(DDRB, 0); // LED 1
    sbi(DDRB, 1); // LED 2

    // lcd_init();
    Timer0_Init();
    INT0_Init();
    sei(); // Włącz globalne przerwania

    char buffer[16];

    while (1) {
        // Wyświetlanie na LCD działa w pętli głównej (nie w przerwaniu!)
        // lcd_clear(); // Zależnie od biblioteki lepiej ustawiać tylko kursor, żeby nie mrugało
        // lcd_set_cursor(0,0);
        sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
        // lcd_str(buffer);

        // lcd_set_cursor(0,1);
        if (stop_flag) {
            // lcd_str("STOP       ");
            // Miganie LED2 po odliczeniu (np. z częstotliwością z pętli)
            if (ms_counter < 500) sbi(PORTB, 1);
            else cbi(PORTB, 1);
        } else if (countdown >= 0) {
            sprintf(buffer, "Odlicz: %d  ", countdown);
            // lcd_str(buffer);
        } else {
            // lcd_str("Gotowy     ");
        }
        
        _delay_ms(50); // Odświeżanie LCD nie musi być tak szybkie
    }
}