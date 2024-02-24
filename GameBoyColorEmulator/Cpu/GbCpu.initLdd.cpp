//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initLdd() {
    this->opCodes[Instruction::LD_DE_nn] = [this]() {
        uint8_t lowByte = this->memory->read(this->registers.regPC++);
        uint8_t highByte = this->memory->read(this->registers.regPC++);

        this->registers.regD = highByte;
        this->registers.regE = lowByte;
    };

    this->opCodes[Instruction::LD_DE_A] = [this]() {
        uint16_t address = (this->registers.regD << 8) | this->registers.regE;
        this->memory->writeByte(address, this->registers.regA);
    };

    this->opCodes[Instruction::LD_D_n] = [this]() {
        this->registers.regD = this->memory->read(this->registers.regPC++);
    };

    this->opCodes[Instruction::LD_D_B] = [this]() {
        this->registers.regD = this->registers.regB;
    };

    this->opCodes[Instruction::LD_D_C] = [this]() {
        this->registers.regD = this->registers.regC;
    };

    this->opCodes[Instruction::LD_D_D] = []() {};

    this->opCodes[Instruction::LD_D_E] = [this]() {
        this->registers.regD = this->registers.regE;
    };

    this->opCodes[Instruction::LD_D_H] = [this]() {
        this->registers.regD = this->registers.regH;
    };

    this->opCodes[Instruction::LD_D_L] = [this]() {
        this->registers.regD = this->registers.regL;
    };

    this->opCodes[Instruction::LD_D_HLptr] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regD = this->memory->read(address);
    };

    this->opCodes[Instruction::LD_D_A] = [this]() {
        this->registers.regD = this->registers.regA;
    };


}

