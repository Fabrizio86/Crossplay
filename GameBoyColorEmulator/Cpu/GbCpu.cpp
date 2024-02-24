//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"
#include "../Instruction.h"

void GbCpu::exec() {
    if (this->interruptEnabled) {
        if (this->interruptController->isInterruptRequests()) {
            this->saveState();
            this->isr->handleInterrupt();
            this->restoreState();
        }
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

    this->opCodes[Instruction::NOP] = []() {};

    this->opCodes[Instruction::RLCA] = [this]() {
        // RLCA - Rotate A left, old bit 7 to carry flag
        bool carry = (this->registers.regA & 0x80) != 0;
        this->registers.regA = (this->registers.regA << 1) | carry;
        this->flags.zero = false; // Clear zero flag
        this->flags.subtract = false; // Clear subtract flag
        this->flags.halfCarry = false; // Clear half carry flag
        this->flags.carry = carry; // Set carry flag
    };

    this->opCodes[Instruction::LD_nn_SP] = [this]() {
        // LD (nn),SP - Store Stack Pointer at address n
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2; // Increment PC by 2 bytes
        this->memory->writeWord(address, this->registers.regSP);
    };


    this->opCodes[Instruction::RRCA] = [this]() {
        this->flags.carry = this->registers.regA & 0x01;
        this->registers.regA = (this->registers.regA >> 1) | (this->flags.carry << 7);
        this->flags.zero = false;
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->registers.regPC++;
    };

    this->opCodes[Instruction::STOP] = [this]() {
        // Implement STOP instruction
        // This instruction halts the CPU and LCD controller until the next interrupt
        this->stopped = true; // Set CPU stopped flag
    };

    this->opCodes[Instruction::RLA] = [this]() {
        // Implement RLA instruction
        // Rotate A register left through carry flag
        bool carry = (this->registers.regA & 0x80) != 0;
        this->registers.regA = (this->registers.regA << 1) | (this->flags.carry ? 1 : 0);
        this->flags.zero = false;
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = carry;
    };

    this->opCodes[Instruction::JR_NC_e] = [this]() {
        // Implement JR NC,e instruction
        // Jump relative to PC if carry flag is not set
        int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        if (!this->flags.carry) {
            this->registers.regPC += displacement;
        }
    };


    this->opCodes[Instruction::RRA] = [this]() {
        // RRA - Rotate A right through carry flag
        bool carry = (this->registers.regA & 0x01) != 0;
        this->registers.regA = (this->registers.regA >> 1) | (this->flags.carry ? 0x80 : 0x00);
        this->flags.zero = false; // Clear zero flag
        this->flags.subtract = false; // Clear subtract flag
        this->flags.halfCarry = false; // Clear half carry flag
        this->flags.carry = carry; // Set carry flag
    };

    this->opCodes[Instruction::JR_NZ_n] = [this]() {
        // Implement JR NZ,n instruction
        // Jump relative to PC if zero flag is not set
        int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        if (!this->flags.zero) {
            this->registers.regPC += displacement;
        }
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

    this->opCodes[Instruction::JR_Z_n] = [this]() {
        // Implement JR Z,n instruction
        // Jump relative to PC if zero flag is set
        int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        if (this->flags.zero) {
            this->registers.regPC += displacement;
        }
    };

    this->opCodes[Instruction::CPL] = [this]() {
        // Implement CPL instruction
        // Complement A register (flip all bits)
        this->registers.regA = ~this->registers.regA;
        this->flags.subtract = true;
        this->flags.halfCarry = true;
    };

    this->opCodes[Instruction::JR_NC_n] = [this]() {
        // Implement JR NC,n instruction
        // Jump relative to PC if carry flag is not set
        int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
        if (!this->flags.carry) {
            this->registers.regPC += displacement;
        }
    };

    this->opCodes[Instruction::LD_SP_nn] = [this]() {
        // Implement LD SP, nn instruction
        // Load 16-bit immediate value into SP register
        uint16_t nn = this->memory->readWord(this->registers.regPC);
        this->registers.regSP = nn;
        this->registers.regPC += 2; // Increment PC by 2 bytes
    };

    this->opCodes[Instruction::SCF] = [this]() {
        // Implement SCF instruction
        // Set carry flag
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = true;
    };

    this->opCodes[Instruction::JR_C_n] = [this]() {
        // Implement JR C, n instruction
        // Jump relative to PC by signed immediate value if carry flag is set
        int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC));
        this->registers.regPC++; // Increment PC by 1 byte
        if (this->flags.carry) {
            this->registers.regPC += displacement;
        }
    };

    this->opCodes[Instruction::CCF] = [this]() {
        // Implement CCF instruction
        // Complement carry flag
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = !this->flags.carry;
    };

    this->opCodes[Instruction::HALT] = [this]() {
        // Implement HALT instruction
        // Halt CPU and LCD controller until the next interrupt
        this->halted = true; // Set CPU halt flag
    };

    this->opCodes[Instruction::SUB_B] = [this]() {
        // Subtract the value of register B from register A
        this->registers.regA -= this->registers.regB;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SUB_C] = [this]() {
        // Subtract the value of register C from register A
        this->registers.regA -= this->registers.regC;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SUB_D] = [this]() {
        // Subtract the value of register D from register A
        this->registers.regA -= this->registers.regD;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SUB_E] = [this]() {
        // Subtract the value of register E from register A
        this->registers.regA -= this->registers.regE;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SUB_H] = [this]() {
        // Subtract the value of register H from register A
        this->registers.regA -= this->registers.regH;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SUB_L] = [this]() {
        // Subtract the value of register L from register A
        this->registers.regA -= this->registers.regL;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SUB_HLptr] = [this]() {
        // Subtract the value at the memory address pointed to by HL from register A
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regA -= this->memory->read(hl);
        // Set flags if necessary
    };

    this->opCodes[Instruction::SUB_A] = [this]() {
        // Subtract the value of register A from itself (resulting in 0)
        this->registers.regA -= this->registers.regA;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SBC_A_B] = [this]() {
        // Subtract the value of register B and the carry flag from register A
        this->registers.regA -= this->registers.regB + this->flags.carry;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SBC_A_C] = [this]() {
        // Subtract the value of register C and the carry flag from register A
        this->registers.regA -= this->registers.regC + this->flags.carry;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SBC_A_D] = [this]() {
        // Subtract the value of register D and the carry flag from register A
        this->registers.regA -= this->registers.regD + this->flags.carry;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SBC_A_E] = [this]() {
        // Subtract the value of register E and the carry flag from register A
        this->registers.regA -= this->registers.regE + this->flags.carry;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SBC_A_H] = [this]() {
        // Subtract the value of register H and the carry flag from register A
        this->registers.regA -= this->registers.regH + this->flags.carry;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SBC_A_L] = [this]() {
        // Subtract the value of register L and the carry flag from register A
        this->registers.regA -= this->registers.regL + this->flags.carry;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SBC_A_HLptr] = [this]() {
        // Subtract the value at the memory address pointed to by HL and the carry flag from register A
        uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regA -= this->memory->read(hl) + this->flags.carry;
        // Set flags if necessary
    };

    this->opCodes[Instruction::SBC_A_A] = [this]() {
        // Subtract the value of register A from itself (resulting in 0), the carry flag, and set the result in A
        this->registers.regA -= this->registers.regA + this->flags.carry;
        // Set flags if necessary
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

    this->opCodes[Instruction::RET_NZ] = [this]() {
        if (!this->flags.zero) {
            uint16_t address = this->memory->readWord(this->registers.regSP);
            this->registers.regSP += 2;
            this->registers.regPC = address;
        }
    };

    this->opCodes[Instruction::POP_BC] = [this]() {
        this->registers.regC = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
        this->registers.regB = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
    };

    this->opCodes[Instruction::JP_NZ_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        if (!this->flags.zero) {
            this->registers.regPC = address;
        }
    };

    this->opCodes[Instruction::JP_nn] = [this]() {
        // Read the 16-bit address from memory at the location of the program counter
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        // Set the program counter to the read address, effectively jumping to that address
        this->registers.regPC = address;
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


    this->opCodes[Instruction::RET_Z] = [this]() {
        if (this->flags.zero) {
            uint16_t address = this->memory->readWord(this->registers.regSP);
            this->registers.regSP += 2;
            this->registers.regPC = address;
        }
    };

    this->opCodes[Instruction::RET] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regSP);
        this->registers.regSP += 2;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_Z_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        if (this->flags.zero) {
            this->registers.regPC = address;
        }
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

    this->opCodes[Instruction::RET_NC] = [this]() {
        if (!this->flags.carry) {
            uint16_t address = this->memory->readWord(this->registers.regSP);
            this->registers.regSP += 2;
            this->registers.regPC = address;
        }
    };

    this->opCodes[Instruction::POP_DE] = [this]() {
        this->registers.regE = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
        this->registers.regD = this->memory->read(this->registers.regSP);
        this->registers.regSP++;
    };

    this->opCodes[Instruction::JP_NC_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        if (!this->flags.carry) {
            this->registers.regPC = address;
        }
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

    this->opCodes[Instruction::SUB_n] = [this]() {
        uint8_t value = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        uint16_t temp = this->registers.regA - value;
        this->flags.zero = (temp & 0xFF) == 0;
        this->flags.subtract = true;
        this->flags.halfCarry = ((this->registers.regA & 0x0F) - (value & 0x0F)) < 0;
        this->flags.carry = temp > 0xFF;
        this->registers.regA = temp & 0xFF;
    };

    this->opCodes[Instruction::RET_C] = [this]() {
        if (this->flags.carry) {
            uint16_t address = this->memory->readWord(this->registers.regSP);
            this->registers.regSP += 2;
            this->registers.regPC = address;
        }
    };

    this->opCodes[Instruction::RETI] = [this]() {
        this->interruptController->requestInterrupt(InterruptType::VBlank);
        uint16_t address = this->memory->readWord(this->registers.regSP);
        this->registers.regSP += 2;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_C_nn] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        if (this->flags.carry) {
            this->registers.regPC = address;
        }
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

    this->opCodes[Instruction::SBC_A_n] = [this]() {
        uint8_t value = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        uint16_t temp = this->registers.regA - value - this->flags.carry;
        this->flags.zero = (temp & 0xFF) == 0;
        this->flags.subtract = true;
        this->flags.halfCarry = ((this->registers.regA & 0x0F) - (value & 0x0F) - this->flags.carry) < 0;
        this->flags.carry = temp > 0xFF;
        this->registers.regA = temp & 0xFF;
    };

    this->opCodes[Instruction::LDH_n_A] = [this]() {
        uint8_t address = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        this->memory->writeByte(0xFF00 + address, this->registers.regA);
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

    this->opCodes[Instruction::JP_HL] = [this]() {
        this->registers.regPC = (this->registers.regH << 8) | this->registers.regL;
    };

    this->opCodes[Instruction::LD_nn_A] = [this]() {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        this->memory->writeByte(address, this->registers.regA);
    };

    this->opCodes[Instruction::LDH_A_n] = [this]() {
        uint8_t address = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        this->registers.regA = this->memory->read(0xFF00 + address);
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

    this->opCodes[Instruction::LD_SP_HL] = [this]() {
        this->registers.regSP = (this->registers.regH << 8) | this->registers.regL;
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
    this->flags.carry = (result < operand1);
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
