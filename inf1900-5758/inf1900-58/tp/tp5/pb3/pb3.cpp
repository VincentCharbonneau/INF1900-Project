#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "memoire_24.h"


void initialisationUART()
{
    UBRR0H = 0;
    UBRR0L = 0xCF;

    UCSR0A |= (1 << RXC0) | (1 << TXC0);
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void transmissionUART(uint8_t donnee)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = donnee;
}

int main()
{
    Memoire24CXXX memoire;

    initialisationUART();

    char chaine1[] = "fini le tp5!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    uint8_t length = sizeof(chaine1);
    char chaine2[length];

    for (uint8_t i = 0; i < length; i++)
    {
        memoire.ecriture(i, (uint8_t)chaine1[i]);
        _delay_ms(5);
    }

    for (uint8_t i = 0; i < length; i++)
    {
        memoire.lecture(i, (uint8_t*)&chaine2[i]);
        _delay_ms(5);
    }

    for (uint8_t i = 0; i < 5; i++)
    {
        for (uint8_t j = 0; j < length; j++)
        {
            transmissionUART(chaine2[j]);
        }
    }
}