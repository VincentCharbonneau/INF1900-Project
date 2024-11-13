#include "Robot1PositioningStateMachine.h"
#include <stdlib.h>

Robot1PositioningStateMachine::Robot1PositioningStateMachine(){
        initialPosition = distanceSensor->getDistance(); 
}

void Robot1PositioningStateMachine::positioningLogic() {
    switch (currentState) {
        case RobotPositioningState::INITIALIZATION:
            currentState = RobotPositioningState::MOVING;
            break;
        case RobotPositioningState::MOVING:
            if (moveToGoal()) {
                currentState = RobotPositioningState::ORIENTING;
            }
            break;
        case RobotPositioningState::ORIENTING:
            if(orientingToGoal()) {
                currentState = RobotPositioningState::MISSION_COMPLETE;
            }
            break;
        case RobotPositioningState::MISSION_COMPLETE:
            robot.stop();
            break;
    }
}

bool Robot1PositioningStateMachine::moveToGoal() {
    uint8_t currentDistance = distanceSensor->getDistance();    //**********Dois-je gerer une distance < 6 pouces? ou levaluateur n'a pas le droit de le mettre? genre si <6 pouces, j'inverse les moteurs pour reculer
    int error = GOALVALUE - currentDistance;
    if (error != 0) {
        if (error > 0) {
            speed = PROPORTION_COEFFICIENT * error;
            robot.moveForward(speed);
        } else {
            speed = PROPORTION_COEFFICIENT * abs(error);
            robot.moveBackward(speed);
        }
        return false; // Not at goal yet
    }
    return true; // Goal reached
}

bool Robot1PositioningStateMachine::orientingToGoal() {
    robot.turnRight();
    _delay_ms(50);
    robot.stop();
    uint8_t minOpenDistance = GOALVALUE;
    uint8_t distance = distanceSensor->getDistance();

    if (distance > minOpenDistance) {
        return true;
    }

    // Start turning until an opening is detected.
    while (distance <= minOpenDistance) {
        initialSide++;
        robot.turnRight(); // Incrementally turn right until an opening is detected.
        _delay_ms(50); // Short delay to allow for a continuous turn check and to give the sensor time to take a reading.
        robot.stop();
        distance = distanceSensor->getDistance();
    }

    robot.stop(); // Stop once the distance indicates the robot is facing the opening.
    return true; // Orientation towards the goal is complete.
} 

uint8_t Robot1PositioningStateMachine::getInitialRobotPosition() {
    return initialPosition;
}   

InitialRobotPositionSide Robot1PositioningStateMachine::getInitialRobotPositionSide() {
    if(initialSide % 2 == 0) {
        return InitialRobotPositionSide::VERS_LE_HAUT;
    } else {
        return InitialRobotPositionSide::VERS_LE_BAS;
    }
}
 RobotPositioningState Robot1PositioningStateMachine::getCurrentState() {
     return currentState;
 }
