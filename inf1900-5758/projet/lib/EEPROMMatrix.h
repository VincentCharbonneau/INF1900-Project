/*
 * DESCRIPTION
 * Cette classe est permet le stockage et l'affichage de caractères dans la EEPROM vers serieViaUSB.
 * 
 *
 * PORTS UTILISES
 * aucun directement, mais la classe utilise la classe Memoire24CXXX
 * 
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Camil Bisson
 * 2024
 *
 * Code qui n'est sous aucune license.
 *
 */

#ifndef EEPROM_MATRIX
#define EEPROM_MATRIX

#include "MatrixProxy.h"
#include "memoire_24.h"
#include "USART.h"
#include <util/delay.h>
#include "Line.h"
#include "DrawableLine.h"
#include "DrawableChar.h"

class EEPROMMatrix : public MatrixProxy {
private:
    uint16_t m_startAddress;
    Memoire24CXXX& m_eeprom;
    
public:
    EEPROMMatrix(const uint8_t nRows, const uint8_t nColumns, uint16_t defaultChar, uint16_t startAddress, Memoire24CXXX& eeprom);

    virtual uint16_t get(const uint8_t row, const uint8_t column) const override;
    virtual void set(const uint8_t row, const uint8_t column, uint16_t val) override;

    void display() const;


    void drawLine(const Line& line);
};

#endif