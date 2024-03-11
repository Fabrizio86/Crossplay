//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initDec()
{
    this->opCodes[Instruction::DEC_SP] = [this]()
    {
        this->registers.regSP--;
    };

    this->opCodes[Instruction::DEC_A] = [this]()
    {
        this->registers.regA--;

        // Update flags
        // The Zero flag is set to true if the result is zero
        this->registers.regF.zero = (this->registers.regA == 0);
        // The Subtract flag is set to true for DEC commands
        this->registers.regF.subtract = true;
        // The Half Carry flag is set if there was no borrow from bit 4
        this->registers.regF.halfCarry = ((this->registers.regA & 0x0F) == 0x0F);
    };

    this->opCodes[Instruction::DEC_HLptr] = [this]()
    {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(hl);
        value--;
        this->memory->writeByte(hl, value);

        // Update flags
        // Set Zero flag if the result is Zero
        this->registers.regF.zero = (value == 0);
        // Set Subtract flag because decrement is a subtract operation
        this->registers.regF.subtract = true;
        // Set Half-Carry flag if there was a borrow from bit 4
        this->registers.regF.halfCarry = ((value & 0x0F) == 0x0F);
    };

    this->opCodes[Instruction::DEC_B] = [this]()
    {
        this->registers.regB--;
    };

    this->opCodes[Instruction::DEC_BC] = [this]()
    {
        uint16_t bc = (this->registers.regB << 8) | this->registers.regC;
        bc--;

        this->registers.regB = (bc >> 8) & 0xFF;
        this->registers.regC = bc & 0xFF;
    };

    this->opCodes[Instruction::DEC_C] = [this]()
    {
        this->registers.regC--;

        // Update flags
        // Zero flag is set if result is zero
        this->registers.regF.zero = (this->registers.regC == 0);
        // Subtract flag is set because decrement is a subtract operation
        this->registers.regF.subtract = true;
        // Half-Carry flag is set if there is a borrow from bit 4
        this->registers.regF.halfCarry = ((this->registers.regC & 0x0F) == 0x0F);
    };

    this->opCodes[Instruction::DEC_D] = [this]()
    {
        this->registers.regD--;

        // Update flags
        // The Zero flag is set to true if the result is zero
        this->registers.regF.zero = (this->registers.regD == 0);
        // The Subtract flag is set to true because a decrement operation just took place
        this->registers.regF.subtract = true;
        // The Half Carry flag is set if there was no borrow from the lower nibble (bits 0-3)
        this->registers.regF.halfCarry = ((this->registers.regD & 0x0F) == 0x0F);
    };

    this->opCodes[Instruction::DEC_DE] = [this]()
    {
        uint16_t de = (this->registers.regD << 8) | this->registers.regE;
        de--;
        this->registers.regD = (de >> 8) & 0xFF;
        this->registers.regE = de & 0xFF;
    };

    this->opCodes[Instruction::DEC_E] = [this]()
    {
        this->registers.regE--;

        // Update flags
        // The Zero flag is set to true if the result is zero
        this->registers.regF.zero = (this->registers.regE == 0);
        // The Subtract flag is set to true because a decrement operation just took place
        this->registers.regF.subtract = true;
        // The Half Carry flag is set if there's a borrow from bit 4
        this->registers.regF.halfCarry = ((this->registers.regE & 0x0F) == 0x0F);
    };

    this->opCodes[Instruction::DEC_H] = [this]()
    {
        this->registers.regH--;

        // Update flags
        // Zero flag becomes true if the result is zero
        this->registers.regF.zero = (this->registers.regH == 0);
        // Subtract flag must be set as a decrement operation results in subtraction
        this->registers.regF.subtract = true;
        // Half Carry flag becomes true if a borrow occurred from bit 4
        this->registers.regF.halfCarry = ((this->registers.regH & 0x0F) == 0x0F);
    };

    this->opCodes[Instruction::DEC_HL] = [this]()
    {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        hl--;
        this->registers.regH = (hl >> 8) & 0xFF;
        this->registers.regL = hl & 0xFF;
    };

    this->opCodes[Instruction::DEC_L] = [this]()
    {
        this->registers.regL--;

        // Update flags
        // The Zero flag is set to true if the result is zero
        this->registers.regF.zero = (this->registers.regL == 0);
        // The Subtract flag is set to true because a decrement operation just took place
        this->registers.regF.subtract = true;
        // The Half Carry flag is set if there was a borrow from the lower nibble (bits 0-3)
        this->registers.regF.halfCarry = ((this->registers.regL & 0x0F) == 0x0F);
    };
}
