/*
 * Ce programme définit une classe Led permettant de contrôler une DEL à traversdeux connexions, une pour le pôle négatif (-) 
 * et une pour le pôle positif (+). Il offre des méthodes pour allumer la DEL en différentes couleurs et pour l'éteindre.
 * ConnectionMinus devrait être branchée sur la broche '-' de la DEL et ConnectionPlus devrait être branchée sur la broche '+' de la DEL.
 *
 * ConnectionMinus devrait être branchée sur la broche '-' de la del
 * ConnectionPlus devrait être branchée sur la broche '+' de la del
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
 *
 */

#ifndef LED_H
#define LED_H

#include "Connection.h"
#include <avr/io.h>
#include <util/delay.h>

const uint8_t AMBER_COLOUR_GREEN_LED_MS = 10;
const uint8_t AMBER_COLOUR_RED_LED_MS = 1;

enum class LedColor
{
    COLOR_RED,
    COLOR_GREEN,
    COLOR_AMBER,
    COLOR_OFF
};

class Led
{
private:
    Connection* connectionMinus_;
    Connection* connectionPlus_;

    void turnOnGreenLed();
    void turnOnRedLed();
    void turnOffLed();
    void turnOnAmberLed();

    LedColor currentColor_ = LedColor::COLOR_OFF;

public:
    Led(Connection&, Connection&);
    void setColor(LedColor);
    LedColor getColor();
};

#endif
