#ifndef ROBOT1_STATE_MACHINE_H
#define ROBOT1_STATE_MACHINE_H

#include "Robot1PositioningStateMachine.h"
// #include "IR_receive.h"
// #include "Button.h"
// #include "IR_transmission.h"
// #include "Led.h"
// #include "Sound.h"

enum class RobotState {
    INITIALIZATION,
    POSITIONING,
    WAITING,
    RECEIVING,
    SENDING,    
};

class Robot1StateMachine { 
private:
    uint16_t irData=0;
    Robot1PositioningStateMachine& robotPositioningStateMachine;
    RobotState currentState = RobotState::INITIALIZATION;
    // Led& led;
    // Button& button;
    // IR_receive& irReceiver;
    // IR_transmission& irTransmitter;
    // Sound& sound;
    Robot1StateMachine();
    Robot1StateMachine(const Robot1StateMachine&) = delete; // Prevent copy-construction
    Robot1StateMachine& operator=(const Robot1StateMachine&) = delete; // Prevent assignment
    void playWaitingSound();
    void playErrorSound();
   // void blinkingLed(bool );

public:
   
    static Robot1StateMachine& getInstance();
    void logic();
};
#endif 
