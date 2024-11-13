/**
 * Cette classe définit un bouton qui peut être utilisé pour détecter des pressions et
 * des relâchements à l'aide d'interruptions matérielles ou par scrutation. Elle offre des
 * méthodes pour configurer le comportement de l'interruption, vérifier les connexions
 * matérielles, détecter l'état du bouton et obtenir le signal associé au bouton,
 * debounced ou non.
 *
 * Les routines ISR doivent être déclarées indépendamment de cette classe. Attention:
 * les interruptions ne gèrent pas les rebonds de bouton.
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Vincent Charbonneau, Rebecca Abi Chahine, Charles-Emile Leblond, Camil Bisson
 * 2024 (5758)
 *
 * Code qui n'est sous aucune license.
 */

#ifndef BUTTON_H
#define BUTTON_H
#include "Connection.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const char INT0_Connection_LETTER = 'D';
const uint8_t INT0_Connection_NUMBER = 2;
const char INT1_Connection_LETTER = 'D';
const uint8_t INT1_Connection_NUMBER = 3;
const char INT2_Connection_LETTER = 'B';
const uint8_t INT2_Connection_NUMBER = 2;
const uint8_t DEBOUNCE_DELAY = 10;

class Button
{
public:
    enum class ButtonInterruptBehavior
    {
        LOW_LEVEL,
        ANY_EDGE,
        ON_PRESS,
        ON_RELEASE
    };

    enum class IntNumber
    {
        ZERO,
        ONE,
        TWO
    };

    Button(IntNumber, ButtonInterruptBehavior, bool);
    bool isButtonPressedDebounced();
    bool isButtonReleasedDebounced();
    bool getButtonState();
    void enableInterrupts();
    void disableInterrupts();
    void clearAwaitingInterrupts();

private:

    bool pressedActivatesVoltage_;
    Connection connection_;
    uint8_t emisk_INTn_;
    uint8_t eifr_INTFn_;

    void setSignalLowLevelBehavior(uint8_t, uint8_t);
    void setSignalAnyEdgeBehavior(uint8_t, uint8_t);
    void setSignalFallingEdgeBehavior(uint8_t, uint8_t);
    void setSignalRisingEdgeBehavior(uint8_t, uint8_t);
    void setBehavior(Button::ButtonInterruptBehavior behavior, uint8_t, uint8_t);

    bool getButtonSignal();

    Connection determineConnection(IntNumber intNumber);
};

#endif
