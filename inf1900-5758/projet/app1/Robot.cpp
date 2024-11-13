#include "Robot.h"

Robot *robotPtr = nullptr;

const uint8_t ADD_TURN_DIST = 5;
const float LINE_DISTANCE_MULTIPLIER = 1.2;

const uint16_t WAIT_TURN_DELAY = 500;
const uint16_t TURN_DELAY = 425;
const uint16_t BLAST = 150;
const uint16_t MINI_TURN_DELAY = 120;
const uint16_t LAST_MINI_TURN_DELAY = 160;
const uint16_t FIRST_READING_DELAY = 2000;
const uint8_t WAITING_SOUND_DELAY = 50;
const uint16_t ERROR_SOUND_DELAY = 1000;
const uint8_t WAITING_NOTE = 81;
const uint8_t ERROR_NOTE = 45;
const uint16_t READING_TO_FIND_VOID_DELAY = 300;

Robot::Robot() : motorLeftOut('B', 4, true),
                 motorLeftDir('B', 5, true),
                 motorRightOut('B', 3, true),
                 motorRightDir('B', 2, true),
                 distSensor('A', 0, false),
                 ledConnectionMinus('A', 4, true),
                 ledConnectionPlus('A', 5, true),
                 piezoGround('D', 6, true),
                 piezoSignal('D', 7, true),
                 signal('A', 2, false),
                 ledReference(ledConnectionMinus, ledConnectionPlus),
                 button('D', 2, false),
                 sound(piezoGround, piezoSignal),
                 receiver(signal, *this),
                 pwm(motorRightDir, motorLeftDir)
{
    robotPtr = this;
};

ISR(TIMER1_COMPA_vect)
{
    if (robotPtr->shouldBlink())
    {
        if (robotPtr->getLed()->getColor() == LedColor::COLOR_OFF)
            robotPtr->getLed()->setColor(LedColor::COLOR_GREEN);
        else
            robotPtr->getLed()->setColor(LedColor::COLOR_OFF);
    }

    robotPtr->incrementQuarterSeconds();
}

void Robot::init()
{

    //USART::getInstance()->print("Robot Init\n");
    sei();
    Timer::getInstance()->setDurationCompareA(250);
    Timer::getInstance()->clearTimer();
    quarterSeconds = 0;
}

void Robot::periodic()
{
    switch (currentState)
    {
    case RobotState::FIND_POSITION:
    {
        getInitialReading();
        blastMotors();
        currentState = RobotState::FORWARD;
        break;
    }
    case RobotState::FORWARD:
    {
        if (moveToGoal())
        {
            stop();
            _delay_ms(WAIT_TURN_DELAY);
            currentState = RobotState::RTURN;
        }
        break;
    }
    case RobotState::RTURN:
    {
        //USART::getInstance()->print("T\n");
        leftTurn();
        _delay_ms(WAIT_TURN_DELAY);
        bool stopTurning = turnUntilVoid(ITERATIONS_NUMBER);
        if (stopTurning)
        {
            initialSide = RobotInitialSide::FACING_DOWN;
            currentState = RobotState::WAIT;
        }
        else
        {
            initialSide = RobotInitialSide::FACING_UP;
            currentState = RobotState::SECOND_TURN;
        }
        break;
    }
    case RobotState::SECOND_TURN:
    {
        //::getInstance()->print("ST\n");
        leftTurn();
        turnUntilVoid(0);
        currentState = RobotState::WAIT;
        break;
    }
    case RobotState::WAIT:
    {
        getInitialPosition();
        ledReference.setColor(LedColor::COLOR_RED);
        if (button.get())
            startReception();
        break;
    }

    case RobotState::RECEIVE:
    {
        ledReference.setColor(LedColor::COLOR_OFF);
        receiver.readMessage();
        if (firstEverTrameSeen != 0 &&
            quarterSeconds > firstEverTrameSeen + 4 * 30 &&
            lineCounter >= 1)
        {
            currentState = RobotState::TRANSMIT;
        }
        else if (firstEverTrameSeen != 0 &&
                 quarterSeconds > firstEverTrameSeen + 4 * 30 &&
                 lineCounter == 0)
        {
            playErrorSound();
            currentState = RobotState::WAIT;
        }
        else if(firstEverTrameSeen == 0 && receiveBegan != 0 &&
                quarterSeconds > receiveBegan + 4 * 45) {
            playErrorSound();
            currentState = RobotState::WAIT;
        }
        break;
    }
    case RobotState::TRANSMIT:
    {
        ledReference.setColor(LedColor::COLOR_GREEN);
        sei();
        if (button.get())
        {
            //USART::getInstance()->print("Envoie des données au PC\n");
            blinkLed = true;
            Timer::getInstance()->setDurationCompareA(200);
            display();
        }
        break;
    }
    }
}

