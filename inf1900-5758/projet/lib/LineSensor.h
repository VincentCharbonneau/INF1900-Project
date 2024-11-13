/*
 * DESCRIPTION
 * Cette classe permet de detecter la presence d'une ligne et de stocker cette information
 * dans un uint8_t.
 *
 * PORTS UTILISES
 * 
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

#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <avr/io.h>
#include <util/delay.h>
#include "Connection.h"

class LineSensor
{
private:

static const uint8_t SENSOR2 = 1; 
static const uint8_t SENSOR3 = 2;
static const uint8_t SENSOR4 = 3;
static const uint8_t SENSOR5 = 4;


    Connection* line1_;
    Connection* line2_;
    Connection* line3_;
    Connection* line4_;
    Connection* line5_;

public:
    LineSensor(Connection&, Connection&, Connection&, Connection&, Connection&);
    uint8_t getLineDetection();
};

#endif