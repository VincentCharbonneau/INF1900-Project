#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t DEBOUNCE_DELAY_MS = 10;
const uint8_t AMBER_COLOUR_GREEN_LED_MS = 30;
const uint8_t AMBER_COLOUR_RED_LED_MS = 5;

bool isButtonPressed()
{
    return PIND & (1 << PD2);
}

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

void turnOnAmberLed()
{
    turnOnGreenLed();
    _delay_ms(AMBER_COLOUR_GREEN_LED_MS);
    turnOnRedLed();
    _delay_ms(AMBER_COLOUR_RED_LED_MS);
}

void initialisation(void)
{
    cli();

    DDRA |= 1 << PA0;
    DDRA |= 1 << PA1;
    DDRD &= ~(1 << PD2);

    EIMSK |= (1 << INT0);

    EICRA |= 1 << ISC00;
    EICRA &= ~(1 << ISC01);

    sei();
}

enum class ButtonState
{
    PRESSED_ONCE,
    PRESSED_TWICE,
    PRESSED_THRICE,
    RELEASED_INITIAL_STATE,
    RELEASED_ONCE,
    RELEASED_TWICE,
};

enum class LedState
{
    GREEN,
    RED,
    AMBER,
    OFF
};

volatile ButtonState button = ButtonState::PRESSED_ONCE;
volatile LedState led = LedState::RED;

ISR(INT0_vect)
{

    bool lecture = isButtonPressed();

    _delay_ms(10);

    volatile bool pressed = !(isButtonPressed() && lecture);

    switch (button)
    {
    case ButtonState::RELEASED_INITIAL_STATE:
        led = LedState::RED;

        if (pressed)
            button = ButtonState::PRESSED_ONCE;

        break;

    case ButtonState::PRESSED_ONCE:
        led = LedState::AMBER;

        if (!pressed)
            button = ButtonState::RELEASED_ONCE;

        break;

    case ButtonState::RELEASED_ONCE:
        led = LedState::GREEN;

        if (pressed)
            button = ButtonState::PRESSED_TWICE;

        break;

    case ButtonState::PRESSED_TWICE:
        led = LedState::RED;

        if (!pressed)
            button = ButtonState::RELEASED_TWICE;

        break;

    case ButtonState::RELEASED_TWICE:
        led = LedState::OFF;

        if (pressed)
            button = ButtonState::PRESSED_THRICE;

        break;

    case ButtonState::PRESSED_THRICE:
        led = LedState::GREEN;

        if (!pressed)
            button = ButtonState::RELEASED_INITIAL_STATE;

        break;
    }

    EIFR |= (1 << INTF0);
}

void ledLogic()
{
    switch (led)
    {
    case LedState::GREEN:
        turnOnGreenLed();

        break;

    case LedState::RED:
        turnOnRedLed();

        break;

    case LedState::AMBER:
        turnOnAmberLed();

        break;

    case LedState::OFF:
        turnOffLed();

        break;
    }
}

int main()
{
    initialisation();

    while (true)
    {
        ledLogic();
    }
}

/*

EICRA        // mode
    EIMSK    // activer
        EIFR // regle les rebonds

    // TCNT1 : 16 bits, valeur du compteur
    // OCR1A : 16 bits, comparateur
    // OCR1B : 16 bits, comparateur
    // TCCR1A : 0,1
    //  WG 0/1
    //  "" A/B
    // TCCR1B : 2,3
    // pwm: tableau page 130
*/