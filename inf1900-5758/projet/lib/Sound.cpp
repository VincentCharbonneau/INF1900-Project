#define F_CPU 8000000UL

#include "Sound.h"
#include "USART.h"

Sound::Sound(Connection &ground, Connection &sound)
{
    ground_ = &ground;
    ground_->set(0);
    sound_ = &sound;
    sound_->set(1);
    
}

const uint8_t Sound::notes[] = { //son grave=45 , son aigus= 81
    45, 46, 47, 48,
    49, 50, 51, 52, 
    53, 54, 55, 56, 
    57, 58, 59, 60, 
    61, 62, 63, 64, 
    65, 66, 67, 68,
    69, 70, 71, 72,
    73, 74, 75, 76,
    77, 78, 79, 80,
    81
    };

const uint16_t Sound::frequencies[] = {
    110, 116, 123, 130,
    138, 146, 155, 164,
    174, 185, 196, 207,
    220, 233, 246, 261,
    277, 293, 311, 329,
    349, 369, 392, 415,
    440, 466, 493, 523,
    554, 587, 622, 659,
    698, 739, 783, 830,
    880
    };


void Sound::init(uint16_t freq) 
{
    TCNT2 = 0; // Réinitialise le compteur

    // Configurez la broche de sortie pour le son
    DDRD |= (1 << DDD6); // Exemple : Utilisez DDD6 pour OC2B

    // Configure OC2B pour basculer sur Compare Match (Toggle on Compare Match)
    TCCR2A = (1 << COM2B0) | (1 << WGM21); // Mode CTC

    // Définissez le prescaler à 128
    TCCR2B = ((1 << CS22) | (1 << CS20));

    // Calculez la valeur de comparaison
    uint16_t compareVal = (uint16_t)((F_CPU / (2.0 * PRESCALER * freq)) - 1);
    OCR2A = compareVal; // Ajustez OCR2A pour correspondre à la fréquence
}

void Sound::playNote(uint8_t noteNumber) {
    uint8_t noteIndex = noteNumber - 45; // Convertir le numéro de note en index
    if(noteIndex < sizeof(notes)/sizeof(notes[0])) { // Vérifiez que l'index est dans le tableau
        uint16_t freq = frequencies[noteIndex]; // Obtenez la fréquence correspondante
        init(freq); // Initialisez le timer avec cette fréquence
    }
}

void Sound::stop() {
    OCR2A = 0;
}