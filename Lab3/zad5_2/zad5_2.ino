#include <avr/io.h>
#include <avr/interrupt.h>

#define ZNAK_MINUS 0b01000000

#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit) reg |= (_BV(bit))
#endif
#ifndef cbi
#define cbi(reg,bit) reg &= ~(_BV(bit))
#endif

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
    0b01101111 // 9
};

volatile int16_t wartosc = 0;

void init() {
    DDRD = 0xFF;  // Segmenty
    DDRC |= 0x0F; // Cyfry
    
    // Enkoder PC4, PC5
    PORTC |= (1 << PC4) | (1 << PC5); // Pull-up
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT12); // Przerwanie na PC4

    // Timer dla multipleksowania
    TCCR0B |= (1 << CS01) | (1 << CS00); // Preskaler 64
    TIMSK0 |= (1 << TOIE0);
}

// Multipleksing wyświetlacza
ISR(TIMER0_OVF_vect) {
    static uint8_t cyfra = 0;
    PORTC &= ~0x0F; // Wyłączanie cyfr

    int16_t cyfra_temp = wartosc;
    if (cyfra_temp < 0) {
      cyfra_temp = -cyfra_temp;
    }

    uint8_t wzor = 0;
    uint8_t d1 = cyfra_temp % 10;          // Jedności
    uint8_t d10 = (cyfra_temp / 10) % 10;  // Dziesiątki
    uint8_t d100 = (cyfra_temp / 100) % 10; // Setki

    switch(cyfra) {
        case 3: // JEDNOŚCI
            wzor = tab[d1]; 
            break;
        case 2: // DZIESIĄTKI
            if (cyfra_temp >= 10) {
              wzor = tab[d10];
            }
            else if (wartosc < 0) {
              wzor = ZNAK_MINUS;
            }
            break;
        case 1: // SETKI
            if (cyfra_temp >= 100) {
              wzor = tab[d100];
            }
            else if (wartosc < 0 && cyfra_temp >= 10) {
              wzor = ZNAK_MINUS;
            }
            break;
        case 0: // TYLKO MINUS DLA -100
            if (wartosc < 0 && cyfra_temp >= 100) {
              wzor = ZNAK_MINUS;
            }
            break;
    }

    PORTD = wzor;
    PORTC |= (1 << cyfra);
    if (++cyfra > 3) cyfra = 0;
}

// Enkoder
ISR(PCINT1_vect) {
    static uint8_t stan_A_poprzedni = 0;
    uint8_t stan_A = (PINC & (1 << PC4));

    // Wykrywamnie zbocza opadającego
    if (stan_A_poprzedni && !stan_A) {
        for(volatile uint16_t i=0; i<500; i++); 

        if (PINC & (1 << PC5)) {
            if (wartosc < 100) {
              wartosc++;
            }
        } else {
            if (wartosc > -100) {
              wartosc--;
            }
        }
    }
    stan_A_poprzedni = stan_A;
}

int main(void) {
    init();
    sei();
    while (1);
}
