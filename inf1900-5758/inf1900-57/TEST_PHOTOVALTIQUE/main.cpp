#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "can.h"
#include <stdint.h>
#include <stdio.h>

enum class LEDColors {RED, GREEN, AMBER, BLACK};
const uint8_t AMBER_DELAY = 1;

void displayColor(LEDColors color) {
    switch(color) {
        case LEDColors::RED:
            PORTA &= ~(1 << PORTA0);
            PORTA |= (1 << PORTA1);
            break;
        case LEDColors::GREEN:
            PORTA |= (1 << PORTA0);
            PORTA &= ~(1 << PORTA1);
            break;
        case LEDColors::AMBER:
            displayColor(LEDColors::RED);
            _delay_ms(AMBER_DELAY);

            displayColor(LEDColors::GREEN);
            _delay_ms(AMBER_DELAY);
            break;
        case LEDColors::BLACK:
            PORTA &= ~(1 << PORTA0);
            PORTA &= ~(1 << PORTA1);
            break;
    }
}

unsigned char USART_Receive( void )
{
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) )
    ;
    /* Get and return received data from buffer */
    return UDR0;
}

void USART_Transmit(unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) )
    ;
    /* Put data into buffer, sends the data */
    UDR0 = data;

    displayColor(LEDColors::GREEN);
}

void initialisationUART ( void ) {
    // 2400 bauds.
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la réception et la transmission par le UART0
    //UCSR0A = 'modifier ici' ;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0B &= ~(1 << UCSZ02);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0C &= ~((1 << USBS0) | (1 << UPM01) | (1 << UPM00));
}

int main() {
    DDRB |= (1 << DDB0) | (1 << DDB1);
    DDRA &= ~(1 << DDA0);

    initialisationUART();

    can convertisseur;


    while(true) {
        uint16_t reading = convertisseur.lecture(0);
        uint8_t scaledReading = reading >> 2;
        char str[4];
        char space = ' ';

        sprintf(str, "%u", scaledReading);
        for (int i=0; i < 4; i++)
            USART_Transmit ( str[i] );
        USART_Transmit(space);
    }


    return 0;
}


