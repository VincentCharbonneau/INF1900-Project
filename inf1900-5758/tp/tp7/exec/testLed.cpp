#define F_CPU 8000000UL

#include <util/delay.h>

#include "./Connection.h"

#include "./Led.h"

#include <avr/io.h>

int main() {

    Connection a0('A', 0, true);

    Connection a1('A', 1, true);

    LedColor ledColor = LedColor::COLOR_GREEN;

    Led led(a0, a1);

    while (true)

    {

        led.setColor(ledColor);

        _delay_ms(1000);

        ledColor = LedColor::COLOR_RED;

        _delay_ms(1000);
    }
    return 0;

}
