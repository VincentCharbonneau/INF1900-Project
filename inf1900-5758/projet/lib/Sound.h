/*
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

#ifndef SOUND_H
#define SOUND_H

#include <avr/io.h>
#include <Connection.h>

class Sound {
    private:
    uint8_t PRESCALER=128;
    Connection* ground_ = nullptr;
    Connection* sound_ = nullptr;
    static const uint8_t notes[];
    static const uint16_t frequencies[];

    void init(uint16_t freq) ;

    public:
    void playNote(uint8_t noteNumber);
    Sound(Connection& ground, Connection& sound);
    ~Sound() = default;
    void stop();   

};

#endif