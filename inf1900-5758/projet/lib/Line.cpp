#include "Line.h"

LineLength Line::getLineType(uint16_t pos) {
    if(pos <= 1) {
        return LineLength::SMALL;
    }
    else if(pos <= 4) {
        return LineLength::MEDIUM;
    }
    else {
        return LineLength::LONG;
    }
}

Line::Line(): distFromPrev(0), type(LineLength::UNDEFINED), firstSide(false), length(0) {}

Line::Line(uint16_t distFromPrev, uint16_t length, bool firstSide):
    distFromPrev(distFromPrev), type(getLineType(length)), firstSide(firstSide), length(length) {}

void Line::display() { //for debugging
    USART::getInstance()->print("distFromPrev", distFromPrev);
    USART::getInstance()->print("\n");
    USART::getInstance()->print("firstSide?", firstSide);
    USART::getInstance()->print("\n");
    
}