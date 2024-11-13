// Titre:       INF1900 - TP6: Probleme #1
// Auteurs:     Camil Bisson et Vincent Charbonneau
// Description: Ce programme permet de faire clignoter la DEL libre
//              un certain nombre de fois selon la duree d'appui
//              du bouton poussoir sur le breadboard, pour une
//              duree maximale de 12s

// Identification materielle des ports et pins:
// - D2 (IN): Interrupteur
// - B0/B1 (OUT): DEL libre

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

enum class LedColor {
    RED, 
    GREEN, 
    BLACK
};

const uint8_t DEBOUNCE_DELAY = 50;

const uint16_t TIMER_PRESCALAR = 1024;
const uint16_t TIMER_COUNTS_10MS = F_CPU / 1024 / 10;

enum class Sequence {INITIAL, COUNTING, SEQUENCE_END};
volatile Sequence currentState = Sequence::INITIAL;

const uint8_t COUNTER_MAX = 120;
volatile uint8_t counter = 0;
volatile bool counterEnable = false;

bool getRawButtonReading(volatile int8_t pinName, volatile uint8_t pinNumber) {
    return !(pinName & (1 << pinNumber));
}

void displayColor(LedColor color) {
    switch(color) {
        case LedColor::RED:
            PORTB &= ~(1 << PORTB0);
            PORTB |= (1 << PORTB1);
            break;
        case LedColor::GREEN:
            PORTB |= (1 << PORTB0);
            PORTB &= ~(1 << PORTB1);
            break;
        case LedColor::BLACK:
            PORTB &= ~(1 << PORTB0);
            PORTB &= ~(1 << PORTB1);
            break;
    }
}

void init(void) {
    cli();
    DDRB |= (1 << DDB0) | (1 << DDB1);
    DDRD &= ~(1 << DDD2);

    // ENABLES EXTERNAL INTERRUPT 0
    EIMSK |= (1 << INT0);

    // INTERRUPT FOR ALL EDGES
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    sei();
}

void startTimer(uint16_t duration) {
    TCNT1 = 0; // Reset the timer 1

    // What to compare the timer 1 to (A)
    OCR1A = duration;

    // Configure the timer 1 to CTC 
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |= (1 << WGM12);

    TCCR1B |= (1 << CS12) | (1 << CS10); 
    TCCR1B &= ~(1 << CS11); // Set the prescaler to 1024

    TCCR1C = 0; 

    TIMSK1 |= (1 << OCIE2A); // Enable the interrupt
} 

ISR(INT0_vect) {
    _delay_ms(DEBOUNCE_DELAY);

    bool buttonPressed = getRawButtonReading(PIND, PIND2);

    if(buttonPressed && currentState == Sequence::INITIAL) {
        currentState = Sequence::COUNTING;
        counter = 0;
        startTimer(TIMER_COUNTS_10MS); 
        counterEnable = true;
    }
    if(!buttonPressed && currentState == Sequence::COUNTING) {
        currentState = Sequence::SEQUENCE_END;
        counterEnable = false;
    }

    // CLEAR AWAITING INTERRUPTS
    EIFR |= (1 << INTF0);
}


ISR(TIMER1_COMPA_vect) {
    if(counterEnable) {
        counter++;
        if(counter >= COUNTER_MAX) {
            counterEnable = false;
            currentState = Sequence::SEQUENCE_END;
        }
        else {
            startTimer(TIMER_COUNTS_10MS);
        }
    }
}

void sequenceEnd(void) {
    cli();
    displayColor(LedColor::GREEN);
    _delay_ms(500);
    displayColor(LedColor::BLACK);
    _delay_ms(2000);
    for(int i = 0; i < counter / 2; i++) {
        displayColor(LedColor::RED);
        _delay_ms(250);
        displayColor(LedColor::BLACK);
        _delay_ms(250);
    }
    displayColor(LedColor::GREEN);
    _delay_ms(1000);
    displayColor(LedColor::BLACK);

    currentState = Sequence::INITIAL;
    sei();
}

int main() {
    init();
    while(true) {
        if(currentState == Sequence::SEQUENCE_END) {
            sequenceEnd();
        }
    }

    return 0;
}