void Robot::startReception()
{
    playWaitingSound();
    receiveBegan = quarterSeconds;
    currentState = RobotState::RECEIVE;
}

void Robot::blastForward()
{
    pwm.setDirectionB(false);
    pwm.setDirectionA(false);
    pwm.setPercentA(BLAST_POWER);
    pwm.setPercentB(BLAST_POWER);
    _delay_ms(BLAST);
}

void Robot::blastBackward()
{
    pwm.setDirectionB(true);
    pwm.setDirectionA(true);
    pwm.setPercentA(BLAST_POWER);
    pwm.setPercentB(BLAST_POWER);
    _delay_ms(BLAST);
}

void Robot::blastMotors()
{
    uint16_t distanceReading = getFilteredDistance();
    if (distanceReading > DISTANCE_GOAL)
        blastBackward();
    else
        blastForward();
}

void Robot::moveForward(uint8_t pct)
{
    pwm.setDirectionB(false);
    pwm.setDirectionA(false);
    pwm.setPercentA(pct - FORWARD_ADJUSTMENT_RIGHT);
    pwm.setPercentB(pct - FORWARD_ADJUSTMENT_LEFT);
}

void Robot::moveBackward(uint8_t pct)
{
    pwm.setDirectionA(true);
    pwm.setDirectionB(true);
    pwm.setPercentA(pct - BACKWARD_ADJUSTMENT_RIGHT);
    pwm.setPercentB(pct - BACKWARD_ADJUSTMENT_LEFT);
}

void Robot::turnLeft(uint8_t pct)
{
    pwm.setDirectionA(false);
    pwm.setDirectionB(true);
    pwm.setPercentA(pct);
    pwm.setPercentB(pct);
}

void Robot::leftTurn()
{
    turnLeft(TURN_POWER);
    _delay_ms(TURN_DELAY);
    stop();
    _delay_ms(WAIT_TURN_DELAY);
}

void Robot::stop()
{
    pwm.setPercentA(0);
    pwm.setPercentB(0);
}

uint16_t Robot::getRawDistanceValue()
{
    return Can::getInstance()->lecture(0);
}

uint16_t Robot::getFilteredDistance()
{
    uint16_t distances[]{0, 0, 0, 0, 0};
    uint8_t size = 5;
    for (uint8_t i = 0; i < size; i++)
    {
        distances[i] = getRawDistanceValue();
    }
    sort(distances, size);
    return distances[size / 2];
}

bool Robot::turnUntilVoid(uint8_t iterations)
{
    uint16_t count = 0;
    uint16_t distanceReading = getFilteredDistance();
    bool turn = distanceReading > DISTANCE_VOID;
    while (turn)
    {
        _delay_ms(READING_TO_FIND_VOID_DELAY);
        distanceReading = getFilteredDistance();
        if (distanceReading <= DISTANCE_VOID)
        {
            turnLeft(TURN_POWER);
            _delay_ms(LAST_MINI_TURN_DELAY);
            stop();
            return true;
        }
        else if (count > iterations && iterations != 0)
        {
            stop();
            return false;
        }
        else
        {
            turnLeft(TURN_POWER);
            _delay_ms(MINI_TURN_DELAY);
            stop();
            count++;
        }
    }
    return 0;
}

bool Robot::epsilonEquals(uint8_t a, uint8_t b, uint8_t epsilon)
{
    int16_t diff = a - b;
    return (diff > 0 && diff <= epsilon) ||
           (diff < 0 && diff >= -epsilon);
}

bool Robot::moveToGoal()
{
    uint16_t distanceReading = getFilteredDistance();

    if (distanceReading > DISTANCE_GOAL)
    {
        moveBackward(TO_GOAL_POWER);
    }
    else
    {
        moveForward(TO_GOAL_POWER);
    }

    bool atGoal = epsilonEquals(distanceReading, DISTANCE_GOAL, 4);

    if (atGoal)
    {
        atGoalCounter++;
        if (atGoalCounter >= 2)
        {
            atGoalCounter = 5;
            //USART::getInstance()->print("at goal!\n");
            return true;
        }
    }
    else
    {
        atGoalCounter = 0;
    }

    return false;
}

