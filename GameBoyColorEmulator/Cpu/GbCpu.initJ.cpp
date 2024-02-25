//
// Created by Fabrizio Paino on 2024-02-24.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void conditionalJump(uint16_t &regPC, bool condition, int8_t offset) {
    if (condition) {
        regPC += offset;
    } else {
        // Skip the offset byte
        regPC++;
    }
}

void GbCpu::initJ() {

    this->opCodes[Instruction::JR_NC_e] = [this]() {
        int8_t offset = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        conditionalJump(this->registers.regPC, !this->flags.carry, offset);
    };

    this->opCodes[Instruction::JR_NZ_n] = [this]() {
        int8_t offset = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        conditionalJump(this->registers.regPC, !this->flags.zero, offset);
    };

    this->opCodes[Instruction::JR_Z_n] = [this]() {
        int8_t offset = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        conditionalJump(this->registers.regPC, this->flags.zero, offset);
    };

    this->opCodes[Instruction::JR_NC_n] = [this]() {
        int8_t offset = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        conditionalJump(this->registers.regPC, !this->flags.carry, offset);
    };

    this->opCodes[Instruction::JR_C_n] = [this]() {
        int8_t offset = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        conditionalJump(this->registers.regPC, this->flags.carry, offset);
    };

    this->opCodes[Instruction::JP_NZ_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (this->flags.zero) return;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_Z_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (!this->flags.zero) return;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_NC_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (this->flags.carry) return;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_C_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (!this->flags.carry) return;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_HL] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regPC = address;
    };
}