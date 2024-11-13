// LineFollower.cpp
#include "LineFollower.h"
#include "USART.h"

LineFollower::LineFollower(LineSensor& line, Pwm& motorCtrl) : lineSensors(line), motorController(motorCtrl) {}

const uint8_t DELAY = 1;

void LineFollower::mircoAdjustRight() {
    motorController.setPercentA(MOTOR_SPEED_NORMAL_A);
    motorController.setPercentB(MOTOR_SPEED_TURN_B);
}

void LineFollower::mircoAdjustRight(uint8_t lineStatus) { //WITH A check for the lineStatus
    if ((lineStatus & LINE_DETECTED_RIGHT)) {
        motorController.setPercentA(MOTOR_SPEED_NORMAL_A);
        motorController.setPercentB(MOTOR_SPEED_TURN_B);
        _delay_ms(DELAY);
    }
}

void LineFollower::mircoAdjustLeft() {
    motorController.setPercentA(MOTOR_SPEED_TURN_A);
    motorController.setPercentB(MOTOR_SPEED_NORMAL_B);
}

void LineFollower::mircoAdjustLeft(uint8_t lineStatus) { //WITH A check for the lineStatus
    if (lineStatus & LINE_DETECTED_LEFT) {
        motorController.setPercentA(MOTOR_SPEED_TURN_A);
        motorController.setPercentB(MOTOR_SPEED_NORMAL_B);
        _delay_ms(DELAY);
    }
}

void LineFollower::followLine() {
    uint8_t lineStatus = lineSensors.getLineDetection();

    if ((lineStatus & LINE_DETECTED_HARD_RIGHT) ) {
        motorController.setDirectionA(1);
        motorController.setDirectionB(0);
        motorController.setPercentA(MOTOR_SPEED_TURN_A);
        motorController.setPercentB(MOTOR_SPEED_TURN_B);
    }
    else if ((lineStatus & LINE_DETECTED_HARD_LEFT)) {
        motorController.setDirectionA(0);
        motorController.setDirectionB(1);
        motorController.setPercentA(MOTOR_SPEED_TURN_A);
        motorController.setPercentB(MOTOR_SPEED_TURN_B);
    }

    else if ((lineStatus & LINE_DETECTED_RIGHT)) {
        motorController.setDirectionA(0);
        motorController.setDirectionB(0);
        mircoAdjustRight();
    }
    else if ((lineStatus & LINE_DETECTED_LEFT)) {
        motorController.setDirectionA(0);
        motorController.setDirectionB(0);
        mircoAdjustLeft();
    }

    else {
        motorController.setPercentA(MOTOR_SPEED_NORMAL_A);
        motorController.setPercentB(MOTOR_SPEED_NORMAL_B);
    }
}

void LineFollower::followLineBackwards() {
    uint8_t lineStatus = lineSensors.getLineDetection();

    if ((lineStatus & LINE_DETECTED_RIGHT)) {
        motorController.setDirectionA(1);
        motorController.setDirectionB(1);
        //USART::getInstance()->print("3\n");
        mircoAdjustLeft();
    }
    else if ((lineStatus & LINE_DETECTED_LEFT)) {
        motorController.setDirectionA(1);
        motorController.setDirectionB(1);
        //USART::getInstance()->print("4\n");
        mircoAdjustRight();
    }
    

    else {
        motorController.setDirectionA(1);
        motorController.setDirectionB(1);
        motorController.setPercentA(MOTOR_SPEED_NORMAL_A);
        motorController.setPercentB(MOTOR_SPEED_NORMAL_B);
    }
}