void Robot::getInitialReading()
{
    _delay_ms(FIRST_READING_DELAY);
    initialReading = getFilteredDistance();
}

void Robot::getInitialPosition()
{
    if (initialReading > 320)
        initialPosition = 7;
    else if (initialReading > 260)
        initialPosition = 6;
    else if (initialReading > 165)
        initialPosition = 5;
    else if (initialReading > 115)
        initialPosition = 4;
    else if (initialReading > 72)
        initialPosition = 3;
    else if (initialReading > 66)
        initialPosition = 2;
    else
        initialPosition = 1;
    if (initialSide == RobotInitialSide::FACING_UP)
        switch (initialPosition)
        {
        case 1:
            initialPosition = 7;
            break;
        case 2:
            initialPosition = 6;
            break;
        case 3:
            initialPosition = 5;
            break;
        case 5:
            initialPosition = 3;
            break;
        case 6:
            initialPosition = 2;
            break;
        case 7:
            initialPosition = 1;
            break;
        }
}

Robot::RobotInitialSide Robot::getInitialSide()
{
    return initialSide;
}

void Robot::playWaitingSound()
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        sound.playNote(WAITING_NOTE); // Joue un son aigu
        _delay_ms(WAITING_SOUND_DELAY);
        sound.stop();
        _delay_ms(WAITING_SOUND_DELAY);
    }
}

void Robot::playErrorSound()
{
    sound.playNote(ERROR_NOTE); // La note la plus grave
    _delay_ms(ERROR_SOUND_DELAY);
    sound.stop();
}

