#include "Connection.h"
#include "IR_receive.h"
#include "IR_transmition.h"
#include <USART.h>

int main() {
    Connection del('A', 2, true);
    Connection signal('A', 3, false);

    IR_transmition transmitter(del);
    IR_receive receiver(signal);

    uint16_t data = 0b111111111111; //12 bits

    transmitter.sendData(data);
    uint16_t receivedData = receiver.irRead();

    if (data == receivedData) {
        USART::getInstance()->print("Test passed");
    }
    else {
        USART::getInstance()->print("Test failed");
    }
    USART::getInstance()->print("Data: ", receivedData);

    return 0;
}
