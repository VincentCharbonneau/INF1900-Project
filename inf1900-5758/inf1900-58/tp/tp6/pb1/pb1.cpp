/*
TP6 Problème 1
Section de laboratoire : 3
Numéro d'équipe : 58
Noms :  Abi Chahine, Rebecca
        Leblond, Charles-Émile

Description du programme :
    Ce programme fait en sorte qu'un compteur s'incrémente 10 fois 
    par seconde quand un bouton-poussoir externe est enfoncé.Quand le bouton 
    est relaché ou que le compteur atteint 120, la del clignote en vert pour 
    une demie seconde puis s'éteint. Deux secondes plus tard, la del s'allume
    en rouge au rythme de 2 fois par seconde, selon un nombre de fois 
    correspondant à la valeur du compteur. Ensuite, la del devient verte 
    pendant 1 seconde avant de revenir à son état initial.
    On peut ensuite recommencer.

    Note: Nous avons utilisé la division d'horloge par 1024, donc le délai de 
          notre minuterie est 8 000 000 / 1024 / 10 = 781 (arrondi) pour un 
          temps de 0,1 seconde.

Identifications matérielles (Broches I/O) :
    B0 : Entrée
    B1 : Entrée
    D2 : Sortie

    B0 et B1 servent a créer une différence de potentiel pour allumer la del
             B1 est + et B0 est -
    D2 est associé au boutton-poussoir externe, permettant de savoir s'il 
       est appuyé
       Quand il est appuyé, il passe de 5V à 0V
       Quand il est relâché, il passe de 0V à 5V
*/


#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile uint8_t gTimerExpired = 0;
volatile uint8_t gPushButton = 0;
volatile uint8_t gCounter = 0;
volatile uint8_t gIsCount = 0;

const uint16_t DELAY = 781;
const uint8_t COUNTER_MAX = 120;
const uint8_t DEBOUNCE_DELAY_MS = 10;
const uint8_t UN_QUART_SECONDE_MS = 250;
const uint16_t UNE_DEMIE_SECONDE_MS = 500;
const uint16_t UNE_SECONDE_MS = 1000;
const uint16_t DEUX_SECONDES_MS = 2000;


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

void turnOffLed()
{
    PORTB &= ~(1 << PB0) & ~(1 << PB1);
}

bool isButtonPressed()
{
    return PIND & (1 << PD2);
}

bool debounce()
{
    bool lecture = !isButtonPressed();

    _delay_ms(DEBOUNCE_DELAY_MS);
    return !isButtonPressed() && lecture;
}

void startTimer(uint16_t duration)
{
    TCNT1 = 0;
    OCR1A = duration;
    TCCR1A = 0;
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);
}

void initialization()
{
    cli();

    DDRB |= (1 << PB0) | (1 << PB1);
    DDRD &= ~(1 << PD2);
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    sei();
}

void flashRed()
{
    for (uint8_t i = 0; i <= gCounter; i++)
    {
        turnOnRedLed();
        _delay_ms(UN_QUART_SECONDE_MS);
        turnOffLed();
        _delay_ms(UN_QUART_SECONDE_MS);
    }
}

void reset()
{
    gCounter = 0;
    gTimerExpired = 0;
    gPushButton = 0;
}

void ledLogic()
{
    turnOnGreenLed();
    _delay_ms(UNE_DEMIE_SECONDE_MS);
    turnOffLed();
    _delay_ms(DEUX_SECONDES_MS);
    flashRed();
    turnOnGreenLed();
    _delay_ms(UNE_SECONDE_MS);
    turnOffLed();
}

ISR(TIMER1_COMPA_vect)
{
    if (gIsCount)
    {
        if (gCounter >= COUNTER_MAX)
            gTimerExpired = 1;
        else
            gCounter++;
    }
}

ISR(INT0_vect)
{
    if (debounce())
    {
        reset();

        gIsCount = 1;

        startTimer(DELAY);
    }
    if (!debounce() && gIsCount)
    {
        gPushButton = 1;
        gIsCount = 0;
    }
    EIFR |= (1 << INTF0);
}

int main()
{
    initialization();

    while (true)
    {
        while (gTimerExpired == 0 && gPushButton == 0)
        {

        }

        ledLogic();
        reset();
    }
}
