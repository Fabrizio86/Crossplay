//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"
#include "../StopCpuException.h"

#include <iostream>

int i = 0;

void GbCpu::exec()
{
    if (this->halted)
        return;

    if (this->stopped)
        throw StopCPUException();

    if (this->interruptEnabled && this->interruptController->isInterruptRequests())
    {
        std::cout << "in the interrupt" << std::endl;
        this->saveState();
        this->isr->handleInterrupt();
        this->restoreState();
    }

    opcode = static_cast<Instruction>(this->memory->read(this->registers.regPC++));

    opCodes[opcode]();
        printf("%d - SP: %x PC: %x Opcode: %x, Flag: %d RegA: %x RegH: %x\n",
               i,
               this->registers.regSP,
               registers.regPC,
               opcode,
               this->registers.regF.zero,
               this->registers.regA,
               this->registers.regH);
}

void GbCpu::reset()
{
    // Initialize flags
    this->registers.regF.zero =
        this->registers.regF.subtract =
        this->registers.regF.halfCarry =
        this->registers.regF.carry = false;

    this->registers.reset();
    this->stopped = false;
    this->halted = false;

    std::cout << std::hex << this->registers.regPC << std::endl;
}

void GbCpu::saveState()
{
    this->savedRegisters = this->registers;
}

void GbCpu::restoreState()
{
    this->registers = this->savedRegisters;
}

GbCpu::GbCpu(IMemory* memory, InterruptController* ic, ISR* isr) :
    memory(memory),
    interruptController(ic),
    isr(isr)
{
    for (int i = 1; i < 256; ++i)
    {
        this->opCodes[i] = [i]() { std::cout << "not implemented, opt code: " << i << std::endl; };
    }

    this->initXor();
    this->initRst();
    this->initLdb();
    this->initLda();
    this->initInc();
    this->initDec();
    this->initLdd();
    this->initLdh();
    this->initLdc();
    this->initLde();
    this->initLdl();
    this->initAdd();
    this->initAnd();
    this->initSub();
    this->initR();
    this->initJ();
    this->initC();
    this->initPushPop();
    this->initRcodes();
    this->initScodes();
    this->initBcodes();

    this->opCodes[Instruction::NOP] = []()
    {
    };

    this->opCodes[Instruction::STOP] = [this]()
    {
        this->stopped = true;
    };

    this->opCodes[Instruction::DI] = [this]()
    {
        this->interruptEnabled = false;
    };

    this->opCodes[Instruction::EI] = [this]()
    {
        this->interruptEnabled = true;
    };
}

void GbCpu::updateFlagsAfterAddition(uint8_t result, uint8_t operand1, uint8_t operand2)
{
    // Zero flag: set if result is zero
    this->registers.regF.zero = (result == 0);

    std::cout << "Flag zero is set: " << (this->registers.regF.zero ? "True" : " False") << std::endl;

    // Subtract flag: reset after an addition
    this->registers.regF.subtract = false;

    // Half Carry flag: set if there was a carry from bit 3 to bit 4
    this->registers.regF.halfCarry = ((operand1 & 0x0F) + (operand2 & 0x0F) > 0x0F);

    // Carry flag: set if there was a carry from the MSB
    this->registers.regF.carry = (operand1 > UINT8_MAX - operand2);
}

void GbCpu::updateFlagsAfterSubtraction(uint8_t result, uint8_t operand1, uint8_t operand2)
{
    // Update zero flag
    this->registers.regF.zero = (result == 0);

    // Update subtract flag
    this->registers.regF.subtract = true;

    // Update half-carry flag
    // (subtraction of lower nibbles generates a borrow if there's a carry from bit 4)
    this->registers.regF.halfCarry = ((operand1 & 0x0F) < (operand2 & 0x0F));

    // Update carry flag
    this->registers.regF.carry = (operand2 > operand1);
}

void GbCpu::updateFlagsAfterLogicalOperation(uint8_t value, LogicalOperation operation)
{
    // Zero flag: set if the result is zero
    this->registers.regF.zero = (value == 0);

    // Subtract flag: reset after a logical operation
    this->registers.regF.subtract = false;

    // Half Carry flag: set or reset based on the logical operation
    switch (operation)
    {
    case AND:
    case OR:
        this->registers.regF.halfCarry = false;
        break;
    case XOR:
        this->registers.regF.halfCarry = true;
        break;
    default:
        break;
    }

    // Carry flag: reset after a logical operation
    this->registers.regF.carry = false;
}
