#include "./Connection.h"
#include "./Led.h"
#include <avr/io.h>
#include "./Button.h"

volatile LedColor ledColor;

// ISR(INT0_vect)
// {
//     ledColor = LedColor::COLOR_GREEN;
// }

int main()
{
    Connection a0('A', 0, true);
    Connection a1('A', 1, true);

    Led led(a0, a1);
    Connection d2('D', 2, false);

    Button button(IntNumber::ZERO, d2, ButtonInterruptBehavior::FALLING_EDGE, true);

    ledColor = LedColor::COLOR_OFF;

    while (true)
    {
        led.setColor(ledColor);
        if (button.isButtonPressed())
        {
             ledColor = LedColor::COLOR_GREEN;
        }
        else
        {
            ledColor = LedColor::COLOR_OFF;
    Button button(IntNumber::ZERO, d2, Button::ButtonInterruptBehavior::ANY_EDGE, true);
    while (true) {
        if (button.isButtonPressed()) {
            led.setColor(ledColor);
        } else {
            led.setColor(LedColor::COLOR_OFF);
        }
    }
    return 0;
}
