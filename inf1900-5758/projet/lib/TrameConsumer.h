#ifndef TRAME_CONSUMER
#define TRAME_CONSUMER

#include "stdint.h"

class TrameConsumer {
    public:
        virtual void consumeTrame(uint8_t trame) = 0;
};

#endif