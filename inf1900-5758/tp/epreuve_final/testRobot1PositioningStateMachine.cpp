// #include "Pwm.h"
#include "Robot1.h" 
// #include "Connection.h"
// #include "DistanceSensor.h"
#include "Robot1PositioningStateMachine.h" 
#include <util/delay.h>

int main() { 
    // // Initialize connections for motor control
    // Connection pwmOutputA('B', 4, true);
    // Connection pwmOutputB('B', 3, true);
    // Connection pwmDirA('B', 2, true);
    // Connection pwmDirB('B', 5, true);

    // // Create the PWM controller
    // Pwm motorController(pwmDirA, pwmDirB);

    // // Create the robot control instance
    // Robot1 robot(motorController);

    // // Initialize the distance sensor (assuming it's already configured correctly)
    // DistanceSensor* distanceSensor = DistanceSensor::getInstance();

    // // Instantiate the state machine with the robot, sensor, and desired distance
    // Robot1PositioningStateMachine stateMachine(robot, distanceSensor);

    // while (true) {
    //     // Update the state machine, controlling the robot based on sensor input
    //     stateMachine.positioningLogic();

    //     // Add a delay to control the update rate, if necessary
    //     _delay_ms(100); // Adjust this value as needed 
    // }

 Robot1PositioningStateMachine& stateMachine = Robot1PositioningStateMachine::getInstance();

    while (true) {
    
        stateMachine.positioningLogic();
    }
}
