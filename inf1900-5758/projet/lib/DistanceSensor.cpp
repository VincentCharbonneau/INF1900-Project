#include "DistanceSensor.h"
#include <math.h>

DistanceSensor DistanceSensor::instance_;

DistanceSensor *DistanceSensor::getInstance()
{
    return &instance_;
}

DistanceSensor::DistanceSensor()
{
    init();
}

void DistanceSensor::init()
{
    Connection a0('A', 0, false);
}

uint8_t DistanceSensor::convertToDistance(uint8_t output)
{
    uint16_t distance = 511/(output - 12);
    uint8_t distance8 = static_cast<uint8_t>(distance);
    return distance8;
}

uint8_t DistanceSensor::getDistance()
{
    uint16_t lecture = Can::getInstance()->lecture(0);
    uint8_t output = lecture >> 2;
    uint8_t distance = convertToDistance(output);
    
    return distance;
}