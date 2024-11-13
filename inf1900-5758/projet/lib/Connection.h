/*
 * Cette classe définit une connexion matérielle, encapsulant les détails de configuration et de 
 * manipulation des broches d'E/S spécifique d'un microcontrôleur AVR. Elle offre des méthodes pour configurer la direction et l'état de 
 * la broche, ainsi que pour récupérer des informations sur la broche.
 *
 * 
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Vincent Charbonneau, Rebecca Abi Chahine, Charles-Emile Leblond, Camil Bisson
 * 2024
 */
#ifndef CONNECTION_H
#define CONNECTION_H
#include <avr/io.h>
#include "USART.h"

class Connection {
private:
    volatile uint8_t* pinRegister_;
    volatile uint8_t pinNumber_;
    bool output_;
    volatile uint8_t* ddrRegister_;
    volatile uint8_t ddrNumber_;
    char pinName_;
    void updateDDR();
public:
    Connection(char pinName, uint8_t pinNumber, bool output);
    void set(bool value);
    bool get();
    const volatile uint8_t* getPinRegister() const;
    const volatile uint8_t getPinNumber() const;
    char getPinName() const;
};

#endif
