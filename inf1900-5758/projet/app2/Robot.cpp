#include "Robot.h"
const float LINE_DISTANCE_MULTIPLIER = 1;
const uint8_t ADD_TURN_DIST = 5;

Robot::Robot(volatile uint16_t& quaterSeconds) : motorLeftOut('B', 4, true),
                 motorLeftDir('B', 5, true),
                 motorRightOut('B', 3, true),
                 motorRightDir('B', 2, true),
                 line0('C', 2, false),
                 line1('C', 3, false),
                 line2('C', 4, false),
                 line3('C', 5, false),
                 line4('C', 6, false),
                 pwm(motorRightDir, motorLeftDir),
                 lineSensor(line0, line1, line2, line3, line4),
                 lineFollower(lineSensor, pwm), 
                 led0('A', 4, true),
                 led1('A', 5, true),
                 led(led0, led1),
                 pGround('D', 6, true),
                 pSignal('D', 7, true),
                 piezo(pGround, pSignal),
                 buttonD2('D', 2, false),
                 buttonD3('B', 0, false),
                 quarterSeconds(quaterSeconds) {}

void Robot::lineFound(uint16_t pos, uint8_t length) {  
    //USART::getInstance()->print("== LINE FOUND ==\n");
    //USART::getInstance()->print("Pos", pos);
    //USART::getInstance()->print("Length", length);
    if (firstSide) {
        if (lineCounter == 0) {
            Line line(pos, length, firstSide);
            lines[lineCounter++] = line;
        }
        else {
            Line line((lines[lineCounter-1].getDistFromPrev() + pos), length, firstSide);
            lines[lineCounter++] = line;
        }
    }
    else {
        if (lines[lineCounter-1].getFirstSide()) {
            Line line(pos, length, firstSide);
            lines[lineCounter++] = line;
        }
        else {
            Line line((lines[lineCounter-1].getDistFromPrev() + pos), length, firstSide);
            lines[lineCounter++] = line;
        }

    }
}

