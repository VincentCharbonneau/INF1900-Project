/*
TP2 Problème 2
Section de laboratoire : 3
Numéro d'équipe : 58
Noms :  Abi Chahine, Rebecca
        Leblond, Charles-Émile

Description du programme :
    La DEL s'allume initialement en rouge. Si on pèse sur le bouton, 
    la DEL affiche la couleur ambre. Si on relâche le bouton, la DEL devient verte. 
    Si on pèse à nouveau sur le bouton, la DEL devient rouge. Quand on relâche le bouton, 
    la DEL s'éteint. Si le bouton est pesé pour une troisième fois, la DEL affiche 
    la couleur verte. Finalement, si on relâche le bouton, la DEL devient rouge, 
    et on est de retour à l'état initial. On peut recommencer.

Identifications matérielles (Broches I/O) :
    A0 : Entrée
    A1 : Entrée
    D2 : Sortie

    A0 et A1 servent a créer une différence de potentiel
    D2 sert à allumer la DEL

Table d'états TP2_1 :
 +========================+==========+========================+=============+
 |       État présent     |  Entrées |      Prochain état     |   Sorties   |
 |                        | (Bouton) |                        | (LED verte) |
 +========================+==========+========================+=============+
 | BUTTON_PRESSED_ONCE    |     0    | BUTTON_RELEASED_ONCE   |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_PRESSED_ONCE    |     1    | BUTTON_PRESSED_ONCE    |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_PRESSED_TWICE   |     0    | BUTTON_RELEASED_TWICE  |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_PRESSED_TWICE   |     1    | BUTTON_PRESSED_TWICE   |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_PRESSED_THRICE  |     0    | BUTTON_RELEASED_THRICE |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_PRESSED_THRICE  |     1    | BUTTON_PRESSED_THRICE  |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_RELEASED_       |     0    | BUTTON_RELEASED_       |      0      |
 | INITIAL_STATE          |          | INITIAL_STATE          |             |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_RELEASED_       |     1    | BUTTON_PRESSED_ONCE    |      0      |
 | INITIAL_STATE          |          |                        |             |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_RELEASED_ONCE   |     0    | BUTTON_RELEASED_ONCE   |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_RELEASED_ONCE   |     1    | BUTTON_PRESSED_TWICE   |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_RELEASED_TWICE  |     0    | BUTTON_RELEASED_TWICE  |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_RELEASED_TWICE  |     1    | BUTTON_PRESSED_THRICE  |      0      |
 +------------------------+----------+------------------------+-------------+
 | BUTTON_RELEASED_THRICE |     X    | BUTTON_RELEASED_       |      1      |
 |                        |          | INITIAL_STATE          |             |
 +------------------------+----------+------------------------+-------------+
*/

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

const uint8_t DEBOUNCE_DELAY_MS = 10;
const uint8_t AMBER_COLOUR_GREEN_LED_MS = 30;
const uint8_t AMBER_COLOUR_RED_LED_MS = 5;

enum class ButtonState
{
    PRESSED_ONCE,
    PRESSED_TWICE,
    PRESSED_THRICE,
    RELEASED_INITIAL_STATE,
    RELEASED_ONCE,
    RELEASED_TWICE,
};

bool isButtonPressed()
{
    return PIND & (1 << PD2);
}

bool debounce()
{
    bool lecture = isButtonPressed();

    _delay_ms(DEBOUNCE_DELAY_MS);

    return isButtonPressed() && lecture;
}

void turnOnGreenLed()
{
    PORTA &= ~(1 << PA1);
    PORTA |= (1 << PA0);
}

void turnOnRedLed()
{
    PORTA &= ~(1 << PA0);
    PORTA |= (1 << PA1);
}

void turnOffLed()
{
    PORTA &= ~(1 << PA0);
    PORTA &= ~(1 << PA1);
}

void turnOnAmberLed()
{
    while (debounce())
    {
        turnOnGreenLed();
        _delay_ms(AMBER_COLOUR_GREEN_LED_MS);
        turnOnRedLed();
        _delay_ms(AMBER_COLOUR_RED_LED_MS);
    }
}            

void buttonLogic(ButtonState& state)
{
    switch (state)
    {
        case ButtonState::RELEASED_INITIAL_STATE:
            turnOnRedLed();
            if (debounce())
                state = ButtonState::PRESSED_ONCE;
            
            break;

        case ButtonState::PRESSED_ONCE:
            turnOnAmberLed();
            if (!debounce())
                state = ButtonState::RELEASED_ONCE;

            break;
            
        case ButtonState::RELEASED_ONCE:
            turnOnGreenLed();
            if (debounce())
                state = ButtonState::PRESSED_TWICE;
            
            break;
            
        case ButtonState::PRESSED_TWICE:
            turnOnRedLed();
            if (!debounce())
                state = ButtonState::RELEASED_TWICE;

            break;

        case ButtonState::RELEASED_TWICE:
            turnOffLed();
            if (debounce())
                state = ButtonState::PRESSED_THRICE;

            break;

        case ButtonState::PRESSED_THRICE:
            turnOnGreenLed();
            if (!debounce())
                state = ButtonState::RELEASED_INITIAL_STATE;

            break;
    }
}

int main()
{
    DDRA |= 1 << PA0;
    DDRA |= 1 << PA1;
    DDRD &= ~(1 << PD2);

    ButtonState state = ButtonState::RELEASED_INITIAL_STATE;

    while (true)
    {
        buttonLogic(state);
    }
}
