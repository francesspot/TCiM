#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
// #include "lcd.h" // Twoja biblioteka LCD

#ifndef bit_macros
#define _BV(bit)  (1<<(bit))
#define sbi(reg, bit) reg |= (_BV(bit))
#define cbi(reg, bit) reg &= ~(_BV(bit))
#define tbi(reg, bit) reg ^= (_BV(bit))
#define bit_is_set(sfr, bit)  ((sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!((sfr) & _BV(bit)))
#endif

volatile uint16_t timer_ms = 0;
volatile uint8_t flag_10hz_update = 0;
volatile uint8_t flag_button_pressed = 0;

// Konfiguracja ADC
void ADC_Init() {
    // Napięcie referencyjne AVCC (zakładamy 5V)
    sbi(ADMUX, REFS0);
    // Włącz ADC i ustaw preskaler na 128 (16MHz/128 = 125kHz)
    sbi(ADCSRA, ADEN);
    sbi(ADCSRA, ADPS2); 
    sbi(ADCSRA, ADPS1); 
    sbi(ADCSRA, ADPS0);
}

// Funkcja odczytu ADC
uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Wybierz kanał
    sbi(ADCSRA, ADSC); // Rozpocznij konwersję
    while(bit_is_set(ADCSRA, ADSC)); // Czekaj na koniec
    return ADC;
}

// Konfiguracja Timera 0 (1ms)
void Timer0_Init() {
    sbi(TCCR0A, WGM01); // Tryb CTC
    sbi(TCCR0B, CS01); 
    sbi(TCCR0B, CS00); // Preskaler 64
    OCR0A = 249; // (16000000 / 64 / 1000) - 1
    sbi(TIMSK0, OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
    timer_ms++;
    // LED1 - zmiana co 500ms (1Hz)
    if (timer_ms % 500 == 0) 
    tbi(PORTB, 0);
    // Flaga odświeżania co 100ms (10Hz)
    if (timer_ms >= 1000) { 
        timer_ms = 0; // Reset co 1s zapobiega przepełnieniu
    }
    if (timer_ms % 100 == 0) flag_10hz_update = 1;
}

// Przycisk - Przerwanie INT0 (PD2)
void INT0_Init() {
    cbi(DDRD, 2); 
    sbi(PORTD, 2); // Pull-up
    sbi(EICRA, ISC01); // Zbocze opadające
    sbi(EIMSK, INT0);
}

ISR(INT0_vect) {
    flag_button_pressed = 1; // Zgłoś żądanie odczytu światła
}

int main(void) {
    sbi(DDRB, 0); // LED1 na wyjście
    // lcd_init();
    ADC_Init();
    Timer0_Init();
    INT0_Init();
    sei();

    char buffer[16];
    uint16_t pot_val, light_val;
    uint32_t mv_pot, mv_light;
    uint8_t light_scale;

    while(1) {
        // --- 1. Ciągły pomiar potencjometru (10Hz) ---
        if (flag_10hz_update) {
            flag_10hz_update = 0;
            pot_val = ADC_Read(0); // Zakładamy potencjometr na ADC0
            mv_pot = ((uint32_t)pot_val * 5000UL) / 1024UL; // Przeliczenie na mV (Vref = 5V)
            
            // lcd_set_cursor(0,0);
            sprintf(buffer, "Pot: %4lumV    ", mv_pot);
            // lcd_str(buffer);
        }

        // --- 2. Jednorazowy pomiar światła na żądanie ---
        if (flag_button_pressed) {
            flag_button_pressed = 0;
            light_val = ADC_Read(1); // Zakładamy czujnik na ADC1
            mv_light = ((uint32_t)light_val * 5000UL) / 1024UL;
            
            // Skala 0-10 (ADC daje 0-1023, więc dzielimy przez 102.4 ~ 102)
            light_scale = light_val / 102;
            if (light_scale > 10) light_scale = 10;
            
            // lcd_set_cursor(0,1);
            if (light_scale <= 3) {
                sprintf(buffer, "%4lumV Ciemno(%d)", mv_light, light_scale);
            } else {
                sprintf(buffer, "%4lumV Jasno (%d)", mv_light, light_scale);
            }
            // lcd_str(buffer);
        }
    }
}