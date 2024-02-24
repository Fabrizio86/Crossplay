//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initLde() {
    this->opCodes[Instruction::LD_E_n] = [this]() {
        this->registers.regE = this->memory->read(this->registers.regPC++);
    };

    this->opCodes[Instruction::LD_E_B] = [this]() {
        this->registers.regE = this->registers.regB;
    };

    this->opCodes[Instruction::LD_E_C] = [this]() {
        this->registers.regE = this->registers.regC;
    };

    this->opCodes[Instruction::LD_E_D] = [this]() {
        this->registers.regE = this->registers.regD;
    };

    this->opCodes[Instruction::LD_E_E] = []() { };

    this->opCodes[Instruction::LD_E_H] = [this]() {
        this->registers.regE = this->registers.regH;
    };

    this->opCodes[Instruction::LD_E_L] = [this]() {
        this->registers.regE = this->registers.regL;
    };

    this->opCodes[Instruction::LD_E_HLptr] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regE = this->memory->read(address);
    };

    this->opCodes[Instruction::LD_E_A] = [this]() {
        this->registers.regE = this->registers.regA;
    };
}