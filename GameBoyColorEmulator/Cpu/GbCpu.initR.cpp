//
// Created by Fabrizio Paino on 2024-02-24.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

uint16_t GbCpu::popFromStack() {
    uint8_t low = this->memory->read(this->registers.regSP++);
    uint8_t high = this->memory->read(this->registers.regSP++);
    return (high << 8) | low;
}

void GbCpu::initR() {
    this->opCodes[Instruction::RLCA] = [this]() {
        this->flags.carry = (this->registers.regA & 0x80) != 0;
        this->registers.regA = (this->registers.regA << 1) | (this->flags.carry ? 1 : 0);
        this->flags.zero = (this->registers.regA == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
    };

    this->opCodes[Instruction::RRCA] = [this]() {
        this->flags.carry = (this->registers.regA & 0x01) != 0;
        this->registers.regA = (this->registers.regA >> 1) | (this->flags.carry ? 0x80 : 0);
        this->flags.zero = (this->registers.regA == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
    };

    this->opCodes[Instruction::RLA] = [this]() {
        bool prevCarry = this->flags.carry;
        this->flags.carry = (this->registers.regA & 0x80) != 0;
        this->registers.regA = ((this->registers.regA << 1) & 0xFF) | (prevCarry ? 1 : 0);
        this->flags.zero = (this->registers.regA == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
    };

    this->opCodes[Instruction::RRA] = [this]() {
        bool prevCarry = this->flags.carry;
        this->flags.carry = (this->registers.regA & 0x01) != 0;
        this->registers.regA = (this->registers.regA >> 1) | (prevCarry ? 0x80 : 0);
        this->flags.zero = (this->registers.regA == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
    };

    this->opCodes[Instruction::RET_NZ] = [this]() {
        if (this->flags.zero) return;

        uint16_t return_address = this->popFromStack();
        this->registers.regPC = return_address;
    };

    this->opCodes[Instruction::RET_Z] = [this]() {
        if (!this->flags.zero) return;

        uint16_t return_address = this->popFromStack();
        this->registers.regPC = return_address;
    };

    this->opCodes[Instruction::RET] = [this]() {
        uint16_t return_address = this->popFromStack();
        this->registers.regPC = return_address;
    };

    this->opCodes[Instruction::RET_NC] = [this]() {
        if (this->flags.carry) return;

        uint16_t return_address = this->popFromStack();
        this->registers.regPC = return_address;
    };

    this->opCodes[Instruction::RET_C] = [this]() {
        if (!this->flags.carry) return;

        uint16_t return_address = this->popFromStack();
        this->registers.regPC = return_address;
    };

    this->opCodes[Instruction::RETI] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regSP);
        this->registers.regSP += 2;
        this->registers.regPC = address;
        this->interruptEnabled = true;
    };

}