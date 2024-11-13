#include <util/delay.h>
#include <avr/io.h>
#include <Connection.h>
#include <LineSensor.h>
#include <USART.h>

// La fonction d'aide pour afficher en binaire
void printBinary(uint8_t value) {
    for(int i = 7; i >= 0; i--) {
        if(value & (1 << i)) {
            USART::getInstance()->print('1');
        } else {
            USART::getInstance()->print('0');
        }
    }
    USART::getInstance()->print('\n'); // Envoyer une nouvelle ligne pour séparer les valeurs
}
int main() {
    
    Connection d0('B', 0, false);
    Connection d1('B', 1, false);
    Connection d2('B', 2, false);
    Connection d3('B', 3, false);
    Connection d4('B', 4, false);
    LineSensor line(d0,d1,d2,d3,d4);
    while (true) {

        uint8_t reading = line.getLineDetection();
       // USART::getInstance()->print("Valeur retournee : ", reading);
        printBinary(reading);
        _delay_ms(1000);
    }



    return 0;
}       
