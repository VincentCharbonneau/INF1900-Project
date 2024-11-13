
#ifndef ROBOT1_H
#define ROBOT1_H
#include "Pwm.h"
#include "Led.h"
#include "Button.h"
#include "Sound.h"
#include "IR_transmission.h"
#include "IR_receive.h"
#include "Connection.h"

class Robot1 {
private:
    Pwm motorController;
    Led led;
    Button button;
    Sound snd;
    IR_transmission transmitter;
    IR_receive receiver;

    // Singleton's private constructor, copy constructor, and assignment operator
    Robot1();
    Robot1(const Robot1&) = delete;
    Robot1& operator=(const Robot1&) = delete;

    // Member variables for device connections
    Connection pwmOutputA, pwmOutputB, pwmDirA, pwmDirB;
    Connection ledConnection1, ledConnection2;
    Connection d2;
    Connection piezoGround, piezoSignal;
    Connection del, signal;

public:
    static Robot1& getInstance();
    void init();
    void moveForward(uint8_t speed);
    void moveBackward(uint8_t speed);
    void turnRight();
    void stop();
};

#endif