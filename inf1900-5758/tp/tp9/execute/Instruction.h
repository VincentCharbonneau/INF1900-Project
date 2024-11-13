#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class Instruction {
    private:
    unsigned char instruction_;
    unsigned char operand_;

    public:
    Instruction(const unsigned char instruction, const unsigned char operand);
    ~Instruction() = default;

    const unsigned char getInstruction(void) const;
    const unsigned char getOperand(void) const;
};

#endif