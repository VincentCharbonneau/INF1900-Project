#ifndef ROBOT_H
#define ROBOT_H

#include "Pwm.h"
#include "Connection.h"
#include "util/delay.h"
#include "Can.h"
#include "IRReceiver.h"
#include "Button.h"
#include "Led.h"
#include "Sound.h"
#include "Timer.h"

#include "EEPROMMatrix.h"
#include "memoire_24.h"
#include "DrawableRectangle.h"
#include "DrawableLine.h"
#include "DrawableTerminatedLine.h"
#include "DrawableMatrix.h"
#include "TrameConsumer.h"
#include <util/crc16.h>
#include <avr/interrupt.h>

class Robot : public TrameConsumer
{

public:
    enum class RobotState
    {
        FIND_POSITION,
        FORWARD,
        RTURN,
        SECOND_TURN,
        WAIT,
        RECEIVE,
        TRANSMIT
    };

    enum class RobotInitialSide
    {
        FACING_UP,
        FACING_DOWN
    };
    Robot();
    
    void init();
    void periodic();
    uint16_t getRawDistanceValue();

    virtual void consumeTrame(uint8_t);

    Led* getLed() { return &ledReference; }
    bool shouldBlink() { return blinkLed; }
    void incrementQuarterSeconds() { quarterSeconds++; }

private:
    const uint16_t DISTANCE_GOAL = 146;
    const uint8_t AT_GOAL_VALIDATION = 5;
    const uint16_t DISTANCE_VOID = 98;
    const uint8_t ITERATIONS_NUMBER = 20;
    const uint8_t BLAST_POWER = 100;
    const uint8_t FORWARD_ADJUSTMENT_LEFT = 5;
    const uint8_t FORWARD_ADJUSTMENT_RIGHT = 0;
    const uint8_t BACKWARD_ADJUSTMENT_LEFT = 1;
    const uint8_t BACKWARD_ADJUSTMENT_RIGHT = 0;
    const uint8_t TURN_POWER = 70;
    const uint8_t TO_GOAL_POWER = 50;

    Connection motorLeftOut;
    Connection motorLeftDir;
    Connection motorRightOut;
    Connection motorRightDir;

    Connection distSensor;

    Connection ledConnectionMinus;
    Connection ledConnectionPlus;

    Connection piezoGround, piezoSignal;
    Connection signal;

    Led ledReference;
    Connection button;

    Sound sound;
    IRReceiver receiver;

    Pwm pwm;

    volatile uint16_t quarterSeconds;

    RobotState currentState = RobotState::FIND_POSITION;
    RobotInitialSide initialSide;

    uint8_t atGoalCounter = 0;
    uint16_t irData;
    uint16_t initialReading;
    uint8_t initialPosition;

    Line lines[6];
    uint8_t lineCounter = 0;

    uint8_t trames[5];
    uint8_t trameCount = 0;
    uint16_t lastTrameSeen = 0;

    bool blinkLed = false;

    uint16_t firstEverTrameSeen = 0;
    uint16_t receiveBegan = 0;

    void startReception();

    void blastForward();
    void blastBackward();
    void blastMotors();
    void moveForward(uint8_t pct);
    void moveBackward(uint8_t pct);
    void turnLeft(uint8_t pct);
    void leftTurn();
    void stop();
    bool moveToGoal();

    bool epsilonEquals(uint8_t a, uint8_t b, uint8_t epsilon);
    bool turnUntilVoid(uint8_t iterations);

    uint16_t getFilteredDistance();
    void getInitialReading();
    void getInitialPosition();
    RobotInitialSide getInitialSide();

    void playWaitingSound();
    void playErrorSound();

    void sort(uint16_t arr[], uint8_t size);

    void display();
};

#endif
