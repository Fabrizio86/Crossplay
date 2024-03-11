//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"

void GbCpu::initLda()
{
    this->opCodes[Instruction::LD_A_BC] = [this]()
    {
        uint16_t bc = (this->registers.regB << 8) | this->registers.regC;
        this->registers.regA = this->memory->read(bc);
    };

    this->opCodes[Instruction::LD_A_DE] = [this]()
    {
        uint16_t de = (this->registers.regD << 8) | this->registers.regE;
        this->registers.regA = this->memory->read(de);
    };

    this->opCodes[Instruction::LD_A_HLI] = [this]()
    {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regA = this->memory->read(address);
        address++;
        this->registers.regH = (address >> 8) & 0xFF;
        this->registers.regL = address & 0xFF;
    };

    this->opCodes[Instruction::LD_HLD_A] = [this]()
    {
//opcode_ldd(Address(hl), a);


        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->memory->writeWord(address, this->registers.regA);
        address--;
        this->registers.regH = (address >> 8) & 0xFF;
        this->registers.regL = address & 0xFF;
    };

    this->opCodes[Instruction::LD_A_HLD] = [this]()
    {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regA = this->memory->read(address);

        // Decrement the HL register pair
        address--;
        this->registers.regH = (address >> 8) & 0xFF;
        this->registers.regL = address & 0xFF;
    };

    this->opCodes[Instruction::LD_A_n] = [this]()
    {
        this->registers.regA = this->memory->read(this->registers.regPC);
        this->registers.regPC++; // Increment PC by 1 byte
    };

    this->opCodes[Instruction::LD_A_B] = [this]()
    {
        this->registers.regA = this->registers.regB;
    };

    this->opCodes[Instruction::LD_A_C] = [this]()
    {
        this->registers.regA = this->registers.regC;
    };

    this->opCodes[Instruction::LD_A_D] = [this]()
    {
        this->registers.regA = this->registers.regD;
    };

    this->opCodes[Instruction::LD_A_E] = [this]()
    {
        this->registers.regA = this->registers.regE;
    };

    this->opCodes[Instruction::LD_A_H] = [this]()
    {
        this->registers.regA = this->registers.regH;
    };

    this->opCodes[Instruction::LD_A_L] = [this]()
    {
        this->registers.regA = this->registers.regL;
    };

    this->opCodes[Instruction::LD_A_HLptr] = [this]()
    {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regA = this->memory->read(hl);
    };

    this->opCodes[Instruction::LD_A_A] = []()
    {
    };

    this->opCodes[Instruction::LD_A_Cptr] = [this]()
    {
        uint16_t address = 0xFF00 + this->registers.regC;
        this->registers.regA = this->memory->read(address);
    };

    this->opCodes[Instruction::LD_A_nn] = [this]()
    {
        uint16_t address = this->memory->read(this->registers.regPC++) |
            (this->memory->read(this->registers.regPC++) << 8);

        this->registers.regA = this->memory->read(address);
    };

    this->opCodes[Instruction::DAA] = [this]()
    {
        uint8_t correction = 0x00;
        if (this->registers.regF.halfCarry || (!this->registers.regF.subtract && (this->registers.regA & 0x0F) > 9))
        {
            correction |= 0x06;
        }

        if (this->registers.regF.carry || (!this->registers.regF.subtract && this->registers.regA > 0x99))
        {
            correction |= 0x60;
        }

        this->registers.regA += (this->registers.regF.subtract ? -correction : correction);
        this->registers.regF.carry = this->registers.regA > 0xFF;
        this->registers.regF.zero = (this->registers.regA == 0);
        this->registers.regF.halfCarry = false;
    };
}
