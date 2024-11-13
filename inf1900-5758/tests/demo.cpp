#include "LineSensor.h"
#include "Pwm.h"
#include "LineFollower.h"
#include "LineSensor.h"
#include "Connection.h"
#include "Button.h"
#include <util/delay.h>



int main() {
    Connection lineSensor1('A', 0, false); // exemple d'initialisation
    Connection lineSensor2('A', 1, false);
    Connection lineSensor3('A', 2, false);
    Connection lineSensor4('A', 3, false);
    Connection lineSensor5('A', 4, false);

    Connection motorA('B', 4, true);
    Connection motorB('B', 3, true);
    Connection pwmDirA('B',2,true);
    Connection pwmDirB('B',5,true);
    Pwm motorController(pwmDirA, pwmDirB);

    motorController.setDirectionA(false);
    motorController.setDirectionB(false);
    motorController.setPercentA(50);
    motorController.setPercentB(50);

    motorController.setPercentA();
    motorController.setPercentB(100);

}
