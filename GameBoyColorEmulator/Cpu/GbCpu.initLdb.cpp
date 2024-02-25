//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initLdb() {
    this->opCodes[Instruction::LD_BC_nn] = [this]() {
        this->registers.regC = this->memory->read(this->registers.regPC++);
        this->registers.regB = this->memory->read(this->registers.regPC++);
    };

    this->opCodes[Instruction::LD_BC_A] = [this]() {
        uint16_t BC = (this->registers.regB << 8) | this->registers.regC; // Combine B and C to form BC
        this->memory->writeByte(BC, this->registers.regA);
    };

    this->opCodes[Instruction::LD_B_n] = [this]() {
        this->registers.regB  = this->memory->read(this->registers.regPC); // Fetch the next byte
        ++this->registers.regPC; // Increment the program counter
    };

    this->opCodes[Instruction::LD_B_B] = []() { };

    this->opCodes[Instruction::LD_B_C] = [this]() {
        this->registers.regB = this->registers.regC;
    };

    this->opCodes[Instruction::LD_B_D] = [this]() {
        this->registers.regB = this->registers.regD;
    };

    this->opCodes[Instruction::LD_B_E] = [this]() {
        this->registers.regB = this->registers.regE;
    };

    this->opCodes[Instruction::LD_B_H] = [this]() {
        this->registers.regB = this->registers.regH;
    };

    this->opCodes[Instruction::LD_B_L] = [this]() {
        this->registers.regB = this->registers.regL;
    };

    this->opCodes[Instruction::LD_B_HLptr] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regB = this->memory->read(address);
    };

    this->opCodes[Instruction::LD_B_A] = [this]() {
        this->registers.regB = this->registers.regA;
    };
}