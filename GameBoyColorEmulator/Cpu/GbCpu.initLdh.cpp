//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initLdh() {

    this->opCodes[Instruction::LD_HL_nn] = [this]() {
        this->registers.regL = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        this->registers.regH = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
    };

    this->opCodes[Instruction::LD_HLI_A] = [this]() {
        uint16_t HL = (this->registers.regH << 8) | this->registers.regL; // Combine H and L to form HL
        this->memory->writeByte(HL, this->registers.regA); // Write regA to memory at address HL
        HL++;
        this->registers.regH = (HL >> 8) & 0xFF; // Store the high byte of HL back in regH
        this->registers.regL = HL & 0xFF; // Store the low byte of HL back in regL
    };

    this->opCodes[Instruction::LD_H_n] = [this]() {
        this->registers.regH = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
    };

    this->opCodes[Instruction::LD_HL_n] = [this]() {
        uint16_t HL = (this->registers.regH << 8) | this->registers.regL;
        uint8_t n = this->memory->read(this->registers.regPC);
        this->memory->writeByte(HL, n);
        this->registers.regPC++;
    };

    this->opCodes[Instruction::LD_H_B] = [this]() {
        this->registers.regH = this->registers.regB;
    };

    this->opCodes[Instruction::LD_H_C] = [this]() {
        this->registers.regH = this->registers.regC;
    };

    this->opCodes[Instruction::LD_H_D] = [this]() {
        this->registers.regH = this->registers.regD;
    };

    this->opCodes[Instruction::LD_H_E] = [this]() {
        this->registers.regH = this->registers.regE;
    };

    this->opCodes[Instruction::LD_H_H] = [this]() {};

    this->opCodes[Instruction::LD_H_L] = [this]() {
        this->registers.regH = this->registers.regL;
    };

    this->opCodes[Instruction::LD_H_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regH = this->memory->read(hl);
    };

    this->opCodes[Instruction::LD_H_A] = [this]() {
        this->registers.regH = this->registers.regA;
    };

    this->opCodes[Instruction::LD_HLptr_B] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->memory->writeByte(hl, this->registers.regB);
    };

    this->opCodes[Instruction::LD_HLptr_C] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->memory->writeByte(hl, this->registers.regC);
    };

    this->opCodes[Instruction::LD_HLptr_D] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->memory->writeByte(hl, this->registers.regD);
    };

    this->opCodes[Instruction::LD_HLptr_E] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->memory->writeByte(hl, this->registers.regE);
    };

    this->opCodes[Instruction::LD_HLptr_H] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->memory->writeByte(hl, this->registers.regH);
    };

    this->opCodes[Instruction::LD_HLptr_L] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->memory->writeByte(hl, this->registers.regL);
    };

    this->opCodes[Instruction::LD_HLptr_A] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->memory->writeByte(hl, this->registers.regA);
    };

    this->opCodes[Instruction::LD_HL_SPd] = [this]() {
        int8_t value = this->memory->read(this->registers.regPC++);
        uint16_t result = this->registers.regSP + value;
        this->registers.regH = (result >> 8) & 0xFF;
        this->registers.regL = result & 0xFF;

        this->registers.regF.zero = false;
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = ((this->registers.regSP ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10;
        this->registers.regF.carry = ((this->registers.regSP ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100;
    };
}