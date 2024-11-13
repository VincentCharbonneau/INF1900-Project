#include <USART.h>
#include <Can.h>
#include <math.h>
#include <util/delay.h>
#include <avr/io.h>

#ifdef DEBUG
# define DEBUG_PRINT(x) Debugger::getInstance()->print(x)
#else
# define DEBUG_PRINT(x) do {} while (0) // code mort
#endif

// Définir la fonction f(x)
double f(double x, double a, double b, double c, double y)
{
    return a * pow(b, x) * pow(x, c) - y;
}

// Définir la dérivée de f(x)
double f_prime(double x, double a, double b, double c)
{
    return a * pow(b, x) * (c * pow(x, c - 1) + log(b) * pow(x, c));
}

// Méthode de Newton-Raphson pour trouver x
double newton_raphson(double a, double b, double c, double y, double x0, double tolerance)
{
    double x = x0;
    int max_iterations = 1500;
    for (int i = 0; i < max_iterations; ++i)
    {
        double fx = f(x, a, b, c, y);
        double fx_prime = f_prime(x, a, b, c);
        double delta_x = fx / fx_prime;
        x -= delta_x;
        if (fabs(delta_x) < tolerance)
        {
            break;
        }
    }
    return x;
}

int main()
{

    double a = 3.296802635869109e3, b = 9.936681317758140e-1, c = -7.568305389630117e-1; // Constantes a, b, et c
    double x0 = 1.0;                                                                     // Valeur initiale de x
    double tolerance = 1e-6;                                                       // Tolérance pour la convergence

    Connection a0('A', 0, false);

    while (true)
    {
        DEBUG_PRINT("--------------------");
        uint16_t lecture = Can::getInstance()->lecture(0);

        USART::getInstance()->print("lecture: ", lecture);

        double y = static_cast<double>(lecture);

        double x = newton_raphson(a, b, c, y, x0, tolerance);

        uint32_t distance = static_cast<uint32_t>(x);

        USART::getInstance()->print("distance (cm): ", distance);
        DEBUG_PRINT("--------------------");
        _delay_ms(3000);
    }

    return 0;
}