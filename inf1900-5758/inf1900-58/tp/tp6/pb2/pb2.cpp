/*
TP2 Problème 2
Section de laboratoire : 3
Numéro d'équipe : 58
Noms :  Abi Chahine, Rebecca
        Leblond, Charles-Émile

Description du programme :
    La valeur de la photorésistance varie en fonction de l'intensité lumineuse
     qu'elle capte.
    Si la lumière est basse, la del prendra la couleur verte.
    Si la lumière est à bon niveau, la del prendra la couleur ambre.
    Si la lumière est trop forte, la del prendra la couleur rouge .

    Note : Nous avons trouvé les valeurs de la photorésistance en les lisant
           avec SerieViaUSB. Pour ce faire, nous avons converti le uint8_t
           en tableau de char et utilisé une méthode transmissionUART(uint8_t donnee)
           dans une boucle for pour envoyer une donnée à la fois.
           Nous en arrivons au résulat que la valeur est généralement supérieure ou
           égale à 236 lorsqu'on utilise une flashlight, et environ 215 à la lumière
           ambiante.

Identifications matérielles (Broches I/O) :
    B0 : Sortie
    B1 : Sortie
    A0 : Entrée

    B0 et B1 servent a créer une différence de potentiel pour allumer la del
             B1 est + et B0 est -
    A0 sert à recevoir la valeur de la photorésistance
*/


#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "can.h"
#include <string.h>


const uint8_t AMBER_COLOUR_GREEN_LED_MS = 10;
const uint8_t AMBER_COLOUR_RED_LED_MS = 1;
const uint8_t FLASH_LIGHT = 236;
const uint8_t AMBIANT_LIGHT = 215;


void turnOnGreenLed()
{
    PORTB &= ~(1 << PB1);
    PORTB |= (1 << PB0);
}

void turnOnRedLed()
{
    PORTB &= ~(1 << PB0);
    PORTB |= (1 << PB1);
}

void turnOnAmberLed()
{
    turnOnGreenLed();
    _delay_ms(AMBER_COLOUR_GREEN_LED_MS);
    turnOnRedLed();
    _delay_ms(AMBER_COLOUR_RED_LED_MS);
}

enum class LedState
{
    GREEN,
    AMBER,
    RED
};

void ledLogic(LedState &ledState)
{
    switch (ledState)
    {
        case LedState::GREEN:
            turnOnGreenLed();
            break;

        case LedState::AMBER:
            turnOnAmberLed();
            break;

        case LedState::RED:
            turnOnRedLed();
            break;
    }
}

int main()
{
    DDRB |= (1 << PB0) | (1 << PB1);
    DDRA &= ~(1 << PA0);

    can convertisseur;
    LedState ledState = LedState::GREEN;

    while (true)
    {
        uint16_t valeurLue = convertisseur.lecture(0);
        uint8_t valeur8Bits = static_cast<uint8_t>(valeurLue >> 2);

        if (valeur8Bits >= FLASH_LIGHT)
            ledState = LedState::RED;
        else if (valeur8Bits >= AMBIANT_LIGHT)
            ledState = LedState::AMBER;
        else
            ledState = LedState::GREEN;

        ledLogic(ledState);
    }
}
