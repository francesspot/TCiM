#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef bit_macros
// ... (Makra bez zmian) ...
#define _BV(bit)  (1<<(bit))
#define sbi(reg, bit) reg |= (_BV(bit))
#define cbi(reg, bit) reg &= ~(_BV(bit))
#define tbi(reg, bit) reg ^= (_BV(bit))
#define bit_is_set(sfr, bit)  ((sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!((sfr) & _BV(bit)))
#endif

// Zmienne globalne do asynchroniczności
volatile uint16_t timer_ms = 0;
volatile uint16_t seconds_counter = 0;
volatile uint8_t flag_button_measure = 0;
volatile uint8_t flag_1sec_tick = 0;

// Zmienne konfiguracyjne systemu (zmieniane przez UART)
uint8_t ch0_active = 0; // Kanał 0 (np. Potencjometr)
uint8_t ch1_active = 0; // Kanał 1 (np. Czujnik światła)
uint16_t cyclic_interval = 0; // 0 oznacza wyłączony tryb cykliczny

// Bufor UART
#define UART_RX_BUFFER_SIZE 32
volatile char rx_buffer[UART_RX_BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t rx_cmd_ready = 0;

// ==========================================
// FUNKCJE POMOCNICZE
// ==========================================

// Inicjalizacja ADC (jak w zad. 4)
void ADC_Init() {
    sbi(ADMUX, REFS0);
    sbi(ADCSRA, ADEN);
    sbi(ADCSRA, ADPS2); sbi(ADCSRA, ADPS1); sbi(ADCSRA, ADPS0);
}

uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    sbi(ADCSRA, ADSC);
    while(bit_is_set(ADCSRA, ADSC));
    return ADC;
}

// Prosta funkcja wysyłająca string po UART (Dostosuj pod swoją bibliotekę!)
void uart_puts(const char* str) {
    while (*str) {
        while (bit_is_clear(UCSR0A, UDRE0));
        UDR0 = *str++;
    }
}

// Inicjalizacja UART (Baudrate 9600 dla 16MHz)
void UART_Init() {
    UBRR0H = 0;
    UBRR0L = 103; // Zgodnie z notą katalogową dla 16MHz i 9600 baud
    sbi(UCSR0B, RXEN0); sbi(UCSR0B, TXEN0); // RX i TX on
    sbi(UCSR0B, RXCIE0); // Przerwanie odbiorcze
    sbi(UCSR0C, UCSZ01); sbi(UCSR0C, UCSZ00); // Format 8N1
}

// Przerwanie RX - Zbieranie znaków do bufora
ISR(USART_RX_vect) {
    char data = UDR0;
    if (data == '\n' || data == '\r') {
        if (rx_index > 0) {
            rx_buffer[rx_index] = '\0'; // Zakończ string
            rx_cmd_ready = 1;
        }
    } else {
        if (rx_index < UART_RX_BUFFER_SIZE - 1) {
            rx_buffer[rx_index++] = data;
        }
    }
}

// Inicjalizacja Timer0 i INT0 (jak w zad. 4)
void Timer0_Init() {
    sbi(TCCR0A, WGM01); sbi(TCCR0B, CS01); sbi(TCCR0B, CS00);
    OCR0A = 249; sbi(TIMSK0, OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
    timer_ms++;
    if (timer_ms % 500 == 0) tbi(PORTB, 0); // Dioda LED 1Hz
    if (timer_ms >= 1000) {
        timer_ms = 0;
        seconds_counter++;
        flag_1sec_tick = 1;
    }
}

void INT0_Init() {
    cbi(DDRD, 2); sbi(PORTD, 2); sbi(EICRA, ISC01); sbi(EIMSK, INT0);
}

ISR(INT0_vect) {
    flag_button_measure = 1; // Komenda manualna "jeden"
}

// Funkcja wykonująca i wysyłająca pomiary
void Execute_Measurements(const char* prefix) {
    char tx_buf[64];
    if (ch0_active == 0 && ch1_active == 0) {
        uart_puts("Brak przypisanych kanalow do pomiaru.\r\n");
        return;
    }
    
    uart_puts(prefix);
    
    if (ch0_active) {
        uint32_t mv0 = ((uint32_t)ADC_Read(0) * 5000UL) / 1024UL;
        sprintf(tx_buf, " CH0: %lumV", mv0);
        uart_puts(tx_buf);
    }
    if (ch1_active) {
        uint32_t mv1 = ((uint32_t)ADC_Read(1) * 5000UL) / 1024UL;
        sprintf(tx_buf, " CH1: %lumV", mv1);
        uart_puts(tx_buf);
    }
    uart_puts("\r\n");
}

// ==========================================
// PĘTLA GŁÓWNA
// ==========================================
int main(void) {
    sbi(DDRB, 0); // LED na wyjście
    ADC_Init();
    UART_Init();
    Timer0_Init();
    INT0_Init();
    sei();

    uart_puts("System gotowy. Wpisz '?' aby zobaczyc menu.\r\n");

    while(1) {
        // --- 1. Obsługa terminala (analiza komend) ---
        if (rx_cmd_ready) {
            char* cmd = (char*)rx_buffer;
            
            if (strcmp(cmd, "jeden") == 0) {
                Execute_Measurements("[Jednorazowy]");
            } 
            else if (strncmp(cmd, "cykliczny-", 10) == 0) {
                // Wyciągnij liczbę sekund
                cyclic_interval = atoi(cmd + 10);
                seconds_counter = 0; // Resetuj licznik
                char msg[40];
                sprintf(msg, "Uruchomiono pomiar co %d sekund.\r\n", cyclic_interval);
                uart_puts(msg);
            } 
            else if (strcmp(cmd, "stop") == 0) {
                cyclic_interval = 0;
                uart_puts("Przerwano pomiar cykliczny.\r\n");
            } 
            else if (strncmp(cmd, "CH-", 3) == 0 && strlen(cmd) >= 6) {
                // Komenda np. CH-0-1 lub CH-1-0
                uint8_t ch_num = cmd[3] - '0';
                uint8_t state = cmd[5] - '0';
                
                if (ch_num == 0) ch0_active = state;
                else if (ch_num == 1) ch1_active = state;
                
                uart_puts("Zmieniono konfiguracje kanalow.\r\n");
            } 
            else if (strcmp(cmd, "?") == 0) {
                uart_puts("--- MENU ---\r\n");
                uart_puts("jeden - jednorazowy pomiar\r\n");
                uart_puts("cykliczny-X - pomiar co X sek\r\n");
                uart_puts("stop - koniec cyklu\r\n");
                uart_puts("CH-X-Y - gdzie X to 0 lub 1 (kanal), Y to 1(wl) lub 0(wyl)\r\n");
            } 
            else {
                uart_puts("Nieznana komenda.\r\n");
            }

            rx_index = 0; // Resetuj bufor po obsłużeniu
            rx_cmd_ready = 0;
        }

        // --- 2. Obsługa przerwania cyklicznego z Timera ---
        if (flag_1sec_tick) {
            flag_1sec_tick = 0;
            if (cyclic_interval > 0 && seconds_counter >= cyclic_interval) {
                Execute_Measurements("[Cykliczny]");
                seconds_counter = 0; // Odliczaj od nowa
            }
        }

        // --- 3. Obsługa wciśnięcia przycisku (działa jak komenda 'jeden') ---
        if (flag_button_measure) {
            flag_button_measure = 0;
            Execute_Measurements("[Przycisk]");
        }
    }
}