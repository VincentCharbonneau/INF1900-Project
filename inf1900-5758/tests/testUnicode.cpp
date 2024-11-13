#include "USART.h"

int main()
{
    USART::getInstance()->printUnicode(0x2554);
    USART::getInstance()->printUnicode(0x2550);
    USART::getInstance()->printUnicode(0x2566);
    USART::getInstance()->printUnicode(0x2550);
    USART::getInstance()->printUnicode(0x2557);
    USART::getInstance()->print('\n');
    USART::getInstance()->printUnicode(0x2551);
    USART::getInstance()->print(' ');
    USART::getInstance()->printUnicode(0x2551);
    USART::getInstance()->print(' ');
    USART::getInstance()->printUnicode(0x2551);
    USART::getInstance()->print('\n');
    USART::getInstance()->printUnicode(0x2560);
    USART::getInstance()->printUnicode(0x2550);
    USART::getInstance()->printUnicode(0x256C);
    USART::getInstance()->printUnicode(0x2550);
    USART::getInstance()->printUnicode(0x2563);
    USART::getInstance()->print('\n');
    USART::getInstance()->printUnicode(0x2551);
    USART::getInstance()->print(' ');
    USART::getInstance()->printUnicode(0x2551);
    USART::getInstance()->print(' ');
    USART::getInstance()->printUnicode(0x2551);
    USART::getInstance()->print('\n');
    USART::getInstance()->printUnicode(0x255A);
    USART::getInstance()->printUnicode(0x2550);
    USART::getInstance()->printUnicode(0x2569);
    USART::getInstance()->printUnicode(0x2550);
    USART::getInstance()->printUnicode(0x255D);
}