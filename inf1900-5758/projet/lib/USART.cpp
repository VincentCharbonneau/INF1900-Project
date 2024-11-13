#include "USART.h"


USART USART::instance_;

USART* USART::getInstance() {
    return &instance_;
}

USART::USART() {
    init();
}

void USART::init() const {
    // 2400 bauds.
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la transmission par le UART0
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0B &= ~(1 << UCSZ02);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0C &= ~((1 << USBS0) | (1 << UPM01) | (1 << UPM00));
}

void USART::print(const unsigned char data) const {
    while (!(UCSR0A & (1 << UDRE0))) {}

    UDR0 = data;
}

unsigned char USART::receive() const {    
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return UDR0;
}

void USART::print(const char* data) const {
    for (uint8_t i = 0; data[i] != '\0'; i++) {
        print(data[i]);
    }
}

void USART::print(const char* label, bool data) const {
    print(label);
    print(" : ");
    if (data) {
        print("true");
    }
    else {
        print("false");
    }
    print('\n');
}


void USART::print(const char* label, const char* data) const {
    print(label);
    print(" : ");
    print(data);
    print('\n');
}

void USART::print(const char* label, const uint8_t data) const {
    char dataStr[16];
    itoa(data, dataStr, 10);

    print(label, dataStr);
}

void USART::print(const char* label, const uint16_t data) const {
    char dataStr[16];
    itoa(data, dataStr, 10);

    print(label, dataStr);
}

void USART::print(const char* label, const uint32_t data) const {
    char dataStr[16];
    itoa(data, dataStr, 10);

    print(label, dataStr);
}


void USART::unicodeToUtf8(uint16_t unicodeChar, uint8_t utf8Bytes[4])
{
    if (unicodeChar <= 0x7F)
    {
        utf8Bytes[0] = static_cast<uint8_t>(unicodeChar);
        utf8Bytes[1] = 0;
    }
    else if (unicodeChar <= 0x7FF)
    {
        utf8Bytes[0] = static_cast<uint8_t>(0xC0 | (unicodeChar >> 6));
        utf8Bytes[1] = static_cast<uint8_t>(0x80 | (unicodeChar & 0x3F));
        utf8Bytes[2] = 0;
    }
    else if (unicodeChar <= 0xFFFF)
    {
        utf8Bytes[0] = static_cast<uint8_t>(0xE0 | (unicodeChar >> 12));
        utf8Bytes[1] = static_cast<uint8_t>(0x80 | ((unicodeChar >> 6) & 0x3F));
        utf8Bytes[2] = static_cast<uint8_t>(0x80 | (unicodeChar & 0x3F));
        utf8Bytes[3] = 0;
    }
    else if (unicodeChar <= 0x10FFFF)
    {
        utf8Bytes[0] = static_cast<uint8_t>(0xF0 | (unicodeChar >> 18));
        utf8Bytes[1] = static_cast<uint8_t>(0x80 | ((unicodeChar >> 12) & 0x3F));
        utf8Bytes[2] = static_cast<uint8_t>(0x80 | ((unicodeChar >> 6) & 0x3F));
        utf8Bytes[3] = static_cast<uint8_t>(0x80 | (unicodeChar & 0x3F));
    }
}
void USART::printUnicode(uint16_t unicode)
{
    // Assurez-vous que le code Unicode est dans la plage souhaitée (0x2500 - 0x257F)
    /*if (unicode < 0x2500 || unicode > 0x257F)
    {
        // Code Unicode invalide
        return;
    }*/
    uint8_t utf8Bytes[4];
    unicodeToUtf8(unicode, utf8Bytes);
    for (int i = 0; utf8Bytes[i] != '\0'; ++i)
    {
        print(utf8Bytes[i]);
    }
}
