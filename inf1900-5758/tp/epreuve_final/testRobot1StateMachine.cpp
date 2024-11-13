// #include "Pwm.h"
#include "Robot1.h" 
// #include "Connection.h"
// #include "DistanceSensor.h"
#include "Robot1PositioningStateMachine.h" 
 #include "Robot1StateMachine.h" 
// #include <util/delay.h>
// #include "IR_receive.h"
// #include "Button.h"
// #include "IR_transmission.h"
// #include "Led.h"
// #include "Sound.h"


int main() { 
//     // Initialize connections for motor control
//     Connection pwmOutputA('B', 4, true);
//     Connection pwmOutputB('B', 3, true);
//     Connection pwmDirA('B', 2, true);
//     Connection pwmDirB('B', 5, true);

//     Connection ledConnection1 = Connection('A', 0, true);
//     Connection ledConnection2 = Connection('A', 1, true);
//     Led led = Led(ledConnection1, ledConnection2);
//     led.setColor(LedColor::COLOR_OFF);


//     Connection d2('D', 2, false);
//    // Button button(IntNumber::ZERO, d2, Button::ButtonInterruptBehavior::ANY_EDGE, true);
//     Button button(IntNumber::ZERO, d2, ButtonInterruptBehavior::FALLING_EDGE, true);


//     Connection piezoGround('D', 6, true);
//     Connection piezoSignal('D', 7, true);
//     Sound snd(piezoGround, piezoSignal);

//     Connection del('A', 2, true);
//     Connection signal('A', 3, false);

//     IR_transmition transmitter(del);
//     IR_receive receiver(signal);

//     // Create the PWM controller
//     Pwm motorController(pwmDirA, pwmDirB);

//     // Create the robot control instance
//     Robot1 robot(motorController);

    // // Initialize the distance sensor (assuming it's already configured correctly)
    // DistanceSensor* distanceSensor = DistanceSensor::getInstance();

    // // Instantiate the state machine with the robot, sensor, and desired distance
    // Robot1PositioningStateMachine positioningStateMachine(robot, distanceSensor);
    // Robot1StateMachine stateMachine(positioningStateMachine, led, button, receiver, transmitter, snd);

    // while (true) {
    //     // Update the state machine, controlling the robot based on sensor input
    //     positioningStateMachine.positioningLogic();
    //     stateMachine.logic();
    // }

     Robot1StateMachine& stateMachine = Robot1StateMachine::getInstance();

    while (true) {
        stateMachine.logic();
    }

}