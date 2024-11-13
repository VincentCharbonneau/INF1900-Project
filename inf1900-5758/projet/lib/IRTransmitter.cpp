#include "IRTransmitter.h"

IRTransmitter IRTransmitter::_instance;

IRTransmitter* IRTransmitter::getInstance() {
    return &_instance;
}

void IRTransmitter::turnOnPwm() {
    Timer::getInstance()->setConnectionMode(ConnectionMode::TOGGLE_MATCH);
    Timer::getInstance()->setPrescalar1();
    Timer::getInstance()->setCountCompareA(104);
}

void IRTransmitter::turnOffPwm() {
    Timer::getInstance()->setConnectionMode(ConnectionMode::OFF);
}

void IRTransmitter::sendOne() {
    turnOnPwm();
    _delay_us(1200);
    turnOffPwm();
    _delay_us(600);
}

void IRTransmitter::sendZero() {
    turnOnPwm();
    _delay_us(600);
    turnOffPwm();
    _delay_us(600);
}

void IRTransmitter::sendStartBit() {
    turnOnPwm();
    _delay_us(2400);
    turnOffPwm();
    _delay_us(600);
}

bool getBitAt(uint8_t byte, uint8_t position) {
    return (byte & (1 << position)) != 0;
}

void IRTransmitter::sendMessage(uint8_t msg) {
    sendStartBit();

    // Command
    for(int i = 0; i < 7; i++) {
        if(getBitAt(msg, i)) {
            sendOne();
        } else {
            sendZero();
        }
    }

    // Address
    for(int i = 0; i < 5; i++) {
        sendZero();
    }
}

void IRTransmitter::sendLine(const Line& line, bool lastLine) {

    //ENVOIE DES DATA DE LA LIGNE
    uint8_t data = 0;
    if (line.getFirstSide()) {
        data |= 0b01000000;
    }

    if (lastLine) {
        data |= 0b00000001;
    }

    LineLength LineLength = line.getRawLineType();
    switch (LineLength) {
        case (LineLength::UNDEFINED): {
            break;
        }
        case (LineLength::SMALL): {
            data |= 0b00010000;
            break;
        }
        case (LineLength::MEDIUM): {
            data |= 0b00100000;
            break;
        }
        case (LineLength::LONG): {
            data |= 0b00110000;
            break;
        }
    }

    sendMessage(data);
    _delay_ms(SEND_DELAY);

    //ENVOIE DE LA POSITION
    uint8_t positionData = line.getDistFromPrev();
    sendMessage(positionData);
    _delay_ms(SEND_DELAY);

    // CRC 16
    uint16_t crc = 0xFFFF;
    crc = _crc16_update(crc, data);
    crc = _crc16_update(crc, positionData);

    uint8_t crc0 = crc;
    sendMessage(crc0);
    _delay_ms(SEND_DELAY);

    uint8_t crc1 = (crc >> 7);
    sendMessage(crc1);
    _delay_ms(SEND_DELAY);

    uint8_t crc2 = (crc >> 14);
    sendMessage(crc2);
    _delay_ms(SEND_DELAY);
}