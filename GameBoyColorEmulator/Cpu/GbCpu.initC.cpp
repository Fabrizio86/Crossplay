//
// Created by Fabrizio Paino on 2024-02-24.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initC() {

    this->opCodes[Instruction::CPL] = [this]() {
        this->registers.regA = ~this->registers.regA;

        this->flags.subtract = true;
        this->flags.halfCarry = true;
    };

    this->opCodes[Instruction::CP_B] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regB;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regB);
    };

    this->opCodes[Instruction::CP_C] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regC;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regC);
    };

    this->opCodes[Instruction::CP_D] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regD;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regD);
    };

    this->opCodes[Instruction::CP_E] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regE;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regE);
    };

    this->opCodes[Instruction::CP_H] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regH;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regH);
    };

    this->opCodes[Instruction::CP_L] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regL;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regL);
    };

    this->opCodes[Instruction::CP_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint8_t valueHL = this->memory->read(hl);
        uint8_t result = this->registers.regA - valueHL;
        updateFlagsAfterSubtraction(result, this->registers.regA, valueHL);
    };

    this->opCodes[Instruction::CP_A] = [this]() {
        updateFlagsAfterSubtraction(0, this->registers.regA, this->registers.regA);
    };

    this->opCodes[Instruction::CALL_NZ_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (this->flags.zero) return;

        this->memory->writeWord(this->registers.regSP - 2, this->registers.regPC);
        this->registers.regSP -= 2;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::CALL_Z_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (!this->flags.zero) return;

        this->memory->writeWord(this->registers.regSP - 2, this->registers.regPC);
        this->registers.regSP -= 2;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::CALL_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        this->memory->writeWord(this->registers.regSP - 2, this->registers.regPC);
        this->registers.regSP -= 2;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::CALL_NC_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (this->flags.carry) return;

        this->memory->writeWord(this->registers.regSP - 2, this->registers.regPC);
        this->registers.regSP -= 2;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::CALL_C_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (!this->flags.carry) return;

        this->memory->writeWord(this->registers.regSP - 2, this->registers.regPC);
        this->registers.regSP -= 2;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::CP_n] = [this]() {
        uint8_t value = this->memory->read(this->registers.regPC);
        this->registers.regPC++;

        uint16_t temp = this->registers.regA - value;

        this->flags.zero = (temp & 0xFF) == 0;
        this->flags.subtract = true;
        this->flags.halfCarry = ((this->registers.regA & 0x0F) - (value & 0x0F)) < 0;
        this->flags.carry = temp > 0xFF;
    };


}