#include "Pwm.h"
#include "USART.h"

Pwm::Pwm(Connection& directionIOA, Connection& directionIOB) {
    directionIOA_ = &directionIOA;
    directionIOB_ = &directionIOB;
    init();
}

void Pwm::init(void) const {
    TCNT0 = 0;

    // Configure OC0A and OC0B to clear when up-matching and set when down-matching
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1); 
    TCCR0A &= ~((1 << COM0A0) | (1 << COM0B0));

    // Set the timer 0 to PWM Phase Correct
    TCCR0A |= (1 << WGM00);
    TCCR0A &= ~(1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // Set the prescaler to 8
    TCCR0B |= (1 << CS01);
    TCCR0B &= ~((1 << CS02) | (1 << CS00));
}

void Pwm::setDirectionA(bool direction) {
    directionIOA_->set(direction);
}

void Pwm::setDirectionB(bool direction) {
    directionIOB_->set(direction);
}

void Pwm::setPercentA(uint8_t pct) {
    OCR0A = (pct * 255) / 100;
}

void Pwm::setPercentB(uint8_t pct) {
    OCR0B = (pct * 255) / 100;
}
