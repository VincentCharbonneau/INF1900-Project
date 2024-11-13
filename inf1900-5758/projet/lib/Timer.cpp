#include "Timer.h"
#include "USART.h"

Timer Timer::instance_;

Timer* Timer::getInstance() {
    return &instance_;
}

Timer::Timer() {
    TCNT1 = 0;

    // By default, set the timer mode to CTC 
    setMode(TimerModes::CTC);

    DDRD |= (1 << DDD5); // Set the OC1A pin as output

    setConnectionMode(ConnectionMode::TOGGLE_MATCH);

    prescalar_ = 1024;
    TCCR1B |= (1 << CS12) | (1 << CS10); 
    TCCR1B &= ~(1 << CS11); // Set the prescaler to 1024
    TCCR1C = 0;
}

void Timer::setPrescalar1024() {
    prescalar_ = 1024;
    TCCR1B |= (1 << CS12) | (1 << CS10); 
    TCCR1B &= ~(1 << CS11); // Set the prescaler to 1024
}

void Timer::setPrescalar1() {
    prescalar_ = 1;
    TCCR1B &= ~((1 << CS12) | (1 << CS11));
    TCCR1B |= (1 << CS10); // Set the prescaler to 1
}

void Timer::setMode(TimerModes mode) {
    mode_ = mode;

	switch(mode) {
		case TimerModes::CTC:
			TCCR1A &= ~(1 << WGM10);
			TCCR1A &= ~(1 << WGM11);
			TCCR1B |= (1 << WGM12);
			TCCR1B &= ~(1 << WGM13);
			break;
		case TimerModes::NORMAL:
			TCCR1A &= ~(1 << WGM10);
			TCCR1A &= ~(1 << WGM11);
			TCCR1B &= ~(1 << WGM12);
			TCCR1B &= ~(1 << WGM13);
			break;
	}
}

void Timer::setConnectionMode(const ConnectionMode mode) {
    switch(mode) {
        case ConnectionMode::TOGGLE_MATCH:
            TCCR1A &= ~(1 << COM1A1); 
            TCCR1A |= (1 << COM1A0); // Toggle OC1A on compare match
            break;
        case ConnectionMode::OFF:
            TCCR1A &= ~(1 << COM1A1); 
            TCCR1A &= ~(1 << COM1A0); // Disconnect OC1A
            break;
    }
}

void Timer::setDurationCompareA(const uint16_t duration_ms) {
    setCountCompareA(durationMsToCount(duration_ms));
}

void Timer::setCountCompareA(const uint16_t count) {
    if(count == 0) {
        TIMSK1 &= ~(1 << OCIE1A);
        return;
    }

    countCompareA_ = count;
    OCR1A = count;
    TIMSK1 |= (1 << OCIE1A);
}

void Timer::setDurationCompareB(const uint16_t duration_ms) {
    setCountCompareB(durationMsToCount(duration_ms));
}

void Timer::setCountCompareB(const uint16_t count) {
    if(count == 0) {
        TIMSK1 &= ~(1 << OCIE1B);
        return;
    }

    countCompareB_ = count;
    OCR1B = count;
    TIMSK1 |= (1 << OCIE1B);
}

const uint16_t Timer::durationMsToCount(const uint16_t duration_ms) {
    return duration_ms * (F_CPU / prescalar_) / 1000;
}

void Timer::clearTimer() {
    TCNT1 = 0;
}

const uint16_t Timer::getPrescalar() const {
    return prescalar_;
}

const uint16_t Timer::getCount() const {
    return TCNT1;
}

const uint16_t Timer::getCountCompareA() const {
    return countCompareA_;
}

const uint16_t Timer::getCountCompareB() const {
    return countCompareB_;
}
