#include <util/delay.h>
#include <avr/io.h>
#include <Connection.h>
#include <avr/interrupt.h>
#include <Sound.h>
#include <Pwm.h>
#include <Timer.h>

int main()
{
    Connection ground('D', 6, true);
    Connection sound('D', 7, true);
    Sound snd(ground, sound);


    //Smoke on the water main riff (scuffed version)
    while (true) {
    snd.playNote(64); //E
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(67); //G
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(69); //A
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(64); //E
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(67); //G
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(70); //A#
    _delay_ms(690);
    snd.playNote(69); //A
    _delay_ms(1000);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(64); //E
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(67); //G
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(69); //A
    _delay_ms(900);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(67); //G
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);

    snd.playNote(64); //E
    _delay_ms(500);

    snd.stop(); //MUTE
    _delay_ms(100);
    }

    

    return 0;
}