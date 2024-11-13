#include <util/delay.h>
#include <avr/io.h>
#include <Connection.h>
#include <avr/interrupt.h>
#include <Sound.h>
#include <Pwm.h>
#include <Timer.h>


void playVictorySound() {

    Connection ground('D', 6, true);
    Connection sound('D', 7, true);
    Sound snd(ground, sound);


    // snd.playNote(65); //E
    // snd.playNote(62); //D
    // snd.playNote(60); //C


    snd.playNote(65); //E
    _delay_ms(100);
    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(65); //E
    _delay_ms(100);
    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(65); //E
    _delay_ms(100);
    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(65); //E
    _delay_ms(400);
    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(60); //C
    _delay_ms(400);
    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(62); //D
    _delay_ms(400);
    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(65); //E
    _delay_ms(150);
    snd.stop(); //MUTE
    _delay_ms(150);

    snd.playNote(62); //D
    _delay_ms(150);
    snd.stop(); //MUTE
    _delay_ms(50);

    snd.playNote(65); //E
    _delay_ms(600);
    snd.stop(); //MUTE
}

int main() {
    playVictorySound();
    return 0;
}

