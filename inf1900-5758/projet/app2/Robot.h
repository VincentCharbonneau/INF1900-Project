/*
 * DESCRIPTION
 * Cette classe est la classe maitresse du robot. Elle contient les differents etats du robot
 * et les actions associees a ces etats.
 * 
 *
 * PORTS UTILISES
 * Tout les ports du robot sont employer dans cette classe.
 * 
 * 
 * 
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Vincent Charbonneau, Rebecca Abi Chahine, Charles-Emile Leblond, Camil Bisson
 * 2024
 *
 * Code qui n'est sous aucune license.
 *
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Pwm.h"
#include "Connection.h"
#include "util/delay.h"
#include "Can.h"
#include "IRTransmitter.h"
#include "Led.h"
#include "Sound.h"
#include "LineFollower.h"
#include "LineSensor.h"
#include "USART.h"
#include "avr/interrupt.h"
#include "Timer.h"
#include "Led.h"
#include "EEPROMMatrix.h"
#include "memoire_24.h"
#include "DrawableRectangle.h"
#include "DrawableLine.h"
#include "DrawableTerminatedLine.h"
#include "DrawableMatrix.h"
#include "Line.h"
#include "IRReceiver.h"

class Robot
{
public:
    Robot(volatile uint16_t &quaterSeconds);
    void followLine();
    void init();
    void periodic();
    void moveForward(uint8_t pct);
    void moveBackward(uint8_t pct);
    void turnLeft(uint8_t pct);
    void turnRight(uint8_t pct);
    void turnRight90();
    bool turnRightUntilLine(bool forwardBefore);
    void turnLeft90();
    void blastForward();
    void blastBackward();
    void blastTurnRight();
    void blastTurnLeft();
    void stop();
    void playReadySound();
    void oneWheelTurnLeft(uint8_t pct);
    void finalTurn();
    void display();

    Led* getLed() { return &led; }
    
    LineSensor lineSensor;
    LineFollower lineFollower;
private:
    enum class RobotState
    {
        INIT,
        EXPLORING,
        SCANNING,
        BACKWARDS,
        DONE,
        TRANSMIT,
        DISPLAY
    };
    static const uint8_t N_BEEP = 3;
    static const uint8_t BEEP_DELAY = 50;

    static const uint8_t INITAL_TURN_SPEED = 40;
    static const uint16_t INITAL_TURN_DELAY = 700;

    static const uint8_t TURN_SPEED = 25;

    static const uint8_t QS_TURN = 22;
    
    static const uint8_t BLAST_DELAY = 150;
    static const uint8_t FORWARD_SPEED = 60;
    static const uint16_t FORWARD_DELAY = 1300;
    static const uint8_t BACKWARD_SPEED = 50;
    static const uint8_t STOP_DELAY = 200;

    static const uint16_t FINAL_FORWARD_DELAY = 400;

    Connection motorLeftOut;
    Connection motorLeftDir;
    Connection motorRightOut;
    Connection motorRightDir;
    Connection line0;
    Connection line1;
    Connection line2;
    Connection line3;
    Connection line4;
    Pwm pwm;
    Connection led0;
    Connection led1;
    Led led;
    Connection pGround;
    Connection pSignal;
    Sound piezo;
    Connection buttonD2;
    Connection buttonD3;
    RobotState currentState = RobotState::INIT;
    volatile uint16_t &quarterSeconds;
    uint16_t startExploring = 0;
    uint16_t startScanning = 0;
    uint16_t detectedLine = 0;
    bool firstSide = true;
    bool started = false;
    Line lines[6];
    uint8_t lineCounter = 0;
    void setState(RobotState state);
    void lineFound(uint16_t pos, uint8_t length);
    void transmitLines(void);
};
#endif