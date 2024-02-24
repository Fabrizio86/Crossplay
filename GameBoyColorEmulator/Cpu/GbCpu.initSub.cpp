//
// Created by Fabrizio Paino on 2024-02-24.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::performSubtraction(uint8_t operandB, bool includeCarry) {
    uint8_t operandA = this->registers.regA;
    uint8_t temp = operandA - operandB;
    if (includeCarry) {
        temp -= this->flags.carry;
    }

    this->updateFlagsAfterSubtraction(temp, operandA, operandB);
    this->registers.regA = temp;
}

void GbCpu::initSub() {
    this->opCodes[Instruction::CCF] = [this]() {
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = !this->flags.carry;
    };

    this->opCodes[Instruction::HALT] = [this]() {
        this->halted = true;
    };

    this->opCodes[Instruction::SUB_B] = [this]() { performSubtraction(this->registers.regB, false); };
    this->opCodes[Instruction::SUB_C] = [this]() { performSubtraction(this->registers.regC, false); };
    this->opCodes[Instruction::SUB_D] = [this]() { performSubtraction(this->registers.regD, false); };
    this->opCodes[Instruction::SUB_E] = [this]() { performSubtraction(this->registers.regE, false); };
    this->opCodes[Instruction::SUB_H] = [this]() { performSubtraction(this->registers.regH, false); };
    this->opCodes[Instruction::SUB_L] = [this]() { performSubtraction(this->registers.regL, false); };

    this->opCodes[Instruction::SUB_HLptr] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t valueAtAddress = this->memory->read(address);
        performSubtraction(valueAtAddress, false);
    };

    this->opCodes[Instruction::SUB_A] = [this]() {
        this->registers.regA = 0;
        this->flags.zero = true;
        this->flags.subtract = true;
    };

    this->opCodes[Instruction::SUB_n] = [this]() {
        uint8_t immediateVal = this->memory->read(this->registers.regPC);
        performSubtraction(immediateVal, false);
    };

    this->opCodes[Instruction::SBC_A_n] = [this]() {
        uint8_t immediateVal = this->memory->read(this->registers.regPC);
        performSubtraction(immediateVal, true);
    };

    this->opCodes[Instruction::SBC_A_B] = [this]() { performSubtraction(this->registers.regB, true); };
    this->opCodes[Instruction::SBC_A_C] = [this]() { performSubtraction(this->registers.regC, true); };
    this->opCodes[Instruction::SBC_A_D] = [this]() { performSubtraction(this->registers.regD, true); };
    this->opCodes[Instruction::SBC_A_E] = [this]() { performSubtraction(this->registers.regE, true); };
    this->opCodes[Instruction::SBC_A_H] = [this]() { performSubtraction(this->registers.regH, true); };
    this->opCodes[Instruction::SBC_A_L] = [this]() { performSubtraction(this->registers.regL, true); };

    this->opCodes[Instruction::SBC_A_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(hl);
        performSubtraction(value, true);
    };

    this->opCodes[Instruction::SBC_A_A] = [this]() {
        performSubtraction(this->registers.regA, true);
    };

    this->opCodes[Instruction::SCF] = [this]() {
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = true;
    };

}