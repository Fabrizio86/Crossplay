//
// Created by Fabrizio Paino on 2024-02-24.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initAnd() {
    this->opCodes[Instruction::AND_B] = [this]() {
        this->registers.regA &= this->registers.regB;
        this->updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::AND);
    };

    this->opCodes[Instruction::AND_C] = [this]() {
        this->registers.regA &= this->registers.regC;
        this->updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::AND);
    };

    this->opCodes[Instruction::AND_D] = [this]() {
        this->registers.regA &= this->registers.regD;
        this->updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::AND);
    };

    this->opCodes[Instruction::AND_E] = [this]() {
        this->registers.regA &= this->registers.regE;
        this->updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::AND);
    };

    this->opCodes[Instruction::AND_H] = [this]() {
        this->registers.regA &= this->registers.regH;
        this->updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::AND);
    };

    this->opCodes[Instruction::AND_L] = [this]() {
        this->registers.regA &= this->registers.regL;
        this->updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::AND);
    };

    this->opCodes[Instruction::AND_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regA &= this->memory->read(hl);
        this->updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::AND);
    };

    this->opCodes[Instruction::AND_n] = [this]() {
        uint8_t value = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        this->registers.regA &= value;
        this->updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::AND);
    };
}