/*
 * DESCRIPTION
 * Cette classe permet un a des moteurs passer en paramettre de suivre la une ligne.
 * 
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

#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H

#include "LineSensor.h"
#include "Pwm.h"
#include <util/delay.h>

class LineFollower {
private:
    LineSensor& lineSensors;
    Pwm& motorController;

    // Constantes pour les valeurs binaires des capteurs de ligne
    const uint8_t LINE_DETECTED_LEFT = 0b00001000;
    const uint8_t LINE_DETECTED_RIGHT = 0b00000010;
    const uint8_t LINE_DETECTED_HARD_LEFT = 0b00010000;
    const uint8_t LINE_DETECTED_HARD_RIGHT = 0b00000001;

    
    // Vitesse des moteurs
    static const uint8_t MOTOR_SPEED_NORMAL_A = 40;
    static const uint8_t MOTOR_SPEED_TURN_A = MOTOR_SPEED_NORMAL_A + 15;


    static const uint8_t MOTOR_SPEED_NORMAL_B = 50; 
    static const uint8_t MOTOR_SPEED_TURN_B = MOTOR_SPEED_NORMAL_B + 15;

public:
    LineFollower(LineSensor& lineSensor, Pwm& motorCtrl);
    ~LineFollower()=default;

    void mircoAdjustRight();
    void mircoAdjustLeft();
    void mircoAdjustRight(uint8_t lineStatus);
    void mircoAdjustLeft(uint8_t lineStatus);
    
    void followLine();
    void followLineBackwards();

};

#endif // LINEFOLLOWER_H
