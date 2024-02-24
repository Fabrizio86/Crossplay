//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"

void GbCpu::performBitwiseXorAndUpdateFlags(uint8_t &registerA, uint8_t value) {
    registerA ^= value;
    updateFlagsAfterLogicalOperation(registerA, false);
}

void GbCpu::initXor() {
    this->opCodes[Instruction::XOR_B] = [this]() {
        performBitwiseXorAndUpdateFlags(this->registers.regA, this->registers.regB);
    };

    this->opCodes[Instruction::XOR_C] = [this]() {
        performBitwiseXorAndUpdateFlags(this->registers.regA, this->registers.regC);
    };

    this->opCodes[Instruction::XOR_D] = [this]() {
        performBitwiseXorAndUpdateFlags(this->registers.regA, this->registers.regD);
    };

    this->opCodes[Instruction::XOR_E] = [this]() {
        performBitwiseXorAndUpdateFlags(this->registers.regA, this->registers.regE);
    };

    this->opCodes[Instruction::XOR_H] = [this]() {
        performBitwiseXorAndUpdateFlags(this->registers.regA, this->registers.regH);
    };

    this->opCodes[Instruction::XOR_L] = [this]() {
        performBitwiseXorAndUpdateFlags(this->registers.regA, this->registers.regL);
    };

    this->opCodes[Instruction::XOR_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        performBitwiseXorAndUpdateFlags(this->registers.regA, this->memory->read(hl));
    };

    this->opCodes[Instruction::XOR_A] = [this]() {
        performBitwiseXorAndUpdateFlags(this->registers.regA, 0);
    };

    this->opCodes[Instruction::OR_B] = [this]() {
        this->registers.regA |= this->registers.regB;
        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::OR_C] = [this]() {
        this->registers.regA |= this->registers.regC;
        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::OR_D] = [this]() {
        this->registers.regA |= this->registers.regD;
        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::OR_E] = [this]() {
        this->registers.regA |= this->registers.regE;
        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::OR_H] = [this]() {
        this->registers.regA |= this->registers.regH;
        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::OR_L] = [this]() {
        this->registers.regA |= this->registers.regL;
        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::OR_HLptr] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t memoryValue = this->memory->read(address);
        this->registers.regA |= memoryValue;

        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::OR_A] = [this]() {
        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::OR_n] = [this]() {
        uint8_t value = this->memory->read(this->registers.regPC++);
        this->registers.regA |= value;
        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

    this->opCodes[Instruction::XOR_n] = [this]() {
        uint8_t n = this->memory->read(this->registers.regPC++);
        this->registers.regA ^= n;

        updateFlagsAfterLogicalOperation(this->registers.regA, false);
    };

}