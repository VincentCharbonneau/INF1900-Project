#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "memoire_24.h"

void turnOnGreenLed()
{
    PORTA &= ~(1 << PA1);
    PORTA |= (1 << PA0);
}

void turnOnRedLed()
{
    PORTA &= ~(1 << PA0);
    PORTA |= (1 << PA1);
}

void turnOffLed()
{
    PORTA &= ~(1 << PA0);
    PORTA &= ~(1 << PA1);
}

int main()
{
    Memoire24CXXX memoire;

    DDRA |= (1 << PA0) | (1 << PA1);

    char chaine1[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L";
    uint8_t length = sizeof(chaine1);
    char chaine2[length];


    for (uint8_t i = 0; i < length; i++)
    {
        memoire.ecriture(i, (uint8_t)chaine1[i]);
        _delay_ms(5);
    }
    for (uint8_t i = 0; i < length; i++)
    {
        /*
        uint8_t output;
        uint8_t* addressOutput = &output;
        memoire.lecture(i, addressOutput);
        chaine[i] = output;
        */

        memoire.lecture(i, (uint8_t*)&chaine2[i]);
        _delay_ms(5);
    }
    bool sameString = true;
    for (uint8_t i = 0; i < length; i++)
    {
        if (chaine1[i] != chaine2[i])
        {
            sameString = false;
            break;
        }
    }
    if (sameString)
        turnOnGreenLed();
    else
        turnOnRedLed();
}
