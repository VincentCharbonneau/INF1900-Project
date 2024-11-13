#include <memoire_24.h>
#include <USART.h>
#include <util/delay.h>
#include <Instruction.h>
#include <avr/interrupt.h>
#include <Timer.h>
#include <Connection.h>
#include <Led.h>
#include <Pwm.h>
#include <Sound.h>

const uint8_t ANGULAR_CORRECTION = 10;


volatile bool gBlockLoop = false;

ISR(TIMER1_COMPA_vect) {
    gBlockLoop = false;
}

Instruction getInstruction(Memoire24CXXX& mem, uint16_t address) {
    uint8_t instructionByte;
    mem.lecture(address, &instructionByte);
    address++;

    uint8_t operandByte;
    mem.lecture(address, &operandByte);

    return Instruction(instructionByte, operandByte);
}


int main() {
    Connection ledConnection1 = Connection('A', 0, true);
    Connection ledConnection2 = Connection('A', 1, true);
    Led led = Led(ledConnection1, ledConnection2);
    led.setColor(LedColor::COLOR_OFF);

    Connection pwmOutputA('B', 4, true);
    Connection pwmOutputB('B', 3, true);
    Connection pwmDirA('B', 2, true);
    Connection pwmDirB('B', 5, true);
    Pwm pwm(pwmDirA, pwmDirB);

    Connection piezoGround('D', 6, true);
    Connection piezoSignal('D', 7, true);
    Sound snd(piezoGround, piezoSignal);

    sei();

    Memoire24CXXX mem;

    uint16_t address = 0x0000;

    bool programBegan = false;
    bool programEnd = false;
    uint16_t loopAddress = 0x0000;
    uint16_t loopCount = 0;

    while(!programEnd) { 
        if(gBlockLoop) {
            continue;
        }

        address += 0x0002;
        Instruction instruction = getInstruction(mem, address);

        if (instruction.getInstruction() == 0x01) { // dbt
            USART::getInstance()->print("dbt", instruction.getOperand());
            programBegan = true;
        }

        if(!programBegan) {
            continue;
        }

        if(instruction.getInstruction() == 0xFF) { // fin
            USART::getInstance()->print("fin", instruction.getOperand());
            programEnd = true;
        }

        if(instruction.getInstruction() == 0x02) { // att
            USART::getInstance()->print("att", instruction.getOperand());
            
            uint16_t duration = instruction.getOperand() * 25;

            Timer::getInstance()->clearTimer();
            Timer::getInstance()->setDurationCompareA(duration);

            gBlockLoop = true;
        }

        if(instruction.getInstruction() == 0x44) { // dal
            USART::getInstance()->print("dal", instruction.getOperand());
            if(instruction.getOperand() == 0x01) {
                led.setColor(LedColor::COLOR_GREEN);
            }
            if(instruction.getOperand() == 0x02) {
                led.setColor(LedColor::COLOR_RED);
            }
        }

        if(instruction.getInstruction() == 0x45) { // det
            USART::getInstance()->print("det", instruction.getOperand());
            led.setColor(LedColor::COLOR_OFF);
        }

        if(instruction.getInstruction() == 0x62) { // mav
            USART::getInstance()->print("mav", instruction.getOperand());

            uint16_t percent = instruction.getOperand() * 100 / 255;
            
            pwm.setDirectionA(false);
            pwm.setDirectionB(false);

            pwm.setPercentA(percent);
            pwm.setPercentB(percent - ANGULAR_CORRECTION);
        }

        if(instruction.getInstruction() == 0x63) { // mre
            USART::getInstance()->print("mar", instruction.getOperand());

            uint16_t percent = instruction.getOperand() * 100 / 255;
            
            pwm.setDirectionA(true);
            pwm.setDirectionB(true);

            pwm.setPercentA(percent);
            pwm.setPercentB(percent);
        }

        if(instruction.getInstruction() == 0x60 ||
            instruction.getInstruction() == 0x61) { // mar
            USART::getInstance()->print("mar", instruction.getOperand());
            
            pwm.setPercentA(0);
            pwm.setPercentB(0);
        }

        if (instruction.getInstruction() == 0x64) { // trd
            USART::getInstance()->print("trd", instruction.getOperand());

            pwm.setDirectionA(true);
            pwm.setDirectionB(false);
            pwm.setPercentA(100);
            pwm.setPercentB(100 - ANGULAR_CORRECTION);
            _delay_ms(550);
            pwm.setPercentA(0);
            pwm.setPercentB(0);
            _delay_ms(500);
        }

        if (instruction.getInstruction() == 0x65) { // trg
            USART::getInstance()->print("trg", instruction.getOperand());

            pwm.setDirectionA(false);
            pwm.setDirectionB(true);
            pwm.setPercentA(100);
            pwm.setPercentB(100 - ANGULAR_CORRECTION);
            _delay_ms(550);
            pwm.setPercentA(0);
            pwm.setPercentB(0);
            _delay_ms(500);
        }

        if (instruction.getInstruction() == 0xC0) { // dbc
            USART::getInstance()->print("dbc", instruction.getOperand());

            loopAddress = address;
            loopCount = instruction.getOperand();
        }

        if (instruction.getInstruction() == 0xC1) { // fbc
            USART::getInstance()->print("fbc", instruction.getOperand());

            if(loopCount > 0) {
                loopCount--;
                address = loopAddress;
            }
        }

        if (instruction.getInstruction() == 0x48) { // sgo
            USART::getInstance()->print("sgo", instruction.getOperand());
            snd.playNote(instruction.getOperand());
        }

        if (instruction.getInstruction() == 0x09) { // sar
            USART::getInstance()->print("sfl", instruction.getOperand());
            snd.stop();
        }
    }

    return 0;
}