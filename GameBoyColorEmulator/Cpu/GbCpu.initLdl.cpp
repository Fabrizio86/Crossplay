//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initLdl() {
    this->opCodes[Instruction::LD_L_B] = [this]() {
        this->registers.regL = this->registers.regB;
    };

    this->opCodes[Instruction::LD_L_C] = [this]() {
        this->registers.regL = this->registers.regC;
    };

    this->opCodes[Instruction::LD_L_D] = [this]() {
        this->registers.regL = this->registers.regD;
    };

    this->opCodes[Instruction::LD_L_E] = [this]() {
        this->registers.regL = this->registers.regE;
    };

    this->opCodes[Instruction::LD_L_H] = [this]() {
        this->registers.regL = this->registers.regH;
    };

    this->opCodes[Instruction::LD_L_L] = []() {};

    this->opCodes[Instruction::LD_L_HLptr] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regL = this->memory->read(address);
    };

    this->opCodes[Instruction::LD_L_A] = [this]() {
        this->registers.regL = this->registers.regA;
    };

    this->opCodes[Instruction::LD_L_n] = [this]() {
        uint8_t immediateValue = this->memory->read(this->registers.regPC++);
        this->registers.regL = immediateValue;
    };

    this->opCodes[Instruction::LD_nn_SP] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        this->memory->writeWord(address, this->registers.regSP);
    };

    this->opCodes[Instruction::LD_SP_nn] = [this]() {
        uint16_t sp = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        this->registers.regSP = sp;
    };

    this->opCodes[Instruction::LD_nn_A] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        this->memory->writeByte(address, this->registers.regA);
    };

    this->opCodes[Instruction::LD_SP_HL] = [this]() {
        this->registers.regSP = (this->registers.regH << 8) | this->registers.regL;
    };

    this->opCodes[Instruction::LDH_A_n] = [this]() {
        uint8_t n = this->memory->read(this->registers.regPC++);
        this->registers.regA = this->memory->read(0xFF00 + n);
    };

    this->opCodes[Instruction::LDH_n_A] = [this]() {
        uint8_t n = this->memory->read(this->registers.regPC++);
        this->memory->writeByte(0xFF00 + n, this->registers.regA);
    };
}