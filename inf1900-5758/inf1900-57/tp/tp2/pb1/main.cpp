// Titre:       INF1900 - TP2: Probleme #1
// Auteurs:     Camil Bisson et Vincent Charbonneau
// Description: Ce programme permet d'allumer la DEL libre en rouge
//              pendant deux secondes apres avoir appuye et relache 
//              l'interrupteur trois fois

// Identification materielle:
// - D2 (IN): Interrupteur
// - A0/A1 (OUT): DEL libre

// Table d'etats de la Finite State Machine du compteur
// | Etat present | IN | Etat suivant | OUT |
// |--------------|----|--------------|-----|
// | ZERO_PRESS   | 0  | ZERO_PRESS   | 0   |
// | ZERO_PRESS   | 1  | ONE_PRESS    | 0   |
// | ONE_PRESS    | 0  | ONE_PRESS    | 0   |
// | ONE_PRESS    | 1  | TWO_PRESS    | 0   |
// | TWO_PRESS    | 0  | TWO_PRESS    | 0   |
// | TWO_PRESS    | 1  | ZERO_PRESS   | 1   |

// Table d'etats de la Finite State Machine du relachement de l'interrupteur
// | Etat present | IN | Etat suivant | OUT |
// |--------------|----|--------------|-----|
// | NOT_PRESSED  | 0  | NOT_PRESSED  | 0   |
// | NOT_PRESSED  | 1  | PRESSED      | 0   |
// | PRESSED      | 0  | NOT_PRESSED  | 1   |
// | PRESSED      | 1  | PRESSED      | 0   |

#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>

enum class LEDColors {RED, BLACK};

enum class ButtonReleased { NOT_PRESSED, PRESSED };

enum class Counter { ZERO_PRESS, ONE_PRESS, TWO_PRESS };

const uint8_t DEBOUNCE_DELAY = 10;
const uint16_t DISPLAY_RED_DELAY = 2000;

void displayColor(const LEDColors& color) {
    switch(color) {
        case LEDColors::RED:
            PORTA &= ~(1 << PORTA0);
            PORTA |= (1 << PORTA1);
            break;
        case LEDColors::BLACK:
            PORTA &= ~(1 << PORTA0);
            PORTA &= ~(1 << PORTA1);
            break;
    }
}

bool updateCounterStateMachine(bool input, Counter& currentCounterState) {
    switch(currentCounterState) {
        default:
            if(input) {
                currentCounterState = Counter::ONE_PRESS;
            }
            return false;
        case Counter::ONE_PRESS:
            if(input) {
                currentCounterState = Counter::TWO_PRESS;
            }
            return false;
        case Counter::TWO_PRESS:
            if(input) {
                currentCounterState = Counter::ZERO_PRESS;
                return true;
            }
            return false;
    }
}

bool getButtonReleased(bool input, ButtonReleased& currentButtonReleasedState) {
    switch (currentButtonReleasedState) {
        default:
            if(input) {
                currentButtonReleasedState = ButtonReleased::PRESSED;
            }
            return false;
        case ButtonReleased::PRESSED:
            if(!input) {
                currentButtonReleasedState = ButtonReleased::NOT_PRESSED;
                return true;
            }
            return false;
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
    DDRD &= ~(1 << DDD0);

    ButtonReleased currentButtonReleasedState = ButtonReleased::NOT_PRESSED;
    Counter currentCounterState = Counter::ZERO_PRESS;

    while (true)
    {
        bool debouncedButtonD2 = getDebouncedButtonReading(PIND, PIND2);
        bool buttonD2Released = getButtonReleased(debouncedButtonD2, currentButtonReleasedState);

        if(updateCounterStateMachine(buttonD2Released, currentCounterState)) {
            displayColor(LEDColors::RED);
            
            _delay_ms(DISPLAY_RED_DELAY);
            
            displayColor(LEDColors::BLACK);
        }
    }
    
    return 0;
}
