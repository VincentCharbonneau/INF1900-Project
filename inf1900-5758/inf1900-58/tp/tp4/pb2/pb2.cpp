#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t gTimerExpired;
volatile uint8_t gPushButton;
const uint16_t DELAY = 7812;

bool isButtonPressed()
{
    return PIND & (1 << PD2);
}

void turnOnGreenLed()
{
    PORTA &= ~(1 << PA1);
    PORTA |= (1 << PA0);
}

void turnOnRedLed()
{
    PORTA &= ~(1 << PA0);
    PORTA |= (1 << PA1);
}

void turnOffLed()
{
    PORTA &= ~(1 << PA0);
    PORTA &= ~(1 << PA1);
}

void reflexLed()
{
    turnOnRedLed();
    _delay_ms(100);
    turnOffLed();
}

void startTimer(uint16_t duree)
{
    gTimerExpired = 0;

    TCNT1 = 0;

    OCR1A = duree;

    TCCR1A = 0;
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);
    TCCR1C = 0;

    TIMSK1 |= (1 << OCIE1A);
}

void initialisation(void)
{
    cli();

    DDRA |= 1 << PA0;
    DDRA |= 1 << PA1;
    DDRD &= ~(1 << PD2);

    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC00);

    sei();
}

ISR(TIMER1_COMPA_vect)
{
    gMinuterieExpiree = 1;
}

ISR(INT0_vect)
{
    gBoutonPoussoir = 1;

    EIFR |= (1 << INTF0);
}

int main()
{
    initialization();
    _delay_ms(10000);
    gBoutonPoussoir = 0;
    reflexLed();

    partirMinuterie(DELAY);

    do
    {
    } while (gMinuterieExpiree == 0 && gBoutonPoussoir == 0);

    cli();


    if (gBoutonPoussoir)
        turnOnGreenLed();
    else
        turnOnRedLed();
    }
