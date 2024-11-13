#include "IRReceiver.h"
#include "Connection.h"
#include "USART.h"

int main() {
    Connection sensor('A', 2, false);

    IRReceiver receiver(sensor);

    while(true) {
        receiver.readMessage();
    }

    return 0;
}