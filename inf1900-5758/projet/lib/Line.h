/*
 * DESCRIPTION
 * Cette classe est utilise pour stocker les informations d'une ligne. 
 * Elle contient la longueur de la ligne, la distance de la ligne par rapport a la ligne precedente
 *
 * PORTS UTILISES
 * Aucun
 * 
 * 
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Vincent Charbonneau, Rebecca Abi Chahine, Charles-Emile Leblond, Camil Bisson
 * 2024
 *
 * Code qui n'est sous aucune license.
 *
 */


#ifndef LINE_H
#define LINE_H


#include <stdint.h>
#include "USART.h"


enum class LineLength {
    SMALL, MEDIUM, LONG, UNDEFINED
};

class Line {
    private:
        uint16_t distFromPrev;
        LineLength type;
        bool firstSide;
        uint16_t length;

        LineLength getLineType(uint16_t pos);
    public:
        Line();
        Line(uint16_t distFromPrev, uint16_t length, bool firstSide);
        void display();
        bool getFirstSide() const {return firstSide;}
        uint16_t getRawLength() const {return length;}
        LineLength getRawLineType() const {return type;}
        uint16_t getDistFromPrev() const {return distFromPrev;}
        
        //temporary for testing
        void setLineType(LineLength type_) {type = type_; }
        
};

#endif