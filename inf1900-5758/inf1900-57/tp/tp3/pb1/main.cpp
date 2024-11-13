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

enum class LEDColors {RED, GREEN, BLACK};

const float PWM_FREQUENCY = 1000.0f;
const float PWM_PERIOD = 1 / PWM_FREQUENCY;
const uint8_t TURN_OFF_DURATION_S = 3;

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
        default:
            PORTA &= ~(1 << PORTA0);
            PORTA &= ~(1 << PORTA1);
            break;
    }
}

void wait(const double delay_ms) {
    const double microDelay = 0.1;
    const double pNeeded = delay_ms / microDelay;
    for(uint32_t i = 0; i < pNeeded; i++) {
        _delay_ms(microDelay);
    }
}

int main() {
    DDRA |= (1 << DDA0) | (1 << DDA1);
    while (true)
    {
        const float periodsNeeded = TURN_OFF_DURATION_S / PWM_PERIOD;

        for(uint32_t i = 0; i < periodsNeeded; i++) {
            float pct = 1 - i / periodsNeeded;
            float a = pct * PWM_PERIOD;
            float a_ms = a * 1000;
            float bminusa_ms = PWM_PERIOD * 1000 - a_ms;
            displayColor(LEDColors::RED);
            wait(a_ms);
            displayColor(LEDColors::BLACK);
            wait(bminusa_ms);
        }
    }
    
    return 0;
}
