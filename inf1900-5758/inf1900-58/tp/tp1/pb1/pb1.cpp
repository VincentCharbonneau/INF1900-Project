#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

const uint8_t AMBER_COLOUR_GREEN_LED_MS = 30;
const uint8_t AMBER_COLOUR_RED_LED_MS = 5;

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

void turnOnAmberLed()
{
    for (int i = 0; i < 90; i++)
    {
        turnOnGreenLed();
        _delay_ms(AMBER_COLOUR_GREEN_LED_MS);
        turnOnRedLed();
        _delay_ms(AMBER_COLOUR_RED_LED_MS);
    }
}

int main()
{
    DDRA |= 1 << PA0 | 1 << PA1;

    while (true)
    {
        turnOnGreenLed();
        _delay_ms(3000);
        turnOnRedLed();
        _delay_ms(3000);
        turnOnAmberLed();
    }
}
