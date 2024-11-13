#include <util/delay.h>
#include <avr/io.h>
#include <USART.h>
#include <DistanceSensor.h>
#include <Can.h>
#include <Pwm.h>
#include <Connection.h>
#include <Sound.h>

Connection motorLeftOut('B', 4, true);
Connection motorLeftDir('B', 5, true);
Connection motorRightOut('B', 3, true);
Connection motorRightDir('B', 2, true);
Pwm pwm(motorRightDir, motorLeftDir);
Connection piezoGround('D', 6, true);
Connection piezoSignal('D', 7, true);
Sound sound(piezoGround, piezoSignal);
Connection button('D', 2, false);

uint16_t getRawDistanceValue()
{
    return Can::getInstance()->lecture(0);
}

void playErrorSound()
{
    sound.playNote(45); // La note la plus grave
    _delay_ms(600);
    sound.stop();
}

void playWaitingSound()
{
    for (uint8_t i = 0; i < 3; ++i)
    {
        sound.playNote(81); // Joue un son aigu
        _delay_ms(50);
        sound.stop();
        _delay_ms(50);
    }
}

void sort(uint16_t arr[], uint8_t size)
{
    for (uint8_t i = 0; i < size - 1; i++)
    {
        for (uint8_t j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // Swap arr[j] and arr[j+1]
                uint16_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

uint16_t getFilterdDistance()
{
    uint16_t distances[]{0, 0, 0, 0, 0};
    uint8_t size = 5;
    for (uint8_t i = 0; i < size; i++)
    {
        distances[i] = getRawDistanceValue();
    }
    sort(distances, size);
    return distances[size / 2];
}

void turnLeft(uint8_t pct)
{
    pwm.setDirectionA(false);
    pwm.setDirectionB(true);
    pwm.setPercentA(pct); //-20
    pwm.setPercentB(pct);
}

void stop()
{
    pwm.setPercentA(0);
    pwm.setPercentB(0);
}

void moveForward(uint8_t pct)
{
    pwm.setDirectionB(false);
    pwm.setDirectionA(false);
    pwm.setPercentA(pct);     // right
    pwm.setPercentB(pct - 4); // left //11
}

void moveBackward(uint8_t pct)
{
    pwm.setDirectionA(true);
    pwm.setDirectionB(true);
    pwm.setPercentA(pct);     // right
    pwm.setPercentB(pct - 1); // left //4
}

void blastForward()
{
    pwm.setDirectionA(0);
    pwm.setDirectionB(0);
    pwm.setPercentA(100);
    pwm.setPercentB(100); // 92
    _delay_ms(150);
}

void blastBackward()
{
    pwm.setDirectionA(1);
    pwm.setDirectionB(1);
    pwm.setPercentA(100);
    pwm.setPercentB(100);
    _delay_ms(150);
}

int main()
{
    // while (true) {
    //     USART::getInstance()->print("DISTANCE", getFilterdDistance());
    //     _delay_ms(1000);
    // }
    // 4 coins
    // sortie 3 fois
    // milieu 2 fois
    // initial 4 fois 2 sens
    // uint16_t distances[65];
    // for (int i = 0; i < 65; i += 5)
    // {
    //     for (uint8_t j = 0; j < 5; j++)
    //     {
    //         _delay_ms(700);
    //         uint16_t distance = getFilterdDistance();
    //         distances[i] = distance;
    //     }
    //     playErrorSound();
    //     _delay_ms(6000);
    //     playWaitingSound();
    // }
    // while (true)
    // {
    //     if (button.get())
    //     {
    //         for (int i = 0; i < 65; i += 5)
    //         {
    //             for (uint8_t j = 0; j < 5; j++)
    //             {
    //                 USART::getInstance()->print("MESURE", distances[i]);
    //             }
    //             USART::getInstance()->print("PROCHAINE MESURE\n");
    //         }
    //     }
    // }

    // right : A
    // left : B

    _delay_ms(1500);
    blastForward();
    moveForward(60);
    // _delay_ms(200);
    // stop();
    // turnLeft(75);
    _delay_ms(3500);

    // pwm.setPercentB(0);
    // pwm.setPercentA(60);
    // _delay_ms(85);
    stop();
    _delay_ms(2000);
    blastBackward();
    moveBackward(60);
    _delay_ms(2800);
    stop();

    return 0;
}

//(2.75; 0.125) -> (140,25; 0,125)
//(47; 1/30)
