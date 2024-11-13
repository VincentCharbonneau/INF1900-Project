#ifndef ROBOT1_POSITIONING_STATE_MACHINE_H
#define ROBOT1_POSITIONING_STATE_MACHINE_H

#include "Robot1.h"
#include "DistanceSensor.h"

enum class RobotPositioningState {
    INITIALIZATION,
    MOVING,
    ORIENTING,
    MISSION_COMPLETE
};

enum class InitialRobotPositionSide {
    VERS_LE_HAUT,
    VERS_LE_BAS
};

class Robot1PositioningStateMachine {
private:
    // v=p*e
    // e=a-d
    uint8_t error = 0; // e
    uint16_t const PROPORTION_COEFFICIENT = 5; // p
    uint8_t speed = 0; // v
    uint16_t currentDistance = 0; // a
    uint8_t const GOALVALUE = 33; // d  The target distance value for the goal position (6x3=18 inch → 45.72 cm)
    // const uint8_t minOpenDistance = 38; // Minimum distance indicating not facing a side wall (5x3 = 15 inch → 38.1 cm)
    uint8_t initialSide = 0; // pair=vers le haut, impair=vers le bas

    Robot1PositioningStateMachine(); // Private constructor
    Robot1PositioningStateMachine(const Robot1PositioningStateMachine&) = delete; // No copy-construction
    Robot1PositioningStateMachine& operator=(const Robot1PositioningStateMachine&) = delete; // No assignment

    Robot1& robot;
    DistanceSensor& distanceSensor;
    RobotPositioningState currentState = RobotPositioningState::INITIALIZATION;
    uint8_t initialPosition = 0;

    bool orientingToGoal();
    bool moveToGoal();

public:
   static Robot1PositioningStateMachine& getInstance();
    void positioningLogic();

    InitialRobotPositionSide getInitialRobotPositionSide();
    uint8_t getInitialRobotPosition();
    RobotPositioningState getCurrentState();
};

#endif // ROBOT1_POSITIONING_STATE_MACHINE_H
