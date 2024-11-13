#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

const double FREQUENCY = 1000;
const double PERIOD = 1/FREQUENCY;
const uint16_t S_TO_MS = 1000;
const double B = 100;
const double DELAY_MS = ((1/FREQUENCY)*S_TO_MS)/B;
const double TIME = 3;
const double NB_REPETITIONS_DOUBLE = (TIME/(B+1))/PERIOD;
const int8_t NB_REPETITIONS_INT8 = int8_t(NB_REPETITIONS_DOUBLE);

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

void delay_ms(uint8_t delay)
{
    for (uint8_t i = 0; i < delay; i++){
        _delay_ms(DELAY_MS);
    }
}

int main()
{
    DDRA |= 1 << PA0;
    DDRA |= 1 << PA1;

    while (true)
    {
        for (uint8_t i = 0; i <= B; i ++)
        {
            float test1 = B - i;
            float test2 = 0 + i;
           
            for (int8_t j = 0; j < NB_REPETITIONS_INT8; j++)
            {
                turnOnRedLed();
                delay_ms(test1);
                turnOffLed();
                delay_ms(test2);
            }
        }
    }
}
