//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"
#include "../StopCpuException.h"

#include <iostream>

void GbCpu::exec() {
    if (this->halted)
        return;

    if (this->stopped)
        throw StopCPUException();

    if (this->interruptEnabled && this->interruptController->isInterruptRequests()) {
        this->saveState();
        this->isr->handleInterrupt();
        this->restoreState();
    }

    opcode = (Instruction) this->memory->read(registers.regPC);
    registers.regPC++;

    if (opcode == Instruction::PREFIX_CB) {
        // needs to implement the cb commands
    }

    opCodes[opcode]();
}

void GbCpu::reset() {
    // Initialize flags
    this->flags.zero =
    this->flags.subtract =
    this->flags.halfCarry =
    this->flags.carry = false;

    this->registers.reset();
    this->stopped = false;
    this->halted = false;
}

void GbCpu::saveState() {
    this->savedRegisters = this->registers;
    this->savedFlags = this->flags;
}

void GbCpu::restoreState() {
    this->flags = this->savedFlags;
    this->registers = this->savedRegisters;
}

GbCpu::GbCpu(IMemory *memory, InterruptController *ic, ISR *isr) :
        memory(memory),
        interruptController(ic),
        isr(isr) {

    this->reset();

    for (int i = 1; i < 256; ++i) {
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

    this->opCodes[Instruction::NOP] = []() {};

    this->opCodes[Instruction::STOP] = [this]() {
        this->stopped = true;
    };
    this->opCodes[Instruction::DAA] = [this]() {
        // Implement DAA instruction
        // Decimal adjust accumulator
        uint8_t correction = 0x00;
        if (this->flags.halfCarry || (!this->flags.subtract && (this->registers.regA & 0x0F) > 9)) {
            correction |= 0x06;
        }
        if (this->flags.carry || (!this->flags.subtract && this->registers.regA > 0x99)) {
            correction |= 0x60;
            this->flags.carry = true;
        }
        this->registers.regA += (this->flags.subtract ? -correction : correction);
        this->flags.zero = (this->registers.regA == 0);
        this->flags.halfCarry = false;
    };

    this->opCodes[Instruction::CPL] = [this]() {
        // Implement CPL instruction
        // Complement A register (flip all bits)
        this->registers.regA = ~this->registers.regA;
        this->flags.subtract = true;
        this->flags.halfCarry = true;
    };

    this->opCodes[Instruction::SCF] = [this]() {
        // Implement SCF instruction
        // Set carry flag
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = true;
    };

    this->opCodes[Instruction::CP_B] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regB;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regB);
    };

    this->opCodes[Instruction::CP_C] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regC;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regC);
    };

    this->opCodes[Instruction::CP_D] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regD;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regD);
    };

    this->opCodes[Instruction::CP_E] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regE;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regE);
    };

    this->opCodes[Instruction::CP_H] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regH;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regH);
    };

    this->opCodes[Instruction::CP_L] = [this]() {
        uint8_t result = this->registers.regA - this->registers.regL;
        updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regL);
    };

    this->opCodes[Instruction::CP_HLptr] = [this]() {
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint8_t valueHL = this->memory->read(hl);
        uint8_t result = this->registers.regA - valueHL;
        updateFlagsAfterSubtraction(result, this->registers.regA, valueHL);
    };

    this->opCodes[Instruction::CP_A] = [this]() {
        // Comparing A with itself
        updateFlagsAfterSubtraction(0, this->registers.regA, this->registers.regA);
    };

    this->opCodes[Instruction::POP_BC] = [this]() {
        this->registers.regC = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
        this->registers.regB = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
    };

    this->opCodes[Instruction::CALL_NZ_nn] = [this]() {
        if (!this->flags.zero) {
            uint16_t returnAddress = this->registers.regPC;
            this->registers.regPC = this->memory->readWord(this->registers.regPC);
            this->registers.regSP -= 2;
            this->memory->writeWord(this->registers.regSP, returnAddress);
        } else {
            // Increment PC to skip the 16-bit address
            this->registers.regPC += 2;
        }
    };

    this->opCodes[Instruction::PUSH_BC] = [this]() {
        this->registers.regSP--;
        this->memory->writeByte(this->registers.regSP, this->registers.regB);
        this->registers.regSP--;
        this->memory->writeByte(this->registers.regSP, this->registers.regC);
    };

    this->opCodes[Instruction::PREFIX_CB] = [this]() {
        // Handle CB prefix (not implemented in this example)
        // Increment PC to skip the following byte
        this->registers.regPC++;
    };

    this->opCodes[Instruction::CALL_Z_nn] = [this]() {
        if (this->flags.zero) {
            uint16_t returnAddress = this->registers.regPC;
            this->registers.regPC = this->memory->readWord(this->registers.regPC);
            this->registers.regSP -= 2;
            this->memory->writeWord(this->registers.regSP, returnAddress);
        } else {
            // Increment PC to skip the 16-bit address
            this->registers.regPC += 2;
        }
    };

    this->opCodes[Instruction::CALL_nn] = [this]() {
        uint16_t returnAddress = this->registers.regPC;
        this->registers.regPC = this->memory->readWord(this->registers.regPC);
        this->registers.regSP -= 2;
        this->memory->writeWord(this->registers.regSP, returnAddress);
    };

    this->opCodes[Instruction::POP_DE] = [this]() {
        this->registers.regE = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
        this->registers.regD = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
    };

    this->opCodes[Instruction::CALL_NC_nn] = [this]() {
        if (!this->flags.carry) {
            uint16_t returnAddress = this->registers.regPC;
            this->registers.regPC = this->memory->readWord(this->registers.regPC);
            this->registers.regSP -= 2;
            this->memory->writeWord(this->registers.regSP, returnAddress);
        } else {
            // Increment PC to skip the 16-bit address
            this->registers.regPC += 2;
        }
    };

    this->opCodes[Instruction::PUSH_DE] = [this]() {
        this->registers.regSP--;
        this->memory->writeByte(this->registers.regSP, this->registers.regD);
        this->registers.regSP--;
        this->memory->writeByte(this->registers.regSP, this->registers.regE);
    };

    this->opCodes[Instruction::CALL_C_nn] = [this]() {
        if (this->flags.carry) {
            uint16_t returnAddress = this->registers.regPC;
            this->registers.regPC = this->memory->readWord(this->registers.regPC);
            this->registers.regSP -= 2;
            this->memory->writeWord(this->registers.regSP, returnAddress);
        } else {
            // Increment PC to skip the 16-bit address
            this->registers.regPC += 2;
        }
    };

    this->opCodes[Instruction::POP_HL] = [this]() {
        this->registers.regL = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
        this->registers.regH = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
    };

    this->opCodes[Instruction::PUSH_HL] = [this]() {
        this->registers.regSP--;
        this->memory->writeByte(this->registers.regSP, this->registers.regH);
        this->registers.regSP--;
        this->memory->writeByte(this->registers.regSP, this->registers.regL);
    };

    this->opCodes[Instruction::POP_AF] = [this]() {
        this->registers.regF = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
        this->registers.regA = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
        // Lower 4 bits of F register are not used
        this->registers.regF &= 0xF0;
    };

    this->opCodes[Instruction::DI] = [this]() {
        this->interruptEnabled = false;
    };

    this->opCodes[Instruction::PUSH_AF] = [this]() {
        this->registers.regSP--;
        this->memory->writeByte(this->registers.regSP, this->registers.regA);
        this->registers.regSP--;
        this->memory->writeByte(this->registers.regSP, this->registers.regF);
    };

    this->opCodes[Instruction::EI] = [this]() {
        this->interruptEnabled = true;
    };

    this->opCodes[Instruction::CP_n] = [this]() {
        uint8_t value = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        uint16_t temp = this->registers.regA - value;
        this->flags.zero = (temp & 0xFF) == 0;
        this->flags.subtract = true;
        this->flags.halfCarry = ((this->registers.regA & 0x0F) - (value & 0x0F)) < 0;
        this->flags.carry = temp > 0xFF;
    };
}

