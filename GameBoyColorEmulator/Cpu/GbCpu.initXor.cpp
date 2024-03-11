//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"

void GbCpu::performXor(uint8_t value)
{
    ui8 reg = this->registers.regA;
    ui8 res = reg ^ value;

    this->registers.regF.zero = res == 0;
    this->registers.regF.subtract = false;
    this->registers.regF.carry = false;
    this->registers.regF.halfCarry = false;

    this->registers.regA = res;
}


void GbCpu::initXor()
{
    this->opCodes[Instruction::XOR_A] = [this]()
    {
        this->performXor(this->registers.regA);
    };

    this->opCodes[Instruction::XOR_B] = [this]()
    {
        this->performXor(this->registers.regB);
    };

    this->opCodes[Instruction::XOR_C] = [this]()
    {
        this->performXor(this->registers.regC);
    };

    this->opCodes[Instruction::XOR_D] = [this]()
    {
        this->performXor(this->registers.regD);
    };

    this->opCodes[Instruction::XOR_E] = [this]()
    {
        this->performXor(this->registers.regE);
    };

    this->opCodes[Instruction::XOR_H] = [this]()
    {
        this->performXor(this->registers.regH);
    };

    this->opCodes[Instruction::XOR_L] = [this]()
    {
        this->performXor(this->registers.regL);
    };

    this->opCodes[Instruction::XOR_HLptr] = [this]()
    {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->performXor(this->memory->read(hl));
    };

    this->opCodes[Instruction::OR_B] = [this]()
    {
        this->registers.regA |= this->registers.regB;
        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::OR_C] = [this]()
    {
        this->registers.regA |= this->registers.regC;
        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::OR_D] = [this]()
    {
        this->registers.regA |= this->registers.regD;
        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::OR_E] = [this]()
    {
        this->registers.regA |= this->registers.regE;
        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::OR_H] = [this]()
    {
        this->registers.regA |= this->registers.regH;
        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::OR_L] = [this]()
    {
        this->registers.regA |= this->registers.regL;
        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::OR_HLptr] = [this]()
    {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t memoryValue = this->memory->read(address);
        this->registers.regA |= memoryValue;

        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::OR_A] = [this]()
    {
        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::OR_n] = [this]()
    {
        uint8_t value = this->memory->read(this->registers.regPC++);
        this->registers.regA |= value;
        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::OR);
    };

    this->opCodes[Instruction::XOR_n] = [this]()
    {
        uint8_t n = this->memory->read(this->registers.regPC++);
        this->registers.regA ^= n;

        updateFlagsAfterLogicalOperation(this->registers.regA, LogicalOperation::XOR);
    };
}
