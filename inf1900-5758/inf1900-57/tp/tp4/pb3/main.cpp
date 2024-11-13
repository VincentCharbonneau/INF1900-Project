#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void init(void) {
    cli();

    DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD2) | (1 << DDD3); // PD4 and PD5 are outputs

    TCNT1 = 0; // Reset the timer 1

    TCCR1A |= (1 << COM1A1) | (1 < COM1A0) | (1 << COM1B1) | (1 << COM1B0); // Set OC1A and OC1B on compare match
    
    // Set the timer 1 to phase correct PWM mode
    TCCR1A |= (1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B &= ~(1 << WGM12);

    TCCR1C = 0;

    TCCR1B &= ~((1 << CS12) | (1 << CS10));
    TCCR1B |= (1 << CS11); // Set the prescaler to 8

    PORTD &= ~(0 << PORTD2);
    PORTD &= ~(0 << PORTD3); // Set motor direction to forward

    sei();
}

void adjustPWM(uint8_t pct) {
    OCR1A = 255 - (pct * 255) / 100;
    OCR1B = 255 - (pct * 255) / 100;
}

int main() {
    init();

    adjustPWM(40);

    _delay_ms(10000);

    /*adjustPWM(25);

    _delay_ms(1000);

    adjustPWM(50);

    _delay_ms(1000);

    adjustPWM(75);

    _delay_ms(1000);

    adjustPWM(100);

    _delay_ms(1000);*/

    return 0;
}