#include <util/delay.h>
#include <avr/io.h>
#include <Connection.h>
#include <Led.h>
#include <avr/interrupt.h>
#include <Timer.h>
#include <Can.h>
#include <Pwm.h>
#include <Button.h>
#include <IR_receive.h>

Connection b2('B', 2, false);
Connection b3('B', 3, false);
Connection a0('A', 0, true);
Connection a1('A', 1, true);

Led led(a0, a1);

enum class State {
    WAIT, RECEVING
};


int main() {
    IR_receive receiver(b2);

    State States = State::WAIT;
    while (true) {
        switch (States) {
            case State::WAIT:
                if (!b2.get())
                {
                    _delay_us(500);
                    if (!b2.get())
                    {
                        States = State::RECEVING;
                    }
                }
                break;

            case State::RECEVING:
                uint16_t data = receiver.irRead();
                USART::getInstance()->print("Data", data);
                States = State::WAIT;
                _delay_ms(500);
                break;
            }
        }
    return 0;
}