void Robot::sort(uint16_t arr[], uint8_t size)
{
    for (uint8_t i = 0; i < size - 1; i++)
    {
        for (uint8_t j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // Swap arr[j] and arr[j+1]
                uint16_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void Robot::consumeTrame(uint8_t trame)
{
    if (firstEverTrameSeen == 0)
    {
        firstEverTrameSeen = quarterSeconds;
    }

    if (quarterSeconds - lastTrameSeen > 2)
    {
        trameCount = 0;
    }
    lastTrameSeen = quarterSeconds;

    trames[trameCount++] = trame;
    if (trameCount == 5)
    {
        trameCount = 0;

        uint8_t metadata = trames[0];
        bool firstSide = metadata & 0b01000000;
        bool lastLine = metadata & 0b00000001;

        uint8_t taille = metadata & 0b00110000;

        LineLength length;
        switch (taille)
        {
        case 0b00010000:
            length = LineLength::SMALL;
            break;
        case 0b00100000:
            length = LineLength::MEDIUM;
            break;
        case 0b00110000:
            length = LineLength::LONG;
            break;
        default:
            length = LineLength::UNDEFINED;
            break;
        }

        uint8_t pos = trames[1];

        uint16_t crc = 0xFFFF;
        crc = _crc16_update(crc, metadata);
        crc = _crc16_update(crc, pos);

        uint16_t receivedCRC = 0;
        uint16_t crc0 = trames[2];
        uint16_t crc1 = trames[3];
        uint16_t crc2 = trames[4];

        receivedCRC |= crc0;
        receivedCRC |= (crc1 << 7);
        receivedCRC |= (crc2 << 14);

        if (crc == receivedCRC)
        {
            Line line(pos, 0, firstSide);
            line.setLineType(length);

            lines[lineCounter++] = line;

            //USART::getInstance()->print("Good line #", lineCounter);

            if (lastLine)
            {
                currentState = RobotState::TRANSMIT;
            }
        }
        else
        {
            //USART::getInstance()->print("Bad line\n");
        }
    }
}

void Robot::display()
{
    Memoire24CXXX eeprom;

    EEPROMMatrix matrix(25, 101, ' ', 0, eeprom);

    DrawableRectangle outline(25, 101, DrawableLine::LineType::THIN);
    DrawableRectangle box(17, 26, DrawableLine::LineType::BOLD);
    DrawableLine boxEntry(DrawableLine::Direction::VERTICAL, 3, DrawableLine::LineType::EMPTY);

    matrix.draw(outline, 0, 0);
    matrix.draw(box, 4, 7);
    matrix.draw(boxEntry, 11, 32);

    DrawableRectangle route(13, 50, DrawableLine::LineType::DOUBLE);
    matrix.draw(route, 6, 42);

    DrawableLine routeEnd(DrawableLine::Direction::HORIZONTAL, 6, DrawableLine::LineType::DOUBLE);
    DrawableTerminatedLine routeEndTerminated(DrawableTerminatedLine::TERMINATION::END, routeEnd);

    matrix.draw(routeEndTerminated, 12, 36);

    uint8_t distFromPrevAcc{};
    uint8_t lineIndex{};

    for (; lineIndex < 6 && lines[lineIndex].getFirstSide(); lineIndex++)
    {
        Line &line = lines[lineIndex];
        //USART::getInstance()->print("Line length", line.getRawLength());
        distFromPrevAcc = line.getDistFromPrev() * LINE_DISTANCE_MULTIPLIER + ADD_TURN_DIST;
        DrawableLine dl(line);
        DrawableTerminatedLine dtl(DrawableTerminatedLine::TERMINATION::START, dl);
        matrix.draw(dtl, 6, 42 + distFromPrevAcc);
    }

    distFromPrevAcc = 0;

    for (; lineIndex < 6 && lines[lineIndex].getRawLineType() != LineLength::UNDEFINED; lineIndex++)
    {
        Line &line = lines[lineIndex];
        // USART::getInstance()->print("Line length", line.getRawLength());
        distFromPrevAcc = line.getDistFromPrev() * LINE_DISTANCE_MULTIPLIER + ADD_TURN_DIST;
        DrawableLine dl(line);
        DrawableTerminatedLine dtl(DrawableTerminatedLine::TERMINATION::END, dl);
        matrix.draw(dtl, 18 - dl.getSize(), 92 - distFromPrevAcc);
    }

    EEPROMMatrix robotMatrix(4, 5, ' ', 10000, eeprom);

    if (initialSide == RobotInitialSide::FACING_UP)
    {
        robotMatrix.set(0, 0, 0x2571);
        robotMatrix.set(0, 1, 0x2594);
        robotMatrix.set(0, 2, 0x2594);
        robotMatrix.set(0, 3, 0x2594);
        robotMatrix.set(0, 4, 0x2572);

        robotMatrix.set(1, 0, 0x258F);
        robotMatrix.set(1, 1, 0x2800);
        robotMatrix.set(1, 2, 0x2800);
        robotMatrix.set(1, 3, 0x2800);
        robotMatrix.set(1, 4, 0x2595);

        robotMatrix.set(2, 0, 0x2593);
        robotMatrix.set(2, 1, 0x2800);
        robotMatrix.set(2, 2, 0x2800);
        robotMatrix.set(2, 3, 0x2800);
        robotMatrix.set(2, 4, 0x2593);

        robotMatrix.set(3, 0, 0x2572);
        robotMatrix.set(3, 1, 0x2581);
        robotMatrix.set(3, 2, 0x2581);
        robotMatrix.set(3, 3, 0x2581);
        robotMatrix.set(3, 4, 0x2571);
    }
    else
    {
        robotMatrix.set(0, 0, 0x2571);
        robotMatrix.set(0, 1, 0x2594);
        robotMatrix.set(0, 2, 0x2594);
        robotMatrix.set(0, 3, 0x2594);
        robotMatrix.set(0, 4, 0x2572);

        robotMatrix.set(1, 0, 0x2593);
        robotMatrix.set(1, 1, 0x2800);
        robotMatrix.set(1, 2, 0x2800);
        robotMatrix.set(1, 3, 0x2800);
        robotMatrix.set(1, 4, 0x2593);

        robotMatrix.set(2, 0, 0x258F);
        robotMatrix.set(2, 1, 0x2800);
        robotMatrix.set(2, 2, 0x2800);
        robotMatrix.set(2, 3, 0x2800);
        robotMatrix.set(2, 4, 0x2595);

        robotMatrix.set(3, 0, 0x2572);
        robotMatrix.set(3, 1, 0x2581);
        robotMatrix.set(3, 2, 0x2581);
        robotMatrix.set(3, 3, 0x2581);
        robotMatrix.set(3, 4, 0x2571);
    }
    uint8_t position = 7;
    switch (initialPosition)
    {
    case 1:
        position = 7;
        break;
    case 2:
        position = 8;
        break;
    case 3:
        position = 9;
        break;
    case 4:
        position = 11;
        break;
    case 5:
        position = 13;
        break;
    case 6:
        position = 14;
        break;
    case 7:
        position = 15;
        break;
    }

    DrawableMatrix robot(robotMatrix);
    matrix.draw(robot, position, 17);

    matrix.display();
    USART::getInstance()->print("Produit par: Mekhani\n");
}
