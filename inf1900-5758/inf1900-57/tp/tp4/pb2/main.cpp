// Titre:       INF1900 - TP4: Probleme #2
// Auteurs:     Camil Bisson et Vincent Charbonneau
// Description: Jeu de reflexe

// Identification materielle:
// - D2 (IN): Interrupteur
// - A0/A1 (OUT): DEL libre

#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

enum class LEDColors {RED, GREEN, AMBER, BLACK};

const uint8_t AMBER_DELAY = 1;
const uint8_t DEBOUNCE_DELAY = 50;
const uint16_t INITIAL_DELAY = 10000;
const uint8_t RED_COLOR_DELAY = 100;

volatile bool gTimerExpired = false;
volatile bool gButtonPressed = false;

void displayColor(LEDColors color) {
    switch(color) {
        case LEDColors::RED:
            PORTA &= ~(1 << PORTA0);
            PORTA |= (1 << PORTA1);
            break;
        case LEDColors::GREEN:
            PORTA |= (1 << PORTA0);
            PORTA &= ~(1 << PORTA1);
            break;  
        case LEDColors::AMBER:
            displayColor(LEDColors::RED);
            _delay_ms(AMBER_DELAY);

            displayColor(LEDColors::GREEN);
            _delay_ms(AMBER_DELAY);
            break;  
        case LEDColors::BLACK:
            PORTA &= ~(1 << PORTA0);
            PORTA &= ~(1 << PORTA1);
            break;
    }
}

bool getRawButtonReading(volatile int8_t pinName, volatile uint8_t pinNumber) {
    return pinName & (1 << pinNumber);
}

ISR(INT0_vect) {
    _delay_ms(DEBOUNCE_DELAY);

    gButtonPressed = getRawButtonReading(PIND, PIND2);

    EIFR |= (1 << INTF0);
}


ISR(TIMER1_COMPA_vect) {
    gTimerExpired = true;
}

void startTimer(uint16_t duration) {
    gTimerExpired = false;

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

void init(void) {
    cli();
    DDRA |= (1 << DDA0) | (1 << DDA1);
    DDRD &= ~(1 << DDD2);

    // ENABLES EXTERNAL INTERRUPT 0
    EIMSK |= (1 << INT0);

    // RISING EDGE FOR EXTERNAL INTERRUPT 0
    EICRA |= (0 << ISC01) | (1 << ISC00);
    sei();
}

int main() {
    init();

    displayColor(LEDColors::BLACK);

    cli();

    _delay_ms(INITIAL_DELAY);
    displayColor(LEDColors::RED);
    _delay_ms(RED_COLOR_DELAY);
    displayColor(LEDColors::BLACK);


    sei();

    startTimer(8000000/1024); // 1 second timer

    do {

    } while(!gTimerExpired && !gButtonPressed);
    
    cli();

    if(gTimerExpired) {
        displayColor(LEDColors::RED);
    } else {
        displayColor(LEDColors::GREEN);
    }

    return 0;
}
