#include "Connection.h"

Connection::Connection(char pinName, uint8_t pinNumber, bool output)
{
    if (pinName < 65 || pinName > 68)
    {
        USART::getInstance()->print("[WARN] A connection pin name must be within A-D");
    }

    if (pinNumber < 0 || pinNumber > 7)
    {
        USART::getInstance()->print("[WARN] A connection pin number mut be within 0-7");
    }

    pinName_ = pinName;
    output_ = output;
    pinNumber_ = pinNumber;
    ddrNumber_ = pinNumber;

    switch (pinName)
    {
    case 'A':
        pinRegister_ = output ? &PORTA : &PINA;
        ddrRegister_ = &DDRA;
        break;

    case 'B':
        pinRegister_ = output ? &PORTB : &PINB;
        ddrRegister_ = &DDRB;
        break;

    case 'C':
        pinRegister_ = output ? &PORTC : &PINC;
        ddrRegister_ = &DDRC;
        break;

    case 'D':
        pinRegister_ = output ? &PORTD : &PIND;
        ddrRegister_ = &DDRD;
        break;
    }

    updateDDR();
}

void Connection::set(bool value)
{
    if (value)
    {
        *pinRegister_ |= (1 << pinNumber_);
    }
    else
    {
        *pinRegister_ &= ~(1 << pinNumber_);
    }
}

bool Connection::get()
{
    return (*pinRegister_ & (1 << pinNumber_));
}

void Connection::updateDDR()
{
    if (output_)
    {
        *ddrRegister_ |= (1 << ddrNumber_);
    }
    else
    {
        *ddrRegister_ &= ~(1 << ddrNumber_);
    }
}

const volatile uint8_t *Connection::getPinRegister() const
{
    return pinRegister_;
}

char Connection::getPinName() const
{
    return pinName_;
}

const volatile uint8_t Connection::getPinNumber() const
{
    return pinNumber_;
}
