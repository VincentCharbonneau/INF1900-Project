#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

const uint8_t AMBER_COLOUR_GREEN_LED_MS = 30;
const uint8_t AMBER_COLOUR_RED_LED_MS = 5;
const uint8_t DEBOUNCE_DELAY_MS = 10;

bool isButtonPressed()
{
    return PIND & (1 << PD2);
}

bool debounce()
{
    bool lecture = isButtonPressed();

    _delay_ms(DEBOUNCE_DELAY_MS);
    
    return isButtonPressed() && lecture;
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

void turnOnAmberLed()
{
    turnOnGreenLed();
    _delay_ms(AMBER_COLOUR_GREEN_LED_MS);
    turnOnRedLed();
    _delay_ms(AMBER_COLOUR_RED_LED_MS);
}

int main()
{
    DDRA |= 1 << PA0 | 1 << PA1;
    DDRD &= ~(1 << PD2);

    while (true)
    {
        turnOffLed();
        if (debounce()) {
            while (debounce())
            {
                turnOnAmberLed();
            }
        }
    }
}
