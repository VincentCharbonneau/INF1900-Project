// Titre:       INF1900 - TP6: Probleme #2
// Auteurs:     Camil Bisson et Vincent Charbonneau
// Description: Ce programme permet de faire allumer la DEL libre
//              selon la luminosite captee par la photoresistance
//              Faible luminosite:      VERT
//              Luminosite ambiante:    AMBRE
//              Luminosite forte:       ROUGE

// Identification materielle des ports et pins:
// - A0 (IN): Photoresistance
// - B0/B1 (OUT): DEL libre

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "can.h"

enum class LedColor {
    RED, 
    GREEN, 
    AMBER,
    BLACK
};

const uint8_t AMBER_DELAY = 1;

const uint8_t MIN_LUMINOSITY_THRESHOLD = 180;
const uint8_t HIGH_LUMINOSITY_THRESHOLD = 20;

void displayColor(LedColor color) {
    switch(color) {
        case LedColor::RED:
            PORTB &= ~(1 << PORTB0);
            PORTB |= (1 << PORTB1);
            break;
        case LedColor::GREEN:
            PORTB |= (1 << PORTB0);
            PORTB &= ~(1 << PORTB1);
            break;
        case LedColor::AMBER:
            displayColor(LedColor::RED);
            _delay_ms(AMBER_DELAY);

            displayColor(LedColor::GREEN);
            _delay_ms(AMBER_DELAY);
            break;  
        case LedColor::BLACK:
            PORTB &= ~(1 << PORTB0);
            PORTB &= ~(1 << PORTB1);
            break;
    }
}

void init(void) {
    DDRB |= (1 << DDB0) | (1 << DDB1);
    DDRA &= ~(1 << DDA0);
}

int main() {
    init();

    can convertisseur;

    while(true) {
        uint16_t reading = convertisseur.lecture(0);
        uint8_t scaledReading = reading >> 2;

        if(scaledReading > MIN_LUMINOSITY_THRESHOLD) {
            displayColor(LedColor::GREEN);
        }
        else if(scaledReading > HIGH_LUMINOSITY_THRESHOLD) {
            displayColor(LedColor::AMBER);
        }
        else {
            displayColor(LedColor::RED);
        }
    }

    return 0;
}


