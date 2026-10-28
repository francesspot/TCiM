#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Definicje makr dla operacji na bitach
#ifndef sbi
#define sbi(reg,bit) reg |= (1<<(bit))
#endif
#ifndef cbi
#define cbi(reg,bit) reg &= ~(1<<(bit))
#endif


volatile uint16_t liczba = 0;

// Tablica cyfr
const uint8_t tab[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

ISR(TIMER0_OVF_vect) {
    static uint8_t aktualna_cyfra = 0;
    
    // Wyłączenie wszystkich cyfr
    PORTC &= ~0x0F; 

    // Cyfry do wyświetlenia
    uint8_t cyfra = 0;
    if (aktualna_cyfra == 0) 
    {
      cyfra = (liczba / 1000) % 10;
    }
    else if (aktualna_cyfra == 1) {
      cyfra = (liczba / 100) % 10;
    }
    else if (aktualna_cyfra == 2) {
      cyfra = (liczba / 10) % 10;
    }
    else if (aktualna_cyfra == 3) {
      cyfra = liczba % 10;
    }

    PORTD = tab[cyfra];

    // 
    sbi(PORTC, aktualna_cyfra);

    // Inkrementacja indeksu cyfry dla kolejnego przerwania
    aktualna_cyfra++;
    if (aktualna_cyfra > 3) {
        aktualna_cyfra = 0;
    }
}

void setup_timer0() {
    // 8MHz: 8000000 / 64 / 256 = 488 przerwań na sekundę
    TCCR0B |= (1 << CS01) | (1 << CS00); 
    TIMSK0 |= (1 << TOIE0); // Włączenie przerwania przepełnienia dla Timer0
}

int main(void) {
    // Konfiguracja portów
    DDRD = 0xFF;  // Cały PORTD jako wyjścia
    DDRC |= 0x0F; // PC0-PC3 jako wyjścia 

    setup_timer0(); 
    sei();          

    while (1) {
        // Aktualizacja liczbv wyświetlanej
        for(uint16_t i = 0; i <= 250; i++) {
          liczba = i;
          _delay_ms(100); 
        }
    }

    return 0;
}
