#include "LineSensor.h"
LineSensor::LineSensor(Connection& connection1,Connection& connection2,Connection& connection3,Connection& connection4,Connection& connection5)
{
    line1_ = &connection1;
   line1_->set(0);
    line2_ = &connection2;
   line2_->set(0);
    line3_ = &connection3;
   line3_->set(0);
    line4_ = &connection4;
   line4_->set(0);
    line5_ = &connection5;
   line5_->set(0);
}

uint8_t LineSensor::getLineDetection() {
    return (line1_->get() ) | (line2_->get() << SENSOR2) | (line3_->get() << SENSOR3) | (line4_->get() << SENSOR4) | (line5_->get() << SENSOR5);
}
 //NB:least significant bit is S1,most significant is S5
