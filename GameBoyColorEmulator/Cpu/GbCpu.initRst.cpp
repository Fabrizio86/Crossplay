//
// Created by Fabrizio Paino on 2024-02-23.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"

// Apply constant refactoring
constexpr uint16_t BYTE_SHIFT = 8;
constexpr uint16_t BYTE_MASK = 0xFF;

void GbCpu::pushToStack(uint8_t value){
    this->memory->writeByte(--this->registers.regSP, value);
}

void GbCpu::resetRegistersAndDisableInterrupts(uint16_t currentPc, uint16_t newPcValue) {
    // Push bytes on the stack
    pushToStack(static_cast<uint8_t>((currentPc >> BYTE_SHIFT) & BYTE_MASK));
    pushToStack(static_cast<uint8_t>(currentPc & BYTE_MASK));

    this->registers.regPC = newPcValue;
    this->interruptEnabled = false; // Disable interrupts
}

void GbCpu::initRst() {
    this->opCodes[Instruction::RST_00H] = [this]() {
        resetRegistersAndDisableInterrupts(this->registers.regPC, 0x00);
    };

    this->opCodes[Instruction::RST_08H] = [this]() {
        resetRegistersAndDisableInterrupts(this->registers.regPC, 0x08);
    };

    this->opCodes[Instruction::RST_10H] = [this]() {
        resetRegistersAndDisableInterrupts(this->registers.regPC, 0x10);
    };

    this->opCodes[Instruction::RST_18H] = [this]() {
        resetRegistersAndDisableInterrupts(this->registers.regPC, 0x18);
    };

    this->opCodes[Instruction::RST_20H] = [this]() {
        resetRegistersAndDisableInterrupts(this->registers.regPC, 0x20);
    };

    this->opCodes[Instruction::RST_28H] = [this]() {
        resetRegistersAndDisableInterrupts(this->registers.regPC, 0x28);
    };

    this->opCodes[Instruction::RST_30H] = [this]() {
        resetRegistersAndDisableInterrupts(this->registers.regPC, 0x30);
    };

    this->opCodes[Instruction::RST_38H] = [this]() {
        resetRegistersAndDisableInterrupts(this->registers.regPC, 0x38);
    };
}