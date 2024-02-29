//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initLdc() {

    this->opCodes[Instruction::LD_C_n] = [this]() {
        uint8_t value = this->memory->read(this->registers.regPC);
        this->registers.regC = value;
    };

    this->opCodes[Instruction::LD_C_B] = [this]() {
        this->registers.regC = this->registers.regB;
    };

    this->opCodes[Instruction::LD_C_C] = [this]() {};

    this->opCodes[Instruction::LD_C_D] = [this]() {
        this->registers.regC = this->registers.regD;
    };

    this->opCodes[Instruction::LD_C_E] = [this]() {
        this->registers.regC = this->registers.regE;
    };

    this->opCodes[Instruction::LD_C_H] = [this]() {
        this->registers.regC = this->registers.regH;
    };

    this->opCodes[Instruction::LD_C_L] = [this]() {
        this->registers.regC = this->registers.regL;
    };

    this->opCodes[Instruction::LD_C_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regC = this->memory->read(hl);
    };

    this->opCodes[Instruction::LD_C_A] = [this]() {
        this->registers.regC = this->registers.regA;
    };

    this->opCodes[Instruction::LD_Cptr_A] = [this]() {
        uint8_t address = 0xFF00 + this->registers.regC;
        this->memory->writeByte(address, this->registers.regA);
    };

}