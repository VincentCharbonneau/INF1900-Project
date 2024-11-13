
#include <avr/io.h>
#include <util/delay.h>
#include "Connection.h"
#include "Can.h"
#include "USART.h"

#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

// const double a = 3296.90;
// const double b = 0.993668;
// const double c = -0.756831;

class DistanceSensor
{
    private:
        static DistanceSensor instance_;
        DistanceSensor();
        DistanceSensor(const DistanceSensor &) = delete;
        DistanceSensor operator=(const DistanceSensor &) = delete;
        uint8_t convertToDistance(uint8_t average);
        void init();
        
    public:
        static DistanceSensor *getInstance(void);
        ~DistanceSensor() = default;
        uint8_t getDistance();
};

#endif
