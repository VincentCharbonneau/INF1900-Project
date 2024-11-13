/*
TP2 Problème 1
Section de laboratoire : 3
Numéro d'équipe : 58
Noms :  Abi Chahine, Rebecca
        Leblond, Charles-Émile

Description du programme :
    Ce programme permet à l'utilisateur d'appuyer et relâcher le bouton sur le 
    robot 3 fois, pour que la DEL s'allume de couleur verte pendant 2 secondes,
    puis on peut recommencer.

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
const uint16_t LED_DELAY_MS = 2000;

enum class ButtonLogic
{
    PRESSED_ONCE,
    PRESSED_TWICE,
    PRESSED_THRICE,
    RELEASED_INITIAL_STATE,
    RELEASED_ONCE,
    RELEASED_TWICE,
    RELEASED_THRICE
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

void greenLed()
{
    PORTA &= ~(1 << PA1);
    PORTA |= (1 << PA0);
}

void turnOffLed()
{
    PORTA &= ~(1 << PA0);
    PORTA &= ~(1 << PA1);
}

void turnOnGreenLed()
{
    greenLed();
    _delay_ms(LED_DELAY_MS);
    turnOffLed();
}

void test(ButtonLogic &state)
{
    switch (state) 
    {
        case ButtonLogic::RELEASED_INITIAL_STATE:
            if (debounce())
                state = ButtonLogic::PRESSED_ONCE;

            break;

        case ButtonLogic::PRESSED_ONCE:
            if (!debounce())
                state = ButtonLogic::RELEASED_ONCE;

            break;

        case ButtonLogic::RELEASED_ONCE:
            if (debounce())
                state = ButtonLogic::PRESSED_TWICE;

            break;

        case ButtonLogic::PRESSED_TWICE:
            if (!debounce()) 
                state = ButtonLogic::RELEASED_TWICE;

            break;

        case ButtonLogic::RELEASED_TWICE:
            if (debounce())
                state = ButtonLogic::PRESSED_THRICE;

            break;

        case ButtonLogic::PRESSED_THRICE:
            if (!debounce())
                state = ButtonLogic::RELEASED_THRICE;

            break;

        case ButtonLogic::RELEASED_THRICE:
            turnOnGreenLed();

            state = ButtonLogic::RELEASED_INITIAL_STATE;

            break;

    }
}

int main()
{
    DDRA |= 1 << PA0;
    DDRA |= 1 << PA1;
    DDRD &= ~(1 << PD2);

    ButtonLogic state = ButtonLogic::RELEASED_INITIAL_STATE;
    
    while (true) {
        test(state);
    }
}
