#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h> // Potrzebne do funkcji abs()

// Definicje makr
#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit) reg |= (_BV(bit))
#endif
#ifndef cbi
#define cbi(reg,bit) reg &= ~(_BV(bit))
#endif

// Zmienna wartości od -100 do +100
volatile int wartosc = 0; 
volatile uint8_t aktualny_wyswietlacz = 0;

const uint8_t tab[10] = {
  0b00111111, //0
  0b00000110, //1
  0b01011011, //2
  0b01001111, //3
  0b01100110, //4
  0b01101101, //5
  0b01111101, //6
  0b00000111, //7
  0b01111111, //8
  0b01101111 //9
};

// Znak minus - zakładam segment 'G', czyli 7 bit od prawej (0b01000000)
#define ZNAK_MINUS 0b01000000 

void displayInit() {
    DDRB = 0xFF; 
    DDRC |= 0x0F; 
    sbi(TCCR0B, CS01);
    sbi(TCCR0B, CS00);
    sbi(TIMSK0, TOIE0); 
}

void encoderInit() {
    // Konfiguracja PD2 (Pin A) i PD3 (Pin B) jako wejścia
    cbi(DDRD, PD2);
    cbi(DDRD, PD3);
    
    // Włączenie wbudowanych rezystorów podciągających (PULL-UP)
    sbi(PORTD, PD2);
    sbi(PORTD, PD3);

    // Przerwanie INT0 (PD2) aktywowane zboczem opadającym (ISC01=1, ISC00=0)
    sbi(EICRA, ISC01);
    cbi(EICRA, ISC00);

    // Włączenie przerwania INT0
    sbi(EIMSK, INT0);
}

// Przerwanie obsługujące odświeżanie wyświetlacza i wyświetlanie minusa
ISR(TIMER0_OVF_vect) {
    PORTC |= 0x0F; // Wygaszenie wszystkiego

    // Ponieważ na wyświetlaczu można wyświetlić tylko wartości dodatnie, używamy abs()
    int val_dodatnia = abs(wartosc); 
    uint8_t d1 = (val_dodatnia / 100) % 10;
    uint8_t d2 = (val_dodatnia / 10) % 10;
    uint8_t d3 = val_dodatnia % 10;

    switch(aktualny_wyswietlacz) {
        case 0:
            PORTB = tab[d3];
            cbi(PORTC, PC0);
            break;
        case 1:
            // Sprawdzamy czy musimy pokazać cyfrę dziesiątek
            if (val_dodatnia > 9) {
                PORTB = tab[d2];
            } else if (wartosc < 0) {
                // Jeśli wartość to np. -5, to na cyfrze dziesiątek pokazujemy minus
                PORTB = ZNAK_MINUS;
            } else {
                PORTB = 0x00; // Puste miejsce
            }
            cbi(PORTC, PC1);
            break;
        case 2:
            if (val_dodatnia > 99) {
                PORTB = tab[d2];
            } else if (wartosc < -9) {
                // Jeśli wartość to np. -20, minus dajemy na pole setek
                PORTB = ZNAK_MINUS;
            } else {
                PORTB = 0x00;
            }
            cbi(PORTC, PC2);
            break;
        case 3:
            // Jeśli mamy -100, minus zajmie pierwszą cyfrę (tysiące)
            if (wartosc == -100) PORTB = ZNAK_MINUS; 
            else PORTB = 0x00;
            
            cbi(PORTC, PC3);
            break;
    }

    aktualny_wyswietlacz++;
    if(aktualny_wyswietlacz > 3) aktualny_wyswietlacz = 0;
}

// Przerwanie zewnętrzne sprzętowe obsługujące obrót enkodera
ISR(INT0_vect) {
    // Zbocze opadające na kanale A. Sprawdzamy stan kanału B (PD3), żeby poznać kierunek obrotu.
    // Jeśli używamy makra _BV z PIND to sprawdzamy logicznie: PIND & (1<<PD3)
    if ( PIND & _BV(PD3) ) {
        wartosc++;
    } else {
        wartosc--;
    }

    // Blokada wartości brzegowych od -100 do 100
    if (wartosc > 100) { 
        wartosc = 100;
    };
    
    if (wartosc < -100) {
        wartosc = -100;
    };
}

int main(void) {
    displayInit();
    encoderInit();
    sei();

    while (1) {
        // Wszystko działa asynchronicznie na przerwaniach!
        // Wyświetlacz odświeża się sam w tle (Timer0)
        // Wartość zmienia się sama w tle w momencie ruchu gałką (INT0)
        // Pętla główna zgodnie z poleceniem jest pusta.
    }
}