//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initInc() {

    this->opCodes[Instruction::INC_BC] = [this]() {
        uint16_t bc = (this->registers.regB << 8) | this->registers.regC;
        bc++;
        this->registers.regB = (bc >> 8) & 0xFF;
        this->registers.regC = bc & 0xFF;
    };

    this->opCodes[Instruction::INC_B] = [this]() {
        this->registers.regB++;

        // Update flags
        // Set the Zero flag if the result is zero
        this->registers.regF.zero = (this->registers.regB == 0);
        // Reset the Subtract flag - it's not affected after an increment operation
        this->registers.regF.subtract = false;
        // Set the Half Carry flag if there was a carry from bit 3
        this->registers.regF.halfCarry = ((this->registers.regB & 0x0F) == 0);
    };

    this->opCodes[Instruction::INC_C] = [this]() {
        this->registers.regC++;
        this->registers.regF.zero = (this->registers.regC == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = (this->registers.regC & 0x0F) == 0x00;
        this->registers.regPC++;
    };

    this->opCodes[Instruction::INC_DE] = [this]() {
        uint16_t de = (this->registers.regD << 8) | this->registers.regE;
        de++;
        this->registers.regD = (de >> 8) & 0xFF;
        this->registers.regE = de & 0xFF;
    };

    this->opCodes[Instruction::INC_D] = [this]() {
        this->registers.regD++;

        // Update flags
        // Set the Zero flag if the result is zero
        this->registers.regF.zero = (this->registers.regD == 0);
        // Reset the Subtract flag - not affected after an increment operation
        this->registers.regF.subtract = false;
        // Set the Half Carry flag if there was a carry from bit 3
        this->registers.regF.halfCarry = ((this->registers.regD & 0x0F) == 0);
    };

    this->opCodes[Instruction::INC_E] = [this]() {
        this->registers.regE++;

        // Update flags
        // Set Zero flag if the result is Zero
        this->registers.regF.zero = (this->registers.regE == 0);
        // Reset the Subtract flag - not affected by increment operation
        this->registers.regF.subtract = false;
        // Set Half-Carry flag if there's a carry from bit 3
        this->registers.regF.halfCarry = ((this->registers.regE & 0x0F) == 0);
    };

    this->opCodes[Instruction::INC_HL] = [this]() {
        uint16_t HL = (this->registers.regH << 8) | this->registers.regL;
        HL++; // Increment the combined HL register value

        // Assign the incremented values back to H and L
        this->registers.regH = (HL >> 8) & 0xFF; // High order byte (H)
        this->registers.regL = HL & 0xFF; // Low order byte (L)
    };

    this->opCodes[Instruction::INC_H] = [this]() {
        this->registers.regH++;

        // Update flags
        // Set the Zero flag if the result is zero
        this->registers.regF.zero = (this->registers.regH == 0);
        // The Subtract flag is reset since it is not affected by an increment operation.
        this->registers.regF.subtract = false;
        // Set the Half Carry flag if there was a carry from bit 3
        this->registers.regF.halfCarry = ((this->registers.regH & 0x0F) == 0);
    };

    this->opCodes[Instruction::INC_L] = [this]() {
        this->registers.regL++;

        // Update flags
        // Set Zero flag if the result is Zero
        this->registers.regF.zero = (this->registers.regL == 0);
        // Reset the Subtract flag - not affected by increment operation
        this->registers.regF.subtract = false;
        // Set Half-Carry flag if there's a carry from bit 3
        this->registers.regF.halfCarry = ((this->registers.regL & 0x0F) == 0);
    };

    this->opCodes[Instruction::INC_SP] = [this]() {
        this->registers.regSP++;
    };

    this->opCodes[Instruction::INC_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(hl);
        value++;
        this->memory->writeByte(hl, value);

        // Update flags
        // Set Zero flag if the result is Zero
        this->registers.regF.zero = (value == 0);
        // Reset the Subtract flag - not affected by increment operation
        this->registers.regF.subtract = false;
        // Set Half-Carry flag if there's a carry from bit 3
        this->registers.regF.halfCarry = ((value & 0x0F) == 0);
    };

    this->opCodes[Instruction::INC_A] = [this]() {
        this->registers.regA++;

        // Update flags
        // The Zero flag is set if the result is zero
        this->registers.regF.zero = (this->registers.regA == 0);
        // The Subtract flag is reset to false as it's not affected by an increment operation
        this->registers.regF.subtract = false;
        // The Half Carry flag is set if a carry from bit 3 occurred
        this->registers.regF.halfCarry = ((this->registers.regA & 0x0F) == 0);
    };
}