/*
* TP7
* Section de laboratoire : 3
* Numéro d'équipe : 5758
* Noms :  Vincent Charbonneau, 
*         Rebecca Abi Chahine, 
*         Charles-Emile Leblond, 
*         Camil Bisson
*
* Description du programme :
*    
*   Ce programme est un exemple d'utilisation des classes de base de notre librairie. 
*   Il utilise les classes Connection, Led, Can, Timer, Pwm et Button.
*   Seulement memoire_24 n'est pas utilisée.
*
* Identifications matérielles (Broches I/O) :
*   
*   A0 : Entrée
*        Utilisée pour la conversion analogique
*
*   B0 : Sortie
*   B1 : Sortie
*        B0 et B1 sont utilisées respectivemet comme connections
*        négatives et positives pour la DEL
*
*   B2 : Sortie
*   B3 : Sortie
*   B4 : Sortie
*   B5 : Sortie
*        Les paires de broches B2 et B3 et B4 et B5 sont utilisées
*        pour contrôler les moteurs en PWM. Chaque paire correspond
*        à un moteur. B2 et B5 sont les broches de direction et B3
*        et B4 sont les broches de PWM.
*
*   D2 : Entrée
*        D2 est utilisée pour le bouton. La connection est crée
*        directement dans le constructeur de la classe Button.
*   
*/

#include <util/delay.h>
#include <avr/io.h>
#include <Connection.h>
#include <Led.h>
#include <avr/interrupt.h>
#include <Timer.h>
#include <Can.h>
#include <Pwm.h>
#include <Button.h>

#ifdef DEBUG
# define DEBUG_PRINT(x) Debugger::getInstance()->print(x)
#else
# define DEBUG_PRINT(x) do {} while (0) // code mort
#endif

const uint8_t CAN_PORT_NUMBER = 0;
const uint8_t LOW_MOTOR_SPEED = 30;

const uint16_t TIMERA_TEST_DELAY = 3000;
const uint16_t TIMERB_TEST_DELAY = 2000;

Led* mainLed = nullptr;
Button* mainButton = nullptr;

ISR(TIMER1_COMPA_vect) {
    mainLed->setColor(LedColor::COLOR_GREEN);
    uint16_t reading = Can::getInstance()->lecture(CAN_PORT_NUMBER);
    Debugger::getInstance()->print("CAN Reading", reading >> 2);
}

ISR(TIMER1_COMPB_vect) {
    mainLed->setColor(LedColor::COLOR_RED);
}

ISR(INT0_vect) {
    mainButton->disableInterrupts();
    
    if(mainButton->isButtonPressedDebounced()) {
        Debugger::getInstance()->print("Button pressed and debounced!\n");
    }

    mainButton->enableInterrupts();
    mainButton->clearAwaitingInterrupts();
}

int main() {
	_delay_ms(4000);

    DEBUG_PRINT("ceci est debug print!!");

    cli();

	Connection c1('B', 0, true);
	Connection c2('B', 1, true);
	Led led(c1, c2);
	mainLed = &led;

    Connection canIO('A', 0, false);

    Connection pwmOutputA('B', 4, true);
    Connection pwmOutputB('B', 3, true);
    Connection pwmDirA('B', 2, true);
    Connection pwmDirB('B', 5, true);

    Button button(Button::IntNumber::ZERO, Button::ButtonInterruptBehavior::ON_PRESS, true); // D2
    mainButton = &button;

    Pwm pwm(pwmDirA, pwmDirB);
    pwm.setPercentA(LOW_MOTOR_SPEED);
    pwm.setPercentB(LOW_MOTOR_SPEED);

    Debugger::getInstance()->print("OCR0A", OCR0A);
    Debugger::getInstance()->print("OCR0B", OCR0B);

    sei();

    Timer::getInstance()->clearTimer();
    Timer::getInstance()->setDurationCompareA(TIMERA_TEST_DELAY);
    Timer::getInstance()->setDurationCompareB(TIMERB_TEST_DELAY);
	
    while(true) {}

    return 0;
}

