#include "Instruction.h"

Instruction::Instruction(const unsigned char instruction, const unsigned char operand) 
    : instruction_(instruction), operand_(operand) {}

const unsigned char Instruction::getInstruction(void) const {
    return instruction_;
}

const unsigned char Instruction::getOperand(void) const {
    return operand_;
}