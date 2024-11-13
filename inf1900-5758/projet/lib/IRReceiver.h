/*
 * DESCRIPTION
 * Cette classe permet de recevoir un signal infrarouge et de le decoder pour obtenir des lignes.
 * 
 *
 * PORTS UTILISES
 * Aucun directement
 * 
 * 
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours INF1900
 *
 * Vincent Charbonneau, Camil Bisson
 * 2024
 *
 * Code qui n'est sous aucune license.
 *
 */


#ifndef IR_RECEIVE_H
#define IR_RECEIVE_H

#include "Connection.h"
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "Line.h"
#include "TrameConsumer.h"

class IRReceiver {
private:
    Connection* signal_ = nullptr;
    uint8_t data = 0b00000000;
    uint8_t signalCount = 0;
    uint8_t workingBit = 0;
    bool startBitSeen = false;

    TrameConsumer& _consumer;
public:
    IRReceiver(Connection& signal, TrameConsumer& consumer);
    ~IRReceiver() = default;
    
    bool getSignal(void);

    void saveData(uint8_t);
    void readMessage(void);
    Line receiveLine();

    uint8_t getTrame();
};

#endif
