#include "LineSensor.h"
#include "Pwm.h"
#include "LineFollower.h"
#include "LineSensor.h"
#include "Connection.h"
#include "Button.h"
#include <util/delay.h>


// int main() {
//     // Créez des objets Connection pour les capteurs et les moteurs.
//     // Les détails d'initialisation dépendent de votre plateforme matérielle.
//     Connection lineSensor1('A', 0, false); // exemple d'initialisation
//     Connection lineSensor2('A', 1, false);
//     Connection lineSensor3('A', 2, false);
//     Connection lineSensor4('A', 3, false);
//     Connection lineSensor5('A', 4, false);

//     Connection motorA('B', 4, true);
//     Connection motorB('B', 3, true);
//     Connection pwmDirA('B',2,true);
//     Connection pwmDirB('B',5,true);
//     Pwm motorController(pwmDirA, pwmDirB);



//     // Créez des objets Line et Pwm avec les connections précédemment créées.
//     LineSensor lineSensors(lineSensor1, lineSensor2, lineSensor3, lineSensor4, lineSensor5);

//     // Créez un objet LineFollower avec les objets Line et Pwm.
//     LineFollower follower(lineSensors, motorController);

//     // Boucle principale du programme.
    
//         // Exécutez le suivi de ligne.
//         while(true){
        
//         follower.followLine();
//         }


//         // Insérez ici une logique pour sortir de la boucle si nécessaire.
    

//     return 0;
// }

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
    

    Connection lineSensor1('A', 0, false); // exemple d'initialisation
    Connection lineSensor2('A', 1, false);
    Connection lineSensor3('A', 2, false);
    Connection lineSensor4('A', 3, false);
    Connection lineSensor5('A', 4, false);

    Connection motorA('B', 4, true);
    Connection motorB('B', 3, true);
    Connection pwmDirA('B',2,true);
    Connection pwmDirB('B',5,true);
    Pwm motorController(pwmDirA, pwmDirB);



    // Créez des objets Line et Pwm avec les connections précédemment créées.
    LineSensor lineSensors(lineSensor1, lineSensor2, lineSensor3, lineSensor4, lineSensor5);

    // Créez un objet LineFollower avec les objets Line et Pwm.
    LineFollower follower(lineSensors, motorController);


motorController.setDirectionA(false);
motorController.setDirectionB(false);
motorController.setPercentA(75);
motorController.setPercentB(100);

_delay_ms(30);
    while (true) {
        motorController.setDirectionA(false);
        motorController.setDirectionB(false);
        follower.followLine();

    } 
}