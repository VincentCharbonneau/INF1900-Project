#include "Connection.h"
#include "Led.h"
#include <avr/io.h>
#include "memoire_24.h"
#include <string.h>
#include <avr/eeprom.h>

int main() {
    Connection a0('A', 0, true);
    Connection a1('A', 1, true);
    LedColor green = LedColor::COLOR_GREEN;
    Led led(a0, a1);
    led.setColor(green);
    _delay_ms(1000);
    Memoire24CXXX memory;
    char phrase[] = "testphrase";
    uint16_t address = 0x0000;
    uint8_t length = sizeof(phrase);
    memory.ecriture(address, (uint8_t*)phrase, length);
    led.setColor(LedColor::COLOR_OFF);
    _delay_ms(1000);
    uint8_t dataRead[length];
    memory.lecture(address, dataRead, length);
    char* result = (char*)dataRead;
    if(strcmp(phrase, result) == 0) {
        led.setColor(green);
    }
    else {
        led.setColor(LedColor::COLOR_RED);
    }
    return 0;
}