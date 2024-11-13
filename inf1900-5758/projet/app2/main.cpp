#include "Robot.h"

#include "USART.h"

#include "avr/interrupt.h"

volatile uint16_t quarterSeconds = 0;
Led* ledPtr = nullptr;

ISR(TIMER1_COMPA_vect) {
    quarterSeconds += 1;
}

ISR(TIMER1_COMPB_vect) {
    if (ledPtr->getColor() == LedColor::COLOR_OFF)
        ledPtr->setColor(LedColor::COLOR_GREEN);
    else
        ledPtr->setColor(LedColor::COLOR_OFF);

    Timer::getInstance()->clearTimer();
}

int main() {
    Robot robot(quarterSeconds);
    ledPtr = robot.getLed();

    Timer::getInstance()->setDurationCompareA(250);
    Timer::getInstance()->clearTimer();
    sei();
    while (true) {    
        robot.periodic();
    }

    return 0;

}
