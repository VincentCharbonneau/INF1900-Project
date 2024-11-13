#include "IRReceiver.h"

IRReceiver::IRReceiver(Connection& signal, TrameConsumer& consumer)
    : _consumer(consumer) {
    signal_ = &signal;
}

bool IRReceiver::getSignal() {
    return !signal_->get();
}

// startbit = 6 signal
// one = 3 signal
// zero = 2 signal

void IRReceiver::readMessage() {
    signalCount = 0;
    if(!getSignal()) return;

    _delay_us(100);

    do {
        signalCount++;
        _delay_us(600);
    } while(getSignal());

    if(signalCount >= 5) {
        data = 0b00000000;
        workingBit = 0;
        startBitSeen = true;
    }

    if(!startBitSeen) return;

    if(signalCount == 2) {
        data &= ~(1 << workingBit);
        workingBit++;
    }
    else if(signalCount == 3) {
        data |= (1 << workingBit);
        workingBit++;
    }

    if(workingBit == 8) {
        saveData(data);
        startBitSeen = false;
    }
}

void IRReceiver::saveData(uint8_t data) {
    _consumer.consumeTrame(data);
}