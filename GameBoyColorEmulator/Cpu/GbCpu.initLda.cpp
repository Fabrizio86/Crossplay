//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"

void GbCpu::initLda() {

    this->opCodes[Instruction::LD_A_BC] = [this]() {
        uint16_t bc = (this->registers.regB << 8) | this->registers.regC;
        this->registers.regA = this->memory->read(bc);
    };

    this->opCodes[Instruction::LD_A_DE] = [this]() {
        uint16_t de = (this->registers.regD << 8) | this->registers.regE;
        this->registers.regA = this->memory->read(de);
    };

    this->opCodes[Instruction::LD_A_HLI] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regA = this->memory->read(address);
        address++;
        this->registers.regH = (address >> 8) & 0xFF;
        this->registers.regL = address & 0xFF;
    };

    this->opCodes[Instruction::LD_HLD_A] = [this]() {
        uint16_t h = static_cast<uint16_t>(this->registers.regH);
        uint16_t l = static_cast<uint16_t>(this->registers.regL);
        uint16_t address = (h << 8) | l;

        this->memory->writeByte(address, this->registers.regA);

        if (this->registers.regL == 0x00) {
            this->registers.regH--;
            this->registers.regL = 0xFF;
        } else {
            this->registers.regL--;
        }
    };

    this->opCodes[Instruction::LD_A_HLD] = [this]() {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL; // Combine H and L to form the HL pair
        this->registers.regA = this->memory->read(address); // Load the byte from the memory location pointed to by HL into A

        // Decrement the HL register pair
        address--;
        this->registers.regH = (address >> 8) & 0xFF; // Get the higher byte and store it in H
        this->registers.regL = address & 0xFF; // Get the lower byte and store it in L
    };

    this->opCodes[Instruction::LD_A_n] = [this]() {
        this->registers.regA = this->memory->read(this->registers.regPC);
        this->registers.regPC++; // Increment PC by 1 byte
    };

    this->opCodes[Instruction::LD_A_B] = [this]() {
        this->registers.regA = this->registers.regB;
    };

    this->opCodes[Instruction::LD_A_C] = [this]() {
        this->registers.regA = this->registers.regC;
    };

    this->opCodes[Instruction::LD_A_D] = [this]() {
        this->registers.regA = this->registers.regD;
    };

    this->opCodes[Instruction::LD_A_E] = [this]() {
        this->registers.regA = this->registers.regE;
    };

    this->opCodes[Instruction::LD_A_H] = [this]() {
        this->registers.regA = this->registers.regH;
    };

    this->opCodes[Instruction::LD_A_L] = [this]() {
        this->registers.regA = this->registers.regL;
    };

    this->opCodes[Instruction::LD_A_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regA = this->memory->read(hl);
    };

    this->opCodes[Instruction::LD_A_A] = []() {};

    this->opCodes[Instruction::LD_A_Cptr] = [this]() {
        uint16_t address = 0xFF00 + this->registers.regC;
        this->registers.regA = this->memory->read(address);
    };

    this->opCodes[Instruction::LD_A_nn] = [this]() {
        uint16_t address = this->memory->read(this->registers.regPC++) |
                           (this->memory->read(this->registers.regPC++) << 8);

        this->registers.regA = this->memory->read(address);
    };

    this->opCodes[Instruction::DAA] = [this]() {
        uint8_t correction = 0x00;
        if (this->flags.halfCarry || (!this->flags.subtract && (this->registers.regA & 0x0F) > 9)) {
            correction |= 0x06;
        }

        if (this->flags.carry || (!this->flags.subtract && this->registers.regA > 0x99)) {
            correction |= 0x60;
        }

        this->registers.regA += (this->flags.subtract ? -correction : correction);
        this->flags.carry = this->registers.regA > 0xFF;
        this->flags.zero = (this->registers.regA == 0);
        this->flags.halfCarry = false;
    };
}