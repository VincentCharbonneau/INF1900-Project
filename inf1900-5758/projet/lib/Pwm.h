/*
 * Cette classe permet de contrôler le PWM ur des broches de sortie d'un microcontrôleur AVR. ELle permet de configurer la direction
 * des broches de sortie, régler le pourcentage de PWM sur chaque broche et changer la direction du signal PWM.
 *
 * Les ports suivants sont réservés pour les broches de sortie PWM:
 * - OC0A (PB3)
 * - OC0B (PB4)
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

#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
#include <Connection.h>

class Pwm {
    private:
    void init(void) const;

    Connection* directionIOA_ = nullptr;
    Connection* directionIOB_ = nullptr;

    public:
    Pwm(Connection& directionIOA, Connection& directionIOB);
    ~Pwm() = default;    

    void setPercentA(uint8_t pct);
    void setPercentB(uint8_t pct);

    void setDirectionA(bool direction);
    void setDirectionB(bool direction);

};

#endif