void GbCpu::updateFlagsAfterAddition(uint8_t result, uint8_t operand1, uint8_t operand2) {
    // Zero flag: set if result is zero
    flags.zero = (result == 0);

    // Subtract flag: reset after an addition
    flags.subtract = false;

    // Half Carry flag: set if there was a carry from bit 3 to bit 4
    flags.halfCarry = ((operand1 & 0x0F) + (operand2 & 0x0F) > 0x0F);

    // Carry flag: set if there was a carry from the MSB
    flags.carry = (operand1 > UINT8_MAX - operand2);
}

void GbCpu::updateFlagsAfterSubtraction(uint8_t result, uint8_t operand1, uint8_t operand2) {
    // Update zero flag
    this->flags.zero = (result == 0);

    // Update subtract flag
    this->flags.subtract = true;

    // Update half-carry flag
    // (subtraction of lower nibbles generates a borrow if there's a carry from bit 4)
    this->flags.halfCarry = ((operand1 & 0x0F) < (operand2 & 0x0F));

    // Update carry flag
    this->flags.carry = (operand2 > operand1);
}

void GbCpu::updateFlagsAfterLogicalOperation(uint8_t value, bool isAndOperation) {
    // Zero flag: set if the result is zero
    this->flags.zero = (value == 0);

    // Subtract flag: reset after a logical operation
    this->flags.subtract = false;

    // Half Carry flag: set after an AND operation, reset otherwise
    this->flags.halfCarry = isAndOperation;

    // Carry flag: reset after a logical operation
    this->flags.carry = false;
}
