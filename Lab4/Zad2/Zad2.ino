#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "LCDI2C.h"

// Makra pomocnicze
#ifndef _BV
#define _BV(bit)        (1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit)    reg |= (_BV(bit))
#endif
#ifndef cbi
#define cbi(reg,bit)    reg &= ~(_BV(bit))
#endif

#define bit_is_clear(sfr, bit)  (!(sfr & _BV(bit)))

// Definicje pinów
#define BTN_GODZ PINB0
#define BTN_MIN PINB1
#define BTN_WYL PINB2
#define ALARM_PIN PIND2

// Zmienne globalne
volatile uint8_t sek = 55, min = 4, godz = 12;
volatile uint8_t flaga_sekundy = 1;

uint8_t b_sek = 0, b_min = 5, b_godz = 12;
uint8_t alarm_dziala = 0;

// Przerwanie timera - wywoływane co 1 sekundę
ISR(TIMER1_COMPA_vect) {
    sek++;
    if (sek > 59) {
        sek = 0;
        min++;
        if (min > 59) {
            min = 0;
            godz++;
            if (godz > 23) {
                godz = 0;
            }
        }
    }
    flaga_sekundy = 1; // Sygnał do odświeżenia ekranu
}

void Timer1_Init() {
    // Tryb CTC (Clear Timer on Compare Match)
    sbi(TCCR1B, WGM12);
    // Ustawienie wartości porównania dla 1s przy 16MHz i preskalerze 256
    OCR1A = 62500;
    // Włączenie przerwania Compare Match A
    sbi(TIMSK1, OCIE1A);
    // Ustawienie preskalera 256 i start timera
    sbi(TCCR1B, CS12);
    // Włączenie globalnych przerwań
    sei();
}

int main() {
    // Inicjalizacja LCD
    LCD_I2C Lcd = LCD_I2C(0x27);
    _delay_ms(100);
    Lcd.backLightOn();

    // Konfiguracja przycisków jako wejścia z rezystorami Pull-Up
    cbi(DDRB, BTN_GODZ);
    cbi(DDRB, BTN_MIN);
    cbi(DDRB, BTN_WYL);
    sbi(PORTB, BTN_GODZ);
    sbi(PORTB, BTN_MIN);
    sbi(PORTB, BTN_WYL);

    // Konfiguracja pinu alarmu (wyjście)
    sbi(DDRD, ALARM_PIN);
    cbi(PORTD, ALARM_PIN);

    Timer1_Init();

    char text[20];

    while (1) {
        // --- 1. ODŚWIEŻANIE LCD ---
        // Odświeżamy tylko gdy zmieni się sekunda lub ustawiamy budzik
        if (flaga_sekundy) {
            flaga_sekundy = 0;

            sprintf(text, "Czas: %02d:%02d:%02d", godz, min, sek);
            Lcd.goTo(0, 0);
            Lcd.writeText(text);

            sprintf(text, "Budz: %02d:%02d:%02d", b_godz, b_min, b_sek);
            Lcd.goTo(0, 1);
            Lcd.writeText(text);
        }

        // --- 2. LOGIKA ALARMU ---
        if (godz == b_godz && min == b_min && sek == b_sek) {
            alarm_dziala = 1;
            sbi(PORTD, ALARM_PIN); 
        }

        // --- 3. OBSŁUGA PRZYCISKÓW ---

        // Przycisk wyłączający alarm
        if (bit_is_clear(PINB, BTN_WYL)) {
            _delay_ms(30); // Eliminacja drgań styków
            if (bit_is_clear(PINB, BTN_WYL)) {
                if (alarm_dziala) {
                    alarm_dziala = 0;
                    cbi(PORTD, ALARM_PIN);
                }
                // Czekaj aż użytkownik puści przycisk
                while(bit_is_clear(PINB, BTN_WYL)); 
            }
        }

        // Ustawianie godzin budzika
        if (bit_is_clear(PINB, BTN_GODZ)) {
            _delay_ms(30);
            if (bit_is_clear(PINB, BTN_GODZ)) {
                b_godz++;
                if (b_godz > 23) b_godz = 0;
                
                flaga_sekundy = 1; // Wymuś odświeżenie LCD
                while (bit_is_clear(PINB, BTN_GODZ));
            }
        }

        // Ustawianie minut budzika
        if (bit_is_clear(PINB, BTN_MIN)) {
            _delay_ms(30);
            if (bit_is_clear(PINB, BTN_MIN)) {
                b_min++;
                if (b_min > 59) b_min = 0;
                
                flaga_sekundy = 1; // Wymuś odświeżenie LCD
                while (bit_is_clear(PINB, BTN_MIN));
            }
        }
    }

    return 0; // Standardowa końcówka main
}
