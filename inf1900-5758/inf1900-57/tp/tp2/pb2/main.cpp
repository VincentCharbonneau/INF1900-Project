// Titre:       INF1900 - TP2: Probleme #2
// Auteurs:     Camil Bisson et Vincent Charbonneau
// Description: Ce programme permet d'allumer la DEL libre du robot
//              en suivant une sequence de couleurs selon l'etat de l'interrupteur
//              en utilisnt un Finite State Machine

// Identification materielle:
// - D2 (IN): Interrupteur
// - A0/A1 (OUT): DEL libre

// Table d'etats de la Finite State Machine:
// | Etat present | IN | Etat suivant | OUT    |
// |--------------|----|--------------|--------|
// | RED1         | 0  | RED1         | RED    |
// | RED1         | 1  | AMBER        | RED    |
// | AMBER        | 0  | GREEN1       | AMBER  |
// | AMBER        | 1  | AMBER        | AMBER  |
// | GREEN1       | 0  | GREEN1       | GREEN  |
// | GREEN1       | 1  | RED2         | GREEN  |
// | RED2         | 0  | BLACK        | RED    |
// | RED2         | 1  | RED2         | RED    |
// | BLACK        | 0  | BLACK        | BLACK  |
// | BLACK        | 1  | GREEN2       | BLACK  |
// | GREEN2       | 0  | RED1         | GREEN  |
// | GREEN2       | 1  | GREEN2       | GREEN  |

#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>

enum class LEDColors {RED, GREEN, AMBER, BLACK};

enum class Sequence {RED1, AMBER, GREEN1, RED2, BLACK, GREEN2};

const uint8_t AMBER_DELAY = 1;
const uint8_t DEBOUNCE_DELAY = 10;

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

LEDColors updateStateMachine(bool buttonInput, Sequence& currentSequenceState) {
    switch (currentSequenceState) {
        case Sequence::RED1:
            if(buttonInput) {
                currentSequenceState = Sequence::AMBER;
            }
            break;
        case Sequence::AMBER:
            if(!buttonInput) {
                currentSequenceState = Sequence::GREEN1;
            }
            break;
        case Sequence::GREEN1:
            if(buttonInput) {
                currentSequenceState = Sequence::RED2;
            }
            break;
        case Sequence::RED2:
            if(!buttonInput) {
                currentSequenceState = Sequence::BLACK;
            }
            break;
        case Sequence::BLACK:
            if(buttonInput) {
                currentSequenceState = Sequence::GREEN2;
            }
            break;
        case Sequence::GREEN2:
            if(!buttonInput) {
                currentSequenceState = Sequence::RED1;
            }
            break;
    }

    switch (currentSequenceState) {
        case Sequence::RED1: 
        case Sequence::RED2:
            return LEDColors::RED;
        case Sequence::AMBER:
            return LEDColors::AMBER;
        case Sequence::GREEN1:
        case Sequence::GREEN2:
            return LEDColors::GREEN;
        case Sequence::BLACK:
        default:
            return LEDColors::BLACK;
    }
}

bool getRawButtonReading(volatile int8_t pinName, volatile uint8_t pinNumber) {
    return pinName & (1 << pinNumber);
}

bool getDebouncedButtonReading(volatile int8_t pinName, volatile uint8_t pinNumber) {
    if(!getRawButtonReading(pinName, pinNumber))
        return false;

    _delay_ms(DEBOUNCE_DELAY);
    return getRawButtonReading(pinName, pinNumber);
}

int main() {
    DDRA |= (1 << DDA0) | (1 << DDA1);
    DDRD &= ~(1 << DDD2);

    Sequence currentSequenceState = Sequence::RED1;

    while (true)
    {
        bool debouncedButtonD2 = getDebouncedButtonReading(PIND, PIND2);
        LEDColors colorToDIsplay = updateStateMachine(debouncedButtonD2, currentSequenceState);
        displayColor(colorToDIsplay);
    }
    
    return 0;
}
