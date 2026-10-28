#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#ifndef bit_macros
// ... (makra te same co wyżej) ...
#define _BV(bit)  (1<<(bit))
#define sbi(reg, bit) reg |= (_BV(bit))
#define cbi(reg, bit) reg &= ~(_BV(bit))
#define tbi(reg, bit) reg ^= (_BV(bit))
#define bit_is_set(sfr, bit)  ((sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!((sfr) & _BV(bit)))
#endif

#define LED1 0 // PORTB
#define LED2 1 // PORTB
#define LED3 2 // PORTB

#define SW1 0 // PIND
#define SW2 1 // PIND

int main(void) {
    // Diody jako wyjścia
    sbi(DDRB, LED1);
    sbi(DDRB, LED2);
    sbi(DDRB, LED3);

    // Przyciski jako wejścia (cbi) z włączonym rezystorem podciągającym (sbi)
    cbi(DDRD, SW1); sbi(PORTD, SW1);
    cbi(DDRD, SW2); sbi(PORTD, SW2);

    uint16_t timer_1hz = 0;
    uint8_t sw2_last_state = 1; // 1 bo podciągnięte do VCC
    uint8_t debounce_timer = 0;

    while (1) {
        _delay_ms(1);
        timer_1hz++;
        
        if(debounce_timer > 0) debounce_timer--;

        // 1. LED1 - miga 1Hz
        if (timer_1hz >= 500) {
            tbi(PORTB, LED1);
            timer_1hz = 0;
        }

        // 2. LED2 - świeci tylko gdy SW1 jest wciśnięty (zakładamy stan niski przy wciśnięciu)
        if (bit_is_clear(PIND, SW1)) {
            sbi(PORTB, LED2);
        } else {
            cbi(PORTB, LED2);
        }

        // 3. LED3 - włącznik bistabilny na SW2
        uint8_t sw2_current_state = bit_is_set(PIND, SW2) ? 1 : 0;
        
        // Wykrywanie zbocza opadającego (był 1, jest 0) + prosty debouncing
        if (sw2_last_state == 1 && sw2_current_state == 0 && debounce_timer == 0) {
            tbi(PORTB, LED3);
            debounce_timer = 50; // Zablokuj odczyt przycisku na 50ms (drgania styków)
        }
        sw2_last_state = sw2_current_state;
    }
}