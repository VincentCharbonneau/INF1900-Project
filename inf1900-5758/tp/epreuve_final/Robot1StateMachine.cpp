#include "Robot1StateMachine.h"

Robot1StateMachine::Robot1StateMachine()
      : positioningStateMachine(Robot1::getInstance(), DistanceSensor::getInstance()) {
}
Robot1StateMachine& Robot1StateMachine::getInstance() {
    static Robot1StateMachine instance; // Guaranteed to be destroyed. Instantiated on first use.
    return instance;
} 

void Robot1StateMachine::logic() {
    switch (currentState) {
        case RobotState::INITIALIZATION:
            currentState = RobotState::POSITIONING;
            break;

        case RobotState::POSITIONING:
            if (robotPositioningStateMachine.getCurrentState() == RobotPositioningState::MISSION_COMPLETE) {  
                currentState = RobotState::WAITING;
            }
            break;

        case RobotState::WAITING:
            led.setColor(LedColor::COLOR_RED); 
            if (button.isButtonPressedDebounced()) {
                playWaitingSound();
                currentState = RobotState::RECEIVING;
            }
            break;

        case RobotState::RECEIVING:
            led.setColor(LedColor::COLOR_OFF); 
            irData=irReceiver.irRead();
            if (irData != 0) { // Commencez la réception des données. Supposons que 0 indique une réception infructueuse.
                currentState = RobotState::SENDING;
            } else { 
                playErrorSound(); 
                currentState = RobotState::WAITING; 
            }
            break;

        case RobotState::SENDING:
            led.setColor(LedColor::COLOR_GREEN); 
            if (button.isButtonPressedDebounced()) {
            //    USART::getInstance()->print("Données envoyées au PC\n");
               // blinkingLed(true); // clignoter?
                irTransmitter.sendData(irData); 
               // blinkingLed(false);
                currentState = RobotState::SENDING; 
            }
            break;
    }
}

void Robot1StateMachine::playWaitingSound() {
    for (uint8_t i = 0; i < 3; ++i) {
        sound.playNote(81); // Joue un son aigu
        _delay_ms(50); 
        sound.stop(); 
        _delay_ms(50); 
    }
}   

void Robot1StateMachine::playErrorSound() {
    sound.playNote(45); // La note la plus grave 
    _delay_ms(1000); 
    sound.stop(); 
}

// void Robot1StateMachine::blinkingLed(bool isSendingData) {
//     while (isSendingData) {
//         led.setColor(LedColor::COLOR_OFF);git st
//         _delay_ms(100);
//         led.setColor(LedColor::COLOR_GREEN);
//         _delay_ms(100);
//     }
// }
