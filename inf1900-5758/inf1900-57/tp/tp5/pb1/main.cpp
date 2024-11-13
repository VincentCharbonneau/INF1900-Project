#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "memoire_24.h"
#include <string.h>
#include <avr/eeprom.h>

enum class LEDColors {RED, GREEN, AMBER, BLACK};
const uint8_t AMBER_DELAY = 1;

void displayColor(LEDColors color) {
    switch(color) {
        case LEDColors::RED:
            PORTA &= ~(1 << PORTA0);
            PORTA |= (1 << PORTA1);
            break;
        case LEDColors::GREEN:
            PORTA |= (1 << PORTA0);
            PORTA &= ~(1 << PORTA1);
            break;
        case LEDColors::AMBER:
            displayColor(LEDColors::RED);
            _delay_ms(AMBER_DELAY);

            displayColor(LEDColors::GREEN);
            _delay_ms(AMBER_DELAY);
            break;
        case LEDColors::BLACK:
            PORTA &= ~(1 << PORTA0);
            PORTA &= ~(1 << PORTA1);
            break;
    }
}




int main() {
    DDRA |= (1 << DDA0) | (1 << DDA1);

    Memoire24CXXX memory;

    char phrase[] = "testphrase";

    uint16_t address = 0x0000;
    uint8_t length = sizeof(phrase);

    memory.ecriture(address, (uint8_t*)phrase, length);

    uint8_t dataRead[length];
    memory.lecture(address, dataRead, length);

    char* result = (char*)dataRead;

    if(strcmp(phrase, result) == 0) {
        displayColor(LEDColors::GREEN);
    }
    else {
        displayColor(LEDColors::RED);
    }

    _delay_ms(2000);

    return 0;
}


