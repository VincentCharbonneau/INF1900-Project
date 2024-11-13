#include "Robot.h"
#include "USART.h"

int main() {
    Robot robot;

    robot.init();

    while(true) {
        robot.periodic();
        // _delay_ms(500);
    }
    return 0;
}