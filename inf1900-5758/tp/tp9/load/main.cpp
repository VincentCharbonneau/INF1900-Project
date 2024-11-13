#include <avr/io.h>
#include <USART.h>

#include "memoire_24.h"

const uint8_t HEADER_SIZE = 2;

uint8_t receiveAndWrite(Memoire24CXXX& memory, uint16_t& address) {
    uint8_t reading = USART::getInstance()->receive();
    memory.ecriture(address, &reading, sizeof(reading));
    address += 0x01;
    return reading;
}

int main() {
    Memoire24CXXX memory;
    uint16_t address = 0x0000;

    uint8_t sizeFirstByte = receiveAndWrite(memory, address);
    uint8_t sizeSecondByte = receiveAndWrite(memory, address);

    uint16_t fileSize = (sizeFirstByte << 8) | sizeSecondByte;
    uint16_t programSize = fileSize - HEADER_SIZE;
    uint16_t instructionsCount = programSize / 2;

    for(uint16_t i = 0; i < instructionsCount; i++) {
        receiveAndWrite(memory, address); // Instruction
        receiveAndWrite(memory, address); // Operand
    }

    // // For debugging purposes
    // USART::getInstance()->print("Adresses", address);
    // for(uint16_t i = 0; i < address; i++) {
    //     uint8_t reading;
    //     memory.lecture(i, &reading, sizeof(reading));
    //     USART::getInstance()->print("label", reading);
    // }
    
    return 0;
}

