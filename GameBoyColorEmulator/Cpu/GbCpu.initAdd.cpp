//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void GbCpu::initAdd() {
    this->opCodes[Instruction::ADD_A_B] = [this]() {
        uint8_t oldValueOfRegA = this->registers.regA;
        this->registers.regA += this->registers.regB;
        this->updateFlagsAfterAddition(this->registers.regA, oldValueOfRegA, this->registers.regB);
    };

    this->opCodes[Instruction::ADD_A_C] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        this->registers.regA += this->registers.regC;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regC);
    };

    this->opCodes[Instruction::ADD_A_D] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        this->registers.regA += this->registers.regD;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regD);
    };

    this->opCodes[Instruction::ADD_A_E] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        this->registers.regA += this->registers.regE;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regE);
    };

    this->opCodes[Instruction::ADD_A_H] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        this->registers.regA += this->registers.regH;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regH);
    };

    this->opCodes[Instruction::ADD_A_L] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        this->registers.regA += this->registers.regL;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regL);
    };

    this->opCodes[Instruction::ADD_A_HLptr] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t valueAtHL = this->memory->read(address);
        this->registers.regA += valueAtHL;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, valueAtHL);
    };

    this->opCodes[Instruction::ADD_A_A] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        this->registers.regA += this->registers.regA;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, oldRegA);
    };

    this->opCodes[Instruction::ADD_HL_BC] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint16_t bc = (this->registers.regB << 8) | this->registers.regC;

        uint32_t result = (uint32_t) hl + (uint32_t) bc;
        this->registers.regF.carry = (result > 0xFFFF);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = ((hl & 0x07FF) + (bc & 0x07FF)) > 0x07FF;
        this->registers.regH = (result >> 8) & 0xFF;
        this->registers.regL = result & 0xFF;
    };

    this->opCodes[Instruction::ADD_HL_DE] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint16_t de = (this->registers.regD << 8) | this->registers.regE;
        uint32_t result = (uint32_t) hl + (uint32_t) de;
        this->registers.regF.carry = ((result & 0x10000) != 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = ((hl & 0x07FF) + (de & 0x07FF)) & 0x0800;
        this->registers.regH = (result >> 8) & 0xFF;
        this->registers.regL = result & 0xFF;
    };

    this->opCodes[Instruction::ADD_HL_HL] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint32_t result = (uint32_t) hl * 2;
        this->registers.regF.carry = ((result & 0x10000) != 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = ((hl & 0x07FF) + (hl & 0x07FF)) & 0x0800;
        this->registers.regH = (result >> 8) & 0xFF;
        this->registers.regL = result & 0xFF;
    };

    this->opCodes[Instruction::ADD_HL_SP] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint16_t sp = this->registers.regSP;
        uint32_t result = (uint32_t) hl + (uint32_t) sp;
        this->registers.regF.carry = (result > 0xFFFF);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = ((hl & 0x07FF) + (sp & 0x07FF) > 0x07FF);
        this->registers.regH = (result >> 8) & 0xFF;
        this->registers.regL = result & 0xFF;
    };

    this->opCodes[Instruction::ADD_A_n] = [this]() {
        uint8_t value = this->memory->read(this->registers.regPC++);
        uint16_t temp = this->registers.regA + value;
        this->registers.regF.zero = (temp & 0xFF) == 0;
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = ((this->registers.regA ^ value ^ temp) & 0x10) != 0;
        this->registers.regF.carry = temp > 0xFF;
        this->registers.regA = temp & 0xFF;
    };

    this->opCodes[Instruction::ADD_SP_d] = [this]() {
        int8_t value = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        uint16_t result = this->registers.regSP + value;
        this->registers.regF.zero = false;
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = ((this->registers.regSP ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10;
        this->registers.regF.carry = ((this->registers.regSP ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100;
        this->registers.regSP = result & 0xFFFF;
    };

    this->opCodes[Instruction::ADC_A_B] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint8_t carry = this->registers.regF.carry;
        this->registers.regA += this->registers.regB + carry;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regB + carry);
    };

    this->opCodes[Instruction::ADC_A_C] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint8_t carry = this->registers.regF.carry;
        this->registers.regA += this->registers.regC + carry;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regC + carry);
    };

    this->opCodes[Instruction::ADC_A_D] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint8_t carry = this->registers.regF.carry;
        this->registers.regA += this->registers.regD + carry;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regD + carry);
    };

    this->opCodes[Instruction::ADC_A_E] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint8_t carry = this->registers.regF.carry;
        this->registers.regA += this->registers.regE + carry;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regE + carry);
    };

    this->opCodes[Instruction::ADC_A_H] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint8_t carry = this->registers.regF.carry;
        this->registers.regA += this->registers.regH + carry;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regH + carry);
    };

    this->opCodes[Instruction::ADC_A_L] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint8_t carry = this->registers.regF.carry;
        this->registers.regA += this->registers.regL + carry;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, this->registers.regL + carry);
    };

    this->opCodes[Instruction::ADC_A_HLptr] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint8_t carry = this->registers.regF.carry;
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t valueAtHL = this->memory->read(address);
        this->registers.regA += valueAtHL + carry;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, valueAtHL + carry);
    };

    this->opCodes[Instruction::ADC_A_A] = [this]() {
        uint8_t oldRegA = this->registers.regA;
        uint8_t carry = this->registers.regF.carry;
        this->registers.regA += this->registers.regA + carry;
        this->updateFlagsAfterAddition(this->registers.regA, oldRegA, oldRegA + carry);
    };

    this->opCodes[Instruction::ADC_A_n] = [this]() {
        uint8_t oldValue = this->registers.regA;
        uint8_t immediateValue = this->memory->read(this->registers.regPC++);
        uint8_t carryValue = this->registers.regF.carry ? 1 : 0;

        uint16_t resultWithoutCarry = oldValue + immediateValue;
        uint8_t result = resultWithoutCarry + carryValue;

        // Update flags
        this->updateFlagsAfterAddition(result, oldValue, immediateValue);

        // If there was a carry from the carry addition, set the carry flag
        if (resultWithoutCarry > UINT8_MAX - carryValue) {
            this->registers.regF.carry = true;
        }

        // Update regA
        this->registers.regA = result;
    };

}