#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
 
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

// Tablica wzorów 7-segmentowych
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
    0b01101111  //9
};

uint16_t liczba = 1234; 

void obsluga_wyswietlacza(void) {
    static uint8_t aktualna_cyfra = 0; 
    
    uint8_t tysiace = liczba / 1000;
    uint8_t setki = (liczba / 100) % 10;
    uint8_t dziesiatki = (liczba / 10) % 10;
    uint8_t jednosci = liczba % 10;

    PORTC &= ~0x0F; 

    if (aktualna_cyfra == 0) {
        PORTD = tab[tysiace];
        sbi(PORTC, PC0);
    } 
    else if (aktualna_cyfra == 1) {
        PORTD = tab[setki];
        sbi(PORTC, PC1);
    } 
    else if (aktualna_cyfra == 2) {
        PORTD = tab[dziesiatki];
        sbi(PORTC, PC2);
    } 
    else if (aktualna_cyfra == 3) {
        PORTD = tab[jednosci];
        sbi(PORTC, PC3);
    }

    aktualna_cyfra++;
    
    if (aktualna_cyfra > 3) {
        aktualna_cyfra = 0;
    }
}

int main(){
    DDRD = 0xFF;
    sbi(DDRC, PC0);
    sbi(DDRC, PC1);
    sbi(DDRC, PC2);
    sbi(DDRC, PC3);

    while(1) {
        obsluga_wyswietlacza();
        _delay_ms(5); 
    }
}