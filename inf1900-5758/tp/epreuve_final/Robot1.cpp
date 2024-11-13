#include "Robot1.h"
#include <util/delay.h>


Robot1::Robot1()
    : pwmOutputA('B', 4, true), pwmOutputB('B', 3, true),
      pwmDirA('B', 6, true), pwmDirB('B', 5, true),
      ledConnection1('A', 5, true), ledConnection2('A', 6, true),
      d2('D', 3, false), piezoGround('D', 7, true), piezoSignal('D', 8, true),
      del('A', 2, true), signal('A', 3, false),
      motorController(pwmDirA, pwmDirB),
      led(ledConnection1, ledConnection2),
      button(d2, Button::ButtonInterruptBehavior::FALLING_EDGE, true),
      snd(piezoGround, piezoSignal),
      transmitter(del),
      receiver(signal) {
    // Initialize components in constructor or create an init() method to call here
}

Robot1& Robot1::getInstance() {
    static Robot1 instance;
    return instance;
}

void Robot1::init() {
    // If there's anything specific to initialize post-construction, do it here.
    // E.g., setting initial LED color or ensuring motors are stopped.
    led.setColor(Led::LedColor::COLOR_OFF);
    stop();
}

void Robot1::moveForward(uint8_t speed) {
    motorController.setPercentA(speed);
    motorController.setPercentB(speed);
    motorController.setDirectionA(false);
    motorController.setDirectionB(false);
}

void Robot1::moveBackward(uint8_t speed){
    motorController.setPercentA(speed);
    motorController.setPercentB(speed);
    motorController.setDirectionA(true);
    motorController.setDirectionB(true);
}

void Robot1::turnRight(){
    motorController.setPercentA(60);
    motorController.setPercentB(60);
    motorController.setDirectionA(false);
    motorController.setDirectionB(true);
  //  _delay_ms(550);
    //stop();
}


void Robot1::stop() {
    motorController.setPercentA(0);
    motorController.setPercentB(0);
}