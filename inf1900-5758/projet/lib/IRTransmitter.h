/*
 * DESCRIPTION
 * Cette classe permet de transmettre des messages IR. Elle utilise le protocol SIRC.
 * 
 * 
 *
 * ELEMENT UTILISES
 * Cette classe utilise le Timer 1 pour generer le signal de 50% de dutycycle, et une frequence de 38kHz.
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

#ifndef IR_TRANSMITTION_H
#define IR_TRANSMITTION_H

#include "Timer.h"
#include "util/delay.h"
#include "Line.h"
#include <util/crc16.h>

class IRTransmitter {
    private:
        IRTransmitter() = default;
        IRTransmitter(const IRTransmitter&) = delete;
        IRTransmitter& operator=(const IRTransmitter&) = delete;

        static IRTransmitter _instance;

    public:

        static const uint8_t SEND_DELAY = 200;
        static IRTransmitter* getInstance();

        void turnOnPwm();
        void turnOffPwm();

        void sendStartBit();
        void sendOne();
        void sendZero();

        void sendMessage(uint8_t msg);

        void sendLine(const Line& line, bool lastLine=false);
};

#endif
