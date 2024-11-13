#include "Led.h"

Led::Led(Connection& connectionMinus, Connection& connectionPlus)
{
    connectionMinus_ = &connectionMinus;
    connectionPlus_ = &connectionPlus;
}

void Led::turnOnGreenLed()
{
    connectionMinus_->set(true);
    connectionPlus_->set(false);
}

void Led::turnOnRedLed()
{
    connectionMinus_->set(false);
    connectionPlus_->set(true);
}

void Led::turnOffLed()
{
    connectionMinus_->set(false);
    connectionPlus_->set(false);
}

void Led::turnOnAmberLed()
{
    turnOnGreenLed();
    _delay_ms(AMBER_COLOUR_GREEN_LED_MS);
    turnOnRedLed();
    _delay_ms(AMBER_COLOUR_RED_LED_MS);
}

void Led::setColor(LedColor ledColor)
{
    currentColor_ = ledColor;
    switch (currentColor_)
    {
    case LedColor::COLOR_AMBER:
        turnOnAmberLed();
        break;

    case LedColor::COLOR_GREEN:
        turnOnGreenLed();
        break;

    case LedColor::COLOR_RED:
        turnOnRedLed();
        break;
        
    case LedColor::COLOR_OFF:
        turnOffLed();
        break;
    }
}

LedColor Led::getColor()
{
    return currentColor_;
}
