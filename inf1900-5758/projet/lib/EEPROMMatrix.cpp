#include "EEPROMMatrix.h"

EEPROMMatrix::EEPROMMatrix(const uint8_t nRows, const uint8_t nColumns, uint16_t defaultChar, uint16_t startAddress, Memoire24CXXX& eeprom)
 : MatrixProxy(nRows, nColumns, defaultChar), m_startAddress(startAddress), m_eeprom(eeprom) {
    for (uint8_t row = 0; row < nRows; row++) {
        for (uint8_t column = 0; column < nColumns; column++) {
            set(row, column, defaultChar);
        }
    }
}

void EEPROMMatrix::set(const uint8_t row, const uint8_t column, uint16_t val) {
    uint8_t mostSignificantByte = val >> 8;
    uint8_t leastSignificantByte = val;

    uint16_t msbAddress = m_startAddress + (row * m_nColumns + column) * 2;
    uint16_t lsbAddress = msbAddress + 1;

    m_eeprom.ecriture(msbAddress, mostSignificantByte);
    _delay_ms(2);
    m_eeprom.ecriture(lsbAddress, leastSignificantByte);
    _delay_ms(2);
}

uint16_t EEPROMMatrix::get(const uint8_t row, const uint8_t column) const {\
    uint8_t mostSignificantByte;
    uint8_t leastSignificantByte;

    uint16_t msbAddress = m_startAddress + (row * m_nColumns + column) * 2;
    uint16_t lsbAddress = msbAddress + 1;

    m_eeprom.lecture(msbAddress, &mostSignificantByte);
    _delay_ms(2);
    m_eeprom.lecture(lsbAddress, &leastSignificantByte);\
    _delay_ms(2);

    return (mostSignificantByte << 8) | leastSignificantByte;
}

void EEPROMMatrix::display() const {
    for (uint8_t row = 0; row < m_nRows; row++) {
        for (uint8_t column = 0; column < m_nColumns; column++) {
            uint16_t val = get(row, column);
            USART::getInstance()->printUnicode(val);
        }
        USART::getInstance()->print('\n');
    }
}