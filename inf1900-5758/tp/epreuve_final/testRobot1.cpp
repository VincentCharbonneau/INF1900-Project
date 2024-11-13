#include "Pwm.h"
#include "Connection.h"
#include <util/delay.h>
#include "Robot1.h"


int main(){
    Connection pwmOutputA('B', 4, true);
    Connection pwmOutputB('B', 3, true);
    Connection pwmDirA('B', 2, true);
    Connection pwmDirB('B', 5, true);
    Pwm motorController(pwmDirA, pwmDirB);
    Robot1 robot1(motorController);

    robot1.moveForward();
    _delay_ms(750);
    robot1.stop();
    robot1.moveBackward();
    _delay_ms(750);
    robot1.stop();
    robot1.turnLeft();
    robot1.turnRight();
    robot1.turnAround();
}
