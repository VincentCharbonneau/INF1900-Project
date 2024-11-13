#include "USART.h"
#include "avr/interrupt.h"
#include "Timer.h"
#include "Connection.h"
#include "util/delay.h"
#include "IRTransmitter.h"
#include "Line.h"

volatile uint16_t quarterSeconds = 0;
volatile bool pwmActive = true;

ISR(TIMER1_COMPA_vect) {
    quarterSeconds += 1;
}

const uint8_t DELAY = 100;

int main() {
    sei();
    
    Line line(120, 2, true);
    line.setLineType(LineLength::MEDIUM);

    Line line2(28, 4, false);
    line.setLineType(LineLength::SMALL);

    Line line3(32, 4, false);
    line.setLineType(LineLength::LONG);

    IRTransmitter::getInstance()->sendLine(line, false);
    _delay_ms(1000);
    IRTransmitter::getInstance()->sendLine(line2);
    _delay_ms(1000);
    IRTransmitter::getInstance()->sendLine(line3, true);
    _delay_ms(1000);
    
    return 0;
}