void Robot::followLine()
{
    lineFollower.followLine();
}
void Robot::setState(RobotState state) {
    currentState = state;
    switch(currentState) {
        case RobotState::EXPLORING:
            USART::getInstance()->print("State: EXPLORING\n");
            startExploring = quarterSeconds;
            blastForward();
            break;
        case RobotState::SCANNING:
            USART::getInstance()->print("State: SCANNING\n");
            startScanning = quarterSeconds;
            blastForward();
            break;
        case RobotState::BACKWARDS:
            USART::getInstance()->print("State: BACKWARDS\n");
            blastBackward();
            break;
        case RobotState::DONE:
            led.setColor(LedColor::COLOR_RED);
            break;
        case RobotState::TRANSMIT:
            cli();
            playReadySound();
            _delay_ms(2000);
            led.setColor(LedColor::COLOR_OFF);
            sei();
            transmitLines();
            Timer::getInstance()->setDurationCompareA(250);
            Timer::getInstance()->setPrescalar1024();
            Timer::getInstance()->clearTimer();
            break;
        case RobotState::DISPLAY:
            Timer::getInstance()->setCountCompareA(0);
            Timer::getInstance()->setDurationCompareB(200);
            display();
            Timer::getInstance()->setCountCompareB(0);
            Timer::getInstance()->setDurationCompareA(250);
        default:
            break;
    }
}
void Robot::periodic()  //MAIN FONCTION
{
    switch (currentState)
    {
    case RobotState::EXPLORING:
        followLine();
        if (lineSensor.getLineDetection() & 0b00001) {
            detectedLine = quarterSeconds;
            bool result = turnRightUntilLine(true);
            if (result) {
                setState(RobotState::SCANNING);
            }
            else {
                lineFound(detectedLine - startExploring, 0);
                turnRightUntilLine(false);
                setState(RobotState::EXPLORING);
            }
        }
        break;
    case RobotState::SCANNING:
        followLine();
        if (lineSensor.getLineDetection() == 0) {
            lineFound(detectedLine - startExploring, quarterSeconds - startScanning);
            stop();
            _delay_ms(100);
            setState(RobotState::BACKWARDS);
        }
        else if (lineSensor.getLineDetection() & 0b00001)
        {   
            turnRightUntilLine(true);
            firstSide = false;
            setState(RobotState::EXPLORING);
        }
        else if (lineSensor.getLineDetection() & 0b10000) {
            finalTurn();
            setState(RobotState::DONE);
        }
        break;
    case RobotState::BACKWARDS:
        moveBackward(BACKWARD_SPEED);
        if (lineSensor.getLineDetection() == 31)
        {
            turnLeft90();
            setState(RobotState::EXPLORING);
        }
        break;
    case RobotState::INIT:
        if (!started) {
            blastForward();
            started = true;
        }
        followLine();
        if (lineSensor.getLineDetection() & 0b00001)
        {   
            turnRightUntilLine(true);
            setState(RobotState::EXPLORING);
        }
        break;
    case RobotState::DONE:
        if (buttonD2.get()) {
            setState(RobotState::TRANSMIT);
        }
        else if (!buttonD3.get()) {
            setState(RobotState::DISPLAY);
        }
        break;
    case RobotState::TRANSMIT:
        setState(RobotState::DONE);
        break;
    case RobotState::DISPLAY:
        setState(RobotState::DONE);
        break;
    }
}
void Robot::moveForward(uint8_t pct)
{
    pwm.setDirectionB(false);
    pwm.setDirectionA(false);
    pwm.setPercentA(pct - 10); // right
    pwm.setPercentB(pct + 4);      // left
}
void Robot::moveBackward(uint8_t pct)
{
    pwm.setDirectionA(true);
    pwm.setDirectionB(true);
    pwm.setPercentA(pct - 5); // right
    pwm.setPercentB(pct);     // left
}
void Robot::turnLeft(uint8_t pct)
{
    pwm.setDirectionA(false);
    pwm.setDirectionB(true);
    pwm.setPercentA(pct);
    pwm.setPercentB(pct+10);
}
void Robot::turnRight(uint8_t pct)
{
    pwm.setDirectionA(true);
    pwm.setDirectionB(false);
    pwm.setPercentA(pct);
    pwm.setPercentB(pct+5);
}
void Robot::turnRight90()
{
    stop();
    _delay_ms(STOP_DELAY);
    moveForward(60);
    _delay_ms(FORWARD_DELAY);
    turnRight(INITAL_TURN_SPEED);
    _delay_ms(INITAL_TURN_DELAY);
    while(!(lineSensor.getLineDetection() & 0b00000100)) {
        turnRight(TURN_SPEED);
    }
    stop();
    _delay_ms(STOP_DELAY);
}
bool Robot::turnRightUntilLine(bool forwardBefore) {
    uint16_t initialTime = quarterSeconds;
    stop();
    _delay_ms(STOP_DELAY);
    if (forwardBefore) {
        moveForward(FORWARD_SPEED);
        _delay_ms(FORWARD_DELAY);
    }
    blastTurnRight();
    turnRight(INITAL_TURN_SPEED);
    _delay_ms(INITAL_TURN_DELAY);
    while(!(lineSensor.getLineDetection() & 0b00000100)) {
        turnRight(TURN_SPEED+10);
    }
    stop();
    _delay_ms(STOP_DELAY);
    return quarterSeconds - initialTime < QS_TURN;
}
void Robot::turnLeft90()
{
    stop();
    _delay_ms(STOP_DELAY);
    moveForward(FORWARD_SPEED);
    _delay_ms(FORWARD_DELAY);
    blastTurnLeft();
    turnLeft(INITAL_TURN_SPEED-10);
    _delay_ms(1000);
    while(!(lineSensor.getLineDetection() & 0b00000100)) {
        turnLeft(TURN_SPEED);
    }
    stop();
    _delay_ms(STOP_DELAY);
}
void Robot::stop()
{
    pwm.setPercentA(0);
    pwm.setPercentB(0);
}
void Robot::blastForward() {
    pwm.setDirectionA(0);
    pwm.setDirectionB(0);
    pwm.setPercentA(85);
    pwm.setPercentB(100);
    _delay_ms(BLAST_DELAY);
}
void Robot::blastBackward() {
    pwm.setDirectionA(1);
    pwm.setDirectionB(1);
    pwm.setPercentA(85);
    pwm.setPercentB(100);
    _delay_ms(BLAST_DELAY);
}
void Robot::blastTurnLeft() {
    pwm.setDirectionA(0);
    pwm.setDirectionB(1);
    pwm.setPercentA(100);
    pwm.setPercentB(100);
    _delay_ms(BLAST_DELAY);
}
void Robot::blastTurnRight() {
    pwm.setDirectionA(1);
    pwm.setDirectionB(0);
    pwm.setPercentA(100);
    pwm.setPercentB(100);
    _delay_ms(BLAST_DELAY);
}
void Robot::playReadySound() {
    for (uint8_t i = 0; i < N_BEEP; ++i) {
        piezo.playNote(81);
        _delay_ms(BEEP_DELAY);
        piezo.stop();
        _delay_ms(BEEP_DELAY);
    }
}
void Robot::oneWheelTurnLeft(uint8_t pct) {
    pwm.setDirectionA(false);
    pwm.setDirectionB(true);
    pwm.setPercentA(pct);
}
void Robot::finalTurn() {
    stop();
    _delay_ms(200);
    moveForward(FORWARD_SPEED);
    _delay_ms(FORWARD_DELAY+100);
    oneWheelTurnLeft(INITAL_TURN_SPEED-20);
    _delay_ms(500);
    while(!(lineSensor.getLineDetection() & 0b00000100)) {
        oneWheelTurnLeft(TURN_SPEED);
    }
    stop();
    _delay_ms(200);
    while ((lineSensor.getLineDetection() != 0)) {
        followLine();
    }
    stop();
    _delay_ms(200);
    blastForward();
    moveForward(FORWARD_SPEED);
    _delay_ms(FINAL_FORWARD_DELAY);
    stop();
}
void Robot::transmitLines() {
    for(uint8_t i = 0; i < lineCounter; i++) {
        IRTransmitter::getInstance()->sendLine(lines[i], i == lineCounter - 1);
        _delay_ms(1000);
    }
}
void Robot::display() {
    Memoire24CXXX eeprom;
    EEPROMMatrix matrix(25, 101, ' ', 0, eeprom);
    DrawableRectangle outline(25, 101, DrawableLine::LineType::THIN);
	DrawableRectangle box(17, 26, DrawableLine::LineType::BOLD);
	DrawableLine boxEntry(DrawableLine::Direction::VERTICAL, 3, DrawableLine::LineType::EMPTY);
	matrix.draw(outline, 0, 0);
	matrix.draw(box, 4, 7);
	matrix.draw(boxEntry, 11, 32);
    //USART::getInstance()->print("b\n");
	DrawableRectangle route(13, 50, DrawableLine::LineType::DOUBLE);
	matrix.draw(route, 6, 42);
	DrawableLine routeEnd(DrawableLine::Direction::HORIZONTAL, 6, DrawableLine::LineType::DOUBLE);
	DrawableTerminatedLine routeEndTerminated(DrawableTerminatedLine::TERMINATION::END, routeEnd);
    //USART::getInstance()->print("c\n");
	matrix.draw(routeEndTerminated, 12, 36);

	uint8_t distFromPrevAcc {};
	uint8_t lineIndex {};
	for(; lineIndex < 6 && lines[lineIndex].getFirstSide(); lineIndex++) {
		Line& line = lines[lineIndex];
        //USART::getInstance()->print("Line length", line.getRawLength());
		distFromPrevAcc = line.getDistFromPrev() * LINE_DISTANCE_MULTIPLIER + ADD_TURN_DIST;
		DrawableLine dl(line);
		DrawableTerminatedLine dtl(DrawableTerminatedLine::TERMINATION::START, dl);
		matrix.draw(dtl, 6, 42 + distFromPrevAcc);
	}
    //USART::getInstance()->print("d\n");
	distFromPrevAcc = 0;
	for(; lineIndex < 6 && lines[lineIndex].getRawLineType() != LineLength::UNDEFINED; lineIndex++) {
		Line& line = lines[lineIndex];
        //USART::getInstance()->print("Line length", line.getRawLength());
		distFromPrevAcc = line.getDistFromPrev() * LINE_DISTANCE_MULTIPLIER + ADD_TURN_DIST;
		DrawableLine dl(line);
		DrawableTerminatedLine dtl(DrawableTerminatedLine::TERMINATION::END, dl);
		matrix.draw(dtl, 18 - dl.getSize(), 92 - distFromPrevAcc);
	}
    //USART::getInstance()->print("e\n");
    matrix.display();
}