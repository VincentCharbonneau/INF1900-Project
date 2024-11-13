#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t B = 100;
const uint8_t TOP = 255;
const uint8_t INCREMENT = 25;

void turnOnMotor()
{
    PORTD |= (1 << PD4);
    PORTD |= (1 << PD5);
}

void ajustPwm(uint8_t ratioAB)
{
    OCR1A = ratioAB * TOP / B;
    OCR1B = ratioAB * TOP / B;

    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B |= (1 << CS11);
    TCCR1C = 0;
}

int main()
{
    DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);

    while (true)
    {
        turnOnMotor();
        
        for (uint8_t i = B; i >= 0; i -= INCREMENT)
        {
            ajustPwm(i);
            _delay_ms(2000);
        }
    }
}
