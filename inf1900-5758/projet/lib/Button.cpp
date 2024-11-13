#include "Button.h"
#include "USART.h"

Button::Button(IntNumber intNumber, ButtonInterruptBehavior behavior, bool pressedActivatesVoltage)
    : connection_(determineConnection(intNumber))
{
    pressedActivatesVoltage_ = pressedActivatesVoltage;

    emisk_INTn_ = 0;
    eifr_INTFn_ = 0;

    switch (intNumber)
    {
    case IntNumber::ZERO:
        emisk_INTn_ = INT0;
        eifr_INTFn_ = INTF0;
        setBehavior(behavior, ISC00, ISC01);
        break;

    case IntNumber::ONE:
        emisk_INTn_ = INT1;
        eifr_INTFn_ = INTF1;
        setBehavior(behavior, ISC10, ISC11);
        break;

    case IntNumber::TWO:
        emisk_INTn_ = INT2;
        eifr_INTFn_ = INTF2;
        setBehavior(behavior, ISC20, ISC21);
        break;
    }

    enableInterrupts();
}

bool Button::isButtonPressedDebounced()
{
    bool lecture = getButtonState();
    _delay_ms(DEBOUNCE_DELAY);
    return getButtonState() && lecture;
}

bool Button::isButtonReleasedDebounced()
{
    bool lecture = getButtonState();
    _delay_ms(DEBOUNCE_DELAY);
    return !getButtonState() && !lecture;
}

bool Button::getButtonState()
{
    if (pressedActivatesVoltage_)
        return getButtonSignal();
    else
        return !getButtonSignal();
}

bool Button::getButtonSignal()
{
    bool signal = connection_.get();
    return signal;
}

void Button::setSignalLowLevelBehavior(uint8_t ISCn0, uint8_t ISCn1)
{
    EICRA &= ~(1 << ISCn0) & ~(1 << ISCn1);
}

void Button::setSignalAnyEdgeBehavior(uint8_t ISCn0, uint8_t ISCn1)
{
    EICRA |= (1 << ISCn0);
    EICRA &= ~(1 << ISCn1);
}

void Button::setSignalFallingEdgeBehavior(uint8_t ISCn0, uint8_t ISCn1)
{
    EICRA &= ~(1 << ISCn0);
    EICRA |= (1 << ISCn1);
}

void Button::setSignalRisingEdgeBehavior(uint8_t ISCn0, uint8_t ISCn1)
{
    EICRA |= (1 << ISCn0) | (1 << ISCn1);
}

void Button::setBehavior(ButtonInterruptBehavior behavior, uint8_t ISCn0, uint8_t ISCn1)
{
    switch (behavior)
    {
    case ButtonInterruptBehavior::LOW_LEVEL:
        setSignalLowLevelBehavior(ISCn0, ISCn1);
        break;

    case ButtonInterruptBehavior::ANY_EDGE:
        setSignalAnyEdgeBehavior(ISCn0, ISCn1);
        break;

    case ButtonInterruptBehavior::ON_PRESS:
        if (pressedActivatesVoltage_)
        {
            setSignalRisingEdgeBehavior(ISCn0, ISCn1);
        }
        else
        {
            setSignalFallingEdgeBehavior(ISCn0, ISCn1);
        }
        break;

    case ButtonInterruptBehavior::ON_RELEASE:
        if (pressedActivatesVoltage_)
        {
            setSignalFallingEdgeBehavior(ISCn0, ISCn1);
        }
        else
        {
            setSignalRisingEdgeBehavior(ISCn0, ISCn1);
        }
        break;
    }
}

Connection Button::determineConnection(IntNumber intNumber)
{
    switch (intNumber)
    {
    case IntNumber::ZERO:
        return Connection(INT0_Connection_LETTER, INT0_Connection_NUMBER, false);
        break;

    case IntNumber::ONE:
        return Connection(INT1_Connection_LETTER, INT1_Connection_NUMBER, false);
        break;

    case IntNumber::TWO:
        return Connection(INT2_Connection_LETTER, INT2_Connection_NUMBER, false);
        break;
    }

    return Connection(INT0_Connection_LETTER, INT0_Connection_NUMBER, false);
}

void Button::enableInterrupts()
{
    EIMSK |= (1 << emisk_INTn_);
}

void Button::disableInterrupts()
{
    EIMSK &= ~(1 << emisk_INTn_);
}

void Button::clearAwaitingInterrupts()
{
    EIFR |= (1 << eifr_INTFn_);
}
