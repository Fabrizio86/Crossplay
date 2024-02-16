//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbCpu.h"
#include "Instruction.h"

void GbCpu::exec() {
    if (this->interruptController->isInterruptRequests()) {
        this->saveState();
        this->isr->handleInterrupt();
        this->restoreState();
    }

    auto opcode = (Instruction) this->memory->read(registers.regPC);
    registers.regPC++;

    // Lookup and execute the opcode handler from the opCodes map
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

    this->opCodes = {
            {
                    Instruction::NOP,         []() {
            }
            },

            {
                    Instruction::LD_BC_nn,    [this]() {
                uint16_t value = this->memory->read(this->registers.regPC);
                this->registers.regB = (value >> 8) & 0xFF; // Store high byte in regB
                this->registers.regC = value & 0xFF; // Store low byte in regC
                this->registers.regPC += 2; // Increment PC by 2 bytes
            }
            },

            {
                    Instruction::LD_BC_A,     [this]() {
                this->memory->writeByte((this->registers.regB << 8) | this->registers.regC, this->registers.regA);
            }
            },

            {
                    Instruction::INC_BC,      [this]() {
                uint16_t bc = (this->registers.regB << 8) | this->registers.regC;
                bc++;
                this->registers.regB = (bc >> 8) & 0xFF;
                this->registers.regC = bc & 0xFF;
            }
            },

            {
                    Instruction::INC_B,       [this]() {
                this->registers.regB++;
            }
            },

            {
                    Instruction::DEC_B,       [this]() {
                this->registers.regB--;
            }
            },

            {
                    Instruction::LD_B_n,      [this]() {
                this->registers.regB = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
            }
            },

            {
                    Instruction::RLCA,        [this]() {
                // RLCA - Rotate A left, old bit 7 to carry flag
                bool carry = (this->registers.regA & 0x80) != 0;
                this->registers.regA = (this->registers.regA << 1) | carry;
                this->flags.zero = false; // Clear zero flag
                this->flags.subtract = false; // Clear subtract flag
                this->flags.halfCarry = false; // Clear half carry flag
                this->flags.carry = carry; // Set carry flag
            }
            },

            {
                    Instruction::LD_nn_SP,    [this]() {
                // LD (nn),SP - Store Stack Pointer at address n
                uint16_t address = this->memory->readWord(this->registers.regPC);
                this->registers.regPC += 2; // Increment PC by 2 bytes
                this->memory->writeWord(address, this->registers.regSP);
            }
            },

            {
                    Instruction::ADD_HL_BC,   [this]() {
                uint16_t bc = (this->registers.regB << 8) | this->registers.regC;
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                uint32_t result = static_cast<uint32_t>(hl) + static_cast<uint32_t>(bc);
                this->flags.carry = ((hl & 0xFFF) + (bc & 0xFFF)) > 0xFFF;
                this->flags.subtract = false;
                this->flags.halfCarry = ((hl & 0xFF) + (bc & 0xFF)) > 0xFF;
                this->registers.regH = (result >> 8) & 0xFF;
                this->registers.regL = result & 0xFF;
                this->registers.regPC++;
            }
            },
            {
                    Instruction::LD_A_BC,     [this]() {
                uint16_t bc = (this->registers.regB << 8) | this->registers.regC;
                this->registers.regA = this->memory->read(bc);
                this->registers.regPC++;
            }
            },
            {
                    Instruction::DEC_BC,      [this]() {
                uint16_t bc = (this->registers.regB << 8) | this->registers.regC;
                bc--;
                this->registers.regB = (bc >> 8) & 0xFF;
                this->registers.regC = bc & 0xFF;
                this->registers.regPC++;
            }
            },
            {
                    Instruction::INC_C,       [this]() {
                this->registers.regC++;
                this->flags.zero = (this->registers.regC == 0);
                this->flags.subtract = false;
                this->flags.halfCarry = (this->registers.regC & 0x0F) == 0x00;
                this->registers.regPC++;
            }
            },
            {
                    Instruction::DEC_C,       [this]() {
                this->registers.regC--;
                this->flags.zero = (this->registers.regC == 0);
                this->flags.subtract = true;
                this->flags.halfCarry = (this->registers.regC & 0x0F) == 0x0F;
                this->registers.regPC++;
            }
            },
            {
                    Instruction::LD_C_n,      [this]() {
                this->registers.regC = this->memory->read(this->registers.regPC + 1);
                this->registers.regPC += 2;
            }
            },
            {
                    Instruction::RRCA,        [this]() {
                this->flags.carry = this->registers.regA & 0x01;
                this->registers.regA = (this->registers.regA >> 1) | (this->flags.carry << 7);
                this->flags.zero = false;
                this->flags.subtract = false;
                this->flags.halfCarry = false;
                this->registers.regPC++;
            }
            },
            {
                    Instruction::STOP,        [this]() {
                // Implement STOP instruction
                // This instruction halts the CPU and LCD controller until the next interrupt
                this->stopped = true; // Set CPU stopped flag
            }
            },
            {
                    Instruction::LD_DE_nn,    [this]() {
                // Implement LD DE,nn instruction
                // Load 16-bit immediate value into DE register pair
                this->registers.regE = this->memory->read(this->registers.regPC++);
                this->registers.regD = this->memory->read(this->registers.regPC++);
            }
            },
            {
                    Instruction::LD_DE_A,     [this]() {
                // Implement LD (DE),A instruction
                // Store contents of A register at address pointed to by DE
                uint16_t de = (this->registers.regD << 8) | this->registers.regE;
                this->memory->writeByte(de, this->registers.regA);
            }
            },
            {
                    Instruction::INC_DE,      [this]() {
                // Implement INC DE instruction
                // Increment DE register pair
                uint16_t de = (this->registers.regD << 8) | this->registers.regE;
                de++;
                this->registers.regD = (de >> 8) & 0xFF;
                this->registers.regE = de & 0xFF;
            }
            },
            {
                    Instruction::INC_D,       [this]() {
                // Implement INC D instruction
                // Increment D register
                this->registers.regD++;
            }
            },
            {
                    Instruction::DEC_D,       [this]() {
                // Implement DEC D instruction
                // Decrement D register
                this->registers.regD--;
            }
            },
            {
                    Instruction::LD_D_n,      [this]() {
                // Implement LD D,n instruction
                // Load 8-bit immediate value into D register
                this->registers.regD = this->memory->read(this->registers.regPC++);
            }
            },
            {
                    Instruction::RLA,         [this]() {
                // Implement RLA instruction
                // Rotate A register left through carry flag
                bool carry = (this->registers.regA & 0x80) != 0;
                this->registers.regA = (this->registers.regA << 1) | (this->flags.carry ? 1 : 0);
                this->flags.zero = false;
                this->flags.subtract = false;
                this->flags.halfCarry = false;
                this->flags.carry = carry;
            }
            },
            {
                    Instruction::JR_NC_e,     [this]() {
                // Implement JR NC,e instruction
                // Jump relative to PC if carry flag is not set
                int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
                if (!this->flags.carry) {
                    this->registers.regPC += displacement;
                }
            }
            },
            {
                    Instruction::ADD_HL_DE,   [this]() {
                uint16_t de = (this->registers.regD << 8) | this->registers.regE;
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                uint32_t result = static_cast<uint32_t>(hl) + static_cast<uint32_t>(de);
                this->flags.carry = ((hl & 0xFFF) + (de & 0xFFF)) > 0xFFF;
                this->flags.subtract = false;
                this->flags.halfCarry = ((hl & 0xFF) + (de & 0xFF)) > 0xFF;
                this->registers.regH = (result >> 8) & 0xFF;
                this->registers.regL = result & 0xFF;
            }
            },

            {
                    Instruction::LD_A_DE,     [this]() {
                uint16_t de = (this->registers.regD << 8) | this->registers.regE;
                this->registers.regA = this->memory->read(de);
            }
            },

            {
                    Instruction::DEC_DE,      [this]() {
                uint16_t de = (this->registers.regD << 8) | this->registers.regE;
                de--;
                this->registers.regD = (de >> 8) & 0xFF;
                this->registers.regE = de & 0xFF;
            }
            },

            {
                    Instruction::INC_E,       [this]() {
                this->registers.regE++;
            }
            },

            {
                    Instruction::DEC_E,       [this]() {
                this->registers.regE--;
            }
            },

            {
                    Instruction::LD_E_n,      [this]() {
                this->registers.regE = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
            }
            },

            {
                    Instruction::RRA,         [this]() {
                // RRA - Rotate A right through carry flag
                bool carry = (this->registers.regA & 0x01) != 0;
                this->registers.regA = (this->registers.regA >> 1) | (this->flags.carry ? 0x80 : 0x00);
                this->flags.zero = false; // Clear zero flag
                this->flags.subtract = false; // Clear subtract flag
                this->flags.halfCarry = false; // Clear half carry flag
                this->flags.carry = carry; // Set carry flag
            }
            },

            {
                    Instruction::JR_NZ_n,     [this]() {
                // Implement JR NZ,n instruction
                // Jump relative to PC if zero flag is not set
                int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
                if (!this->flags.zero) {
                    this->registers.regPC += displacement;
                }
            }
            },

            {
                    Instruction::LD_HL_nn,    [this]() {
                uint16_t value = this->memory->readWord(this->registers.regPC);
                this->registers.regL = (value >> 8) & 0xFF; // Store high byte in regL
                this->registers.regH = value & 0xFF; // Store low byte in regH
                this->registers.regPC += 2; // Increment PC by 2 bytes
            }
            },

            {
                    Instruction::LD_HLI_A,    [this]() {
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regA);
                hl++;
                this->registers.regH = (hl >> 8) & 0xFF;
                this->registers.regL = hl & 0xFF;
            }
            },

            {
                    Instruction::INC_HL,      [this]() {
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                hl++;
                this->registers.regH = (hl >> 8) & 0xFF;
                this->registers.regL = hl & 0xFF;
            }
            },
            {
                    Instruction::INC_H,       [this]() {
                this->registers.regH++;
            }
            },

            {
                    Instruction::DEC_H,       [this]() {
                this->registers.regH--;
            }
            },

            {
                    Instruction::LD_H_n,      [this]() {
                this->registers.regH = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
            }
            },

            {
                    Instruction::DAA,         [this]() {
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
            }
            },

            {
                    Instruction::JR_Z_n,      [this]() {
                // Implement JR Z,n instruction
                // Jump relative to PC if zero flag is set
                int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
                if (this->flags.zero) {
                    this->registers.regPC += displacement;
                }
            }
            },

            {
                    Instruction::ADD_HL_HL,   [this]() {
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                uint32_t result = static_cast<uint32_t>(hl) + static_cast<uint32_t>(hl);
                this->flags.carry = (result & 0x10000) != 0;
                this->flags.subtract = false;
                this->flags.halfCarry = ((hl & 0x0FFF) + (hl & 0x0FFF)) > 0x0FFF;
                this->registers.regH = (result >> 8) & 0xFF;
                this->registers.regL = result & 0xFF;
            }
            },

            {
                    Instruction::LD_A_HLI,    [this]() {
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA = this->memory->read(hl);
                hl++;
                this->registers.regH = (hl >> 8) & 0xFF;
                this->registers.regL = hl & 0xFF;
            }
            },

            {
                    Instruction::DEC_HL,      [this]() {
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                hl--;
                this->registers.regH = (hl >> 8) & 0xFF;
                this->registers.regL = hl & 0xFF;
            }
            },

            {
                    Instruction::INC_L,       [this]() {
                this->registers.regL++;
                this->flags.zero = (this->registers.regL == 0);
                this->flags.subtract = false;
                this->flags.halfCarry = (this->registers.regL & 0x0F) == 0x00;
            }
            },

            {
                    Instruction::DEC_L,       [this]() {
                this->registers.regL--;
                this->flags.zero = (this->registers.regL == 0);
                this->flags.subtract = true;
                this->flags.halfCarry = (this->registers.regL & 0x0F) == 0x0F;
            }
            },

            {
                    Instruction::LD_L_n,      [this]() {
                this->registers.regL = this->memory->read(this->registers.regPC + 1);
                this->registers.regPC += 2;
            }
            },

            {
                    Instruction::CPL,         [this]() {
                // Implement CPL instruction
                // Complement A register (flip all bits)
                this->registers.regA = ~this->registers.regA;
                this->flags.subtract = true;
                this->flags.halfCarry = true;
            }
            },

            {
                    Instruction::JR_NC_n,     [this]() {
                // Implement JR NC,n instruction
                // Jump relative to PC if carry flag is not set
                int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC++));
                if (!this->flags.carry) {
                    this->registers.regPC += displacement;
                }
            }
            },
            {
                    Instruction::LD_SP_nn,    [this]() {
                // Implement LD SP, nn instruction
                // Load 16-bit immediate value into SP register
                uint16_t nn = this->memory->readWord(this->registers.regPC);
                this->registers.regSP = nn;
                this->registers.regPC += 2; // Increment PC by 2 bytes
            }
            },

            {
                    Instruction::LD_HLD_A,    [this]() {
                // Implement LD (HL-), A instruction
                // Store contents of A register at address pointed to by HL and decrement HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regA);
                hl--;
                this->registers.regH = (hl >> 8) & 0xFF;
                this->registers.regL = hl & 0xFF;
            }
            },

            {
                    Instruction::INC_SP,      [this]() {
                // Implement INC SP instruction
                // Increment SP register
                this->registers.regSP++;
            }
            },

            {
                    Instruction::INC_HLptr,   [this]() {
                // Implement INC (HL) instruction
                // Increment value stored at address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                uint8_t value = this->memory->read(hl);
                value++;
                this->memory->writeByte(hl, value);
            }
            },

            {
                    Instruction::DEC_HLptr,   [this]() {
                // Implement DEC (HL) instruction
                // Decrement value stored at address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                uint8_t value = this->memory->read(hl);
                value--;
                this->memory->writeByte(hl, value);
            }
            },

            {
                    Instruction::LD_HL_n,     [this]() {
                // Implement LD HL, n instruction
                // Load 8-bit immediate value into HL register pair
                uint8_t n = this->memory->read(this->registers.regPC);
                this->registers.regPC++; // Increment PC by 1 byte
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, n);
            }
            },

            {
                    Instruction::SCF,         [this]() {
                // Implement SCF instruction
                // Set carry flag
                this->flags.subtract = false;
                this->flags.halfCarry = false;
                this->flags.carry = true;
            }
            },

            {
                    Instruction::JR_C_n,      [this]() {
                // Implement JR C, n instruction
                // Jump relative to PC by signed immediate value if carry flag is set
                int8_t displacement = static_cast<int8_t>(this->memory->read(this->registers.regPC));
                this->registers.regPC++; // Increment PC by 1 byte
                if (this->flags.carry) {
                    this->registers.regPC += displacement;
                }
            }
            },

            {
                    Instruction::ADD_HL_SP,   [this]() {
                // Implement ADD HL, SP instruction
                // Add value of SP to value of HL
                uint16_t sp = this->registers.regSP;
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                uint32_t result = static_cast<uint32_t>(hl) + static_cast<uint32_t>(sp);
                this->flags.carry = ((hl & 0xFFF) + (sp & 0xFFF)) > 0xFFF;
                this->flags.subtract = false;
                this->flags.halfCarry = ((hl & 0xFF) + (sp & 0xFF)) > 0xFF;
                this->registers.regH = (result >> 8) & 0xFF;
                this->registers.regL = result & 0xFF;
            }
            },

            {
                    Instruction::LD_A_HLD,    [this]() {
                // Implement LD A, (HL-) instruction
                // Load value at address pointed to by HL into A and decrement HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA = this->memory->read(hl);
                hl--;
                this->registers.regH = (hl >> 8) & 0xFF;
                this->registers.regL = hl & 0xFF;
            }
            },

            {
                    Instruction::DEC_SP,      [this]() {
                // Implement DEC SP instruction
                // Decrement SP register
                this->registers.regSP--;
            }
            },

            {
                    Instruction::INC_A,       [this]() {
                // Implement INC A instruction
                // Increment A register
                this->registers.regA++;
            }
            },

            {
                    Instruction::DEC_A,       [this]() {
                // Implement DEC A instruction
                // Decrement A register
                this->registers.regA--;
            }
            },

            {
                    Instruction::LD_A_n,      [this]() {
                // Implement LD A, n instruction
                // Load 8-bit immediate value into A register
                this->registers.regA = this->memory->read(this->registers.regPC);
                this->registers.regPC++; // Increment PC by 1 byte
            }
            },

            {
                    Instruction::CCF,         [this]() {
                // Implement CCF instruction
                // Complement carry flag
                this->flags.subtract = false;
                this->flags.halfCarry = false;
                this->flags.carry = !this->flags.carry;
            }
            },

            {
                    Instruction::LD_B_B,      [this]() {
                // Implement LD B, B instruction
                // Load value of B register into B register (no operation)
                // No action needed
            }
            },

            {
                    Instruction::LD_B_C,      [this]() {
                // Implement LD B, C instruction
                // Load value of C register into B register
                this->registers.regB = this->registers.regC;
            }
            },

            {
                    Instruction::LD_B_D,      [this]() {
                // Implement LD B, D instruction
                // Load value of D register into B register
                this->registers.regB = this->registers.regD;
            }
            },

            {
                    Instruction::LD_B_E,      [this]() {
                // Implement LD B, E instruction
                // Load value of E register into B register
                this->registers.regB = this->registers.regE;
            }
            },

            {
                    Instruction::LD_B_H,      [this]() {
                // Implement LD B, H instruction
                // Load value of H register into B register
                this->registers.regB = this->registers.regH;
            }
            },

            {
                    Instruction::LD_B_L,      [this]() {
                // Implement LD B, L instruction
                // Load value of L register into B register
                this->registers.regB = this->registers.regL;
            }
            },

            {
                    Instruction::LD_B_HLptr,  [this]() {
                // Implement LD B, (HL) instruction
                // Load value at address pointed to by HL into B register
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regB = this->memory->read(hl);
            }
            },

            {
                    Instruction::LD_B_A,      [this]() {
                // Implement LD B, A instruction
                // Load value of A register into B register
                this->registers.regB = this->registers.regA;
            }
            },

            {
                    Instruction::LD_C_B,      [this]() {
                // Implement LD C, B instruction
                // Load value of B register into C register
                this->registers.regC = this->registers.regB;
            }
            },

            {
                    Instruction::LD_C_C,      [this]() {
                // Implement LD C, C instruction
                // Load value of C register into C register (no operation)
                // No action needed
            }
            },

            {
                    Instruction::LD_C_D,      [this]() {
                // Implement LD C, D instruction
                // Load value of D register into C register
                this->registers.regC = this->registers.regD;
            }
            },

            {
                    Instruction::LD_C_E,      [this]() {
                // Implement LD C, E instruction
                // Load value of E register into C register
                this->registers.regC = this->registers.regE;
            }
            },

            {
                    Instruction::LD_C_H,      [this]() {
                // Implement LD C, H instruction
                // Load value of H register into C register
                this->registers.regC = this->registers.regH;
            }
            },

            {
                    Instruction::LD_C_L,      [this]() {
                // Implement LD C, L instruction
                // Load value of L register into C register
                this->registers.regC = this->registers.regL;
            }
            },

            {
                    Instruction::LD_C_HLptr,  [this]() {
                // Implement LD C, (HL) instruction
                // Load value at address pointed to by HL into C register
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regC = this->memory->read(hl);
            }
            },

            {
                    Instruction::LD_C_A,      [this]() {
                // Implement LD C, A instruction
                // Load value of A register into C register
                this->registers.regC = this->registers.regA;
            }
            },

            {
                    Instruction::LD_D_B,      [this]() {
                // Implement LD D, B instruction
                // Load value of B register into D register
                this->registers.regD = this->registers.regB;
            }
            },

            {
                    Instruction::LD_D_C,      [this]() {
                // Implement LD D, C instruction
                // Load value of C register into D register
                this->registers.regD = this->registers.regC;
            }
            },

            {
                    Instruction::LD_D_D,      [this]() {
                // Implement LD D, D instruction
                // Load value of D register into D register (no operation)
                // No action needed
            }
            },

            {
                    Instruction::LD_D_E,      [this]() {
                // Implement LD D, E instruction
                // Load value of E register into D register
                this->registers.regD = this->registers.regE;
            }
            },

            {
                    Instruction::LD_D_H,      [this]() {
                // Implement LD D, H instruction
                // Load value of H register into D register
                this->registers.regD = this->registers.regH;
            }
            },

            {
                    Instruction::LD_D_L,      [this]() {
                // Implement LD D, L instruction
                // Load value of L register into D register
                this->registers.regD = this->registers.regL;
            }
            },

            {
                    Instruction::LD_D_HLptr,  [this]() {
                // Implement LD D, (HL) instruction
                // Load value at address pointed to by HL into D register
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regD = this->memory->read(hl);
            }
            },

            {
                    Instruction::LD_D_A,      [this]() {
                // Implement LD D, A instruction
                // Load value of A register into D register
                this->registers.regD = this->registers.regA;
            }
            },

            {
                    Instruction::LD_E_B,      [this]() {
                // Implement LD E, B instruction
                // Load value of B register into E register
                this->registers.regE = this->registers.regB;
            }
            },

            {
                    Instruction::LD_E_C,      [this]() {
                // Implement LD E, C instruction
                // Load value of C register into E register
                this->registers.regE = this->registers.regC;
            }
            },

            {
                    Instruction::LD_E_D,      [this]() {
                // Implement LD E, D instruction
                // Load value of D register into E register
                this->registers.regE = this->registers.regD;
            }
            },

            {
                    Instruction::LD_E_E,      [this]() {
                // Implement LD E, E instruction
                // Load value of E register into E register (no operation)
                // No action needed
            }
            },

            {
                    Instruction::LD_E_H,      [this]() {
                // Implement LD E, H instruction
                // Load value of H register into E register
                this->registers.regE = this->registers.regH;
            }
            },

            {
                    Instruction::LD_E_L,      [this]() {
                // Implement LD E, L instruction
                // Load value of L register into E register
                this->registers.regE = this->registers.regL;
            }
            },

            {
                    Instruction::LD_E_HLptr,  [this]() {
                // Implement LD E, (HL) instruction
                // Load value at address pointed to by HL into E register
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regE = this->memory->read(hl);
            }
            },

            {
                    Instruction::LD_E_A,      [this]() {
                // Implement LD E, A instruction
                // Load value of A register into E register
                this->registers.regE = this->registers.regA;
            }
            },

            {
                    Instruction::LD_H_B,      [this]() {
                // Implement LD H, B instruction
                // Load value of B register into H register
                this->registers.regH = this->registers.regB;
            }
            },

            {
                    Instruction::LD_H_C,      [this]() {
                // Implement LD H, C instruction
                // Load value of C register into H register
                this->registers.regH = this->registers.regC;
            }
            },

            {
                    Instruction::LD_H_D,      [this]() {
                // Implement LD H, D instruction
                // Load value of D register into H register
                this->registers.regH = this->registers.regD;
            }
            },

            {
                    Instruction::LD_H_E,      [this]() {
                // Implement LD H, E instruction
                // Load value of E register into H register
                this->registers.regH = this->registers.regE;
            }
            },

            {
                    Instruction::LD_H_H,      [this]() {
                // Implement LD H, H instruction
                // Load value of H register into H register (no operation)
                // No action needed
            }
            },

            {
                    Instruction::LD_H_L,      [this]() {
                // Implement LD H, L instruction
                // Load value of L register into H register
                this->registers.regH = this->registers.regL;
            }
            },

            {
                    Instruction::LD_H_HLptr,  [this]() {
                // Implement LD H, (HL) instruction
                // Load value at address pointed to by HL into H register
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regH = this->memory->read(hl);
            }
            },

            {
                    Instruction::LD_H_A,      [this]() {
                // Implement LD H, A instruction
                // Load value of A register into H register
                this->registers.regH = this->registers.regA;
            }
            },

            {
                    Instruction::LD_L_B,      [this]() {
                // Implement LD L, B instruction
                // Load value of B register into L register
                this->registers.regL = this->registers.regB;
            }
            },

            {
                    Instruction::LD_L_C,      [this]() {
                // Implement LD L, C instruction
                // Load value of C register into L register
                this->registers.regL = this->registers.regC;
            }
            },

            {
                    Instruction::LD_L_D,      [this]() {
                // Implement LD L, D instruction
                // Load value of D register into L register
                this->registers.regL = this->registers.regD;
            }
            },

            {
                    Instruction::LD_L_E,      [this]() {
                // Implement LD L, E instruction
                // Load value of E register into L register
                this->registers.regL = this->registers.regE;
            }
            },

            {
                    Instruction::LD_L_H,      [this]() {
                // Implement LD L, H instruction
                // Load value of H register into L register
                this->registers.regL = this->registers.regH;
            }
            },

            {
                    Instruction::LD_L_L,      [this]() {
                // Implement LD L, L instruction
                // Load value of L register into L register (no operation)
                // No action needed
            }
            },

            {
                    Instruction::LD_L_HLptr,  [this]() {
                // Implement LD L, (HL) instruction
                // Load value at address pointed to by HL into L register
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regL = this->memory->read(hl);
            }
            },

            {
                    Instruction::LD_L_A,      [this]() {
                // Implement LD L, A instruction
                // Load value of A register into L register
                this->registers.regL = this->registers.regA;
            }
            },

            {
                    Instruction::LD_HLptr_B,  [this]() {
                // Implement LD (HL), B instruction
                // Load value of B register into memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regB);
            }
            },

            {
                    Instruction::LD_HLptr_C,  [this]() {
                // Implement LD (HL), C instruction
                // Load value of C register into memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regC);
            }
            },

            {
                    Instruction::LD_HLptr_D,  [this]() {
                // Implement LD (HL), D instruction
                // Load value of D register into memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regD);
            }
            },

            {
                    Instruction::LD_HLptr_E,  [this]() {
                // Implement LD (HL), E instruction
                // Load value of E register into memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regE);
            }
            },

            {
                    Instruction::LD_HLptr_H,  [this]() {
                // Implement LD (HL), H instruction
                // Load value of H register into memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regH);
            }
            },

            {
                    Instruction::LD_HLptr_L,  [this]() {
                // Implement LD (HL), L instruction
                // Load value of L register into memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regL);
            }
            },

            {
                    Instruction::HALT,        [this]() {
                // Implement HALT instruction
                // Halt CPU and LCD controller until the next interrupt
                this->halted = true; // Set CPU halt flag
            }
            },

            {
                    Instruction::LD_HLptr_A,  [this]() {
                // Implement LD (HL), A instruction
                // Load value of A register into memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->memory->writeByte(hl, this->registers.regA);
            }
            },

            {
                    Instruction::LD_A_B,      [this]() {
                // Implement LD A, B instruction
                // Load value of B register into A register
                this->registers.regA = this->registers.regB;
            }
            },

            {
                    Instruction::LD_A_C,      [this]() {
                // Implement LD A, C instruction
                // Load value of C register into A register
                this->registers.regA = this->registers.regC;
            }
            },

            {
                    Instruction::LD_A_D,      [this]() {
                // Implement LD A, D instruction
                // Load value of D register into A register
                this->registers.regA = this->registers.regD;
            }
            },

            {
                    Instruction::LD_A_E,      [this]() {
                // Implement LD A, E instruction
                // Load value of E register into A register
                this->registers.regA = this->registers.regE;
            }
            },

            {
                    Instruction::LD_A_H,      [this]() {
                // Implement LD A, H instruction
                // Load value of H register into A register
                this->registers.regA = this->registers.regH;
            }
            },

            {
                    Instruction::LD_A_L,      [this]() {
                // Implement LD A, L instruction
                // Load value of L register into A register
                this->registers.regA = this->registers.regL;
            }
            },

            {
                    Instruction::LD_A_HLptr,  [this]() {
                // Implement LD A, (HL) instruction
                // Load value at address pointed to by HL into A register
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA = this->memory->read(hl);
            }
            },

            {
                    Instruction::LD_A_A,      []() {
                // Implement LD A, A instruction
                // Load value of A register into A register (no operation)
                // No action needed
            }
            },


            {
                    Instruction::ADD_A_B,     [this]() {
                uint8_t oldValueOfRegA = this->registers.regA;

                // Add the value of register B to register A
                this->registers.regA += this->registers.regB;
                // Set flags if necessary
                this->updateFlagsAfterAddition(this->registers.regA, oldValueOfRegA, this->registers.regB);
            }
            },
            {
                    Instruction::ADD_A_C,     [this]() {
                // Add the value of register C to register A
                this->registers.regA += this->registers.regC;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADD_A_D,     [this]() {
                // Add the value of register D to register A
                this->registers.regA += this->registers.regD;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADD_A_E,     [this]() {
                // Add the value of register E to register A
                this->registers.regA += this->registers.regE;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADD_A_H,     [this]() {
                // Add the value of register H to register A
                this->registers.regA += this->registers.regH;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADD_A_L,     [this]() {
                // Add the value of register L to register A
                this->registers.regA += this->registers.regL;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADD_A_HLptr, [this]() {
                // Add the value at the memory address pointed to by HL to register A
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA += this->memory->read(hl);
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADD_A_A,     [this]() {
                // Add the value of register A to itself (double it)
                this->registers.regA += this->registers.regA;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADC_A_B,     [this]() {
                // Add the value of register B and the carry flag to register A
                this->registers.regA += this->registers.regB + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADC_A_C,     [this]() {
                // Add the value of register C and the carry flag to register A
                this->registers.regA += this->registers.regC + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADC_A_D,     [this]() {
                // Add the value of register D and the carry flag to register A
                this->registers.regA += this->registers.regD + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADC_A_E,     [this]() {
                // Add the value of register E and the carry flag to register A
                this->registers.regA += this->registers.regE + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADC_A_H,     [this]() {
                // Add the value of register H and the carry flag to register A
                this->registers.regA += this->registers.regH + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADC_A_L,     [this]() {
                // Add the value of register L and the carry flag to register A
                this->registers.regA += this->registers.regL + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADC_A_HLptr, [this]() {
                // Add the value at the memory address pointed to by HL and the carry flag to register A
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA += this->memory->read(hl) + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::ADC_A_A,     [this]() {
                // Add the value of register A to itself, the carry flag, and set the result in A
                this->registers.regA += this->registers.regA + this->flags.carry;
                // Set flags if necessary
            }
            },

            {
                    Instruction::SUB_B,       [this]() {
                // Subtract the value of register B from register A
                this->registers.regA -= this->registers.regB;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SUB_C,       [this]() {
                // Subtract the value of register C from register A
                this->registers.regA -= this->registers.regC;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SUB_D,       [this]() {
                // Subtract the value of register D from register A
                this->registers.regA -= this->registers.regD;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SUB_E,       [this]() {
                // Subtract the value of register E from register A
                this->registers.regA -= this->registers.regE;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SUB_H,       [this]() {
                // Subtract the value of register H from register A
                this->registers.regA -= this->registers.regH;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SUB_L,       [this]() {
                // Subtract the value of register L from register A
                this->registers.regA -= this->registers.regL;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SUB_HLptr,   [this]() {
                // Subtract the value at the memory address pointed to by HL from register A
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA -= this->memory->read(hl);
                // Set flags if necessary
            }
            },
            {
                    Instruction::SUB_A,       [this]() {
                // Subtract the value of register A from itself (resulting in 0)
                this->registers.regA -= this->registers.regA;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SBC_A_B,     [this]() {
                // Subtract the value of register B and the carry flag from register A
                this->registers.regA -= this->registers.regB + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SBC_A_C,     [this]() {
                // Subtract the value of register C and the carry flag from register A
                this->registers.regA -= this->registers.regC + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SBC_A_D,     [this]() {
                // Subtract the value of register D and the carry flag from register A
                this->registers.regA -= this->registers.regD + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SBC_A_E,     [this]() {
                // Subtract the value of register E and the carry flag from register A
                this->registers.regA -= this->registers.regE + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SBC_A_H,     [this]() {
                // Subtract the value of register H and the carry flag from register A
                this->registers.regA -= this->registers.regH + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SBC_A_L,     [this]() {
                // Subtract the value of register L and the carry flag from register A
                this->registers.regA -= this->registers.regL + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SBC_A_HLptr, [this]() {
                // Subtract the value at the memory address pointed to by HL and the carry flag from register A
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA -= this->memory->read(hl) + this->flags.carry;
                // Set flags if necessary
            }
            },
            {
                    Instruction::SBC_A_A,     [this]() {
                // Subtract the value of register A from itself (resulting in 0), the carry flag, and set the result in A
                this->registers.regA -= this->registers.regA + this->flags.carry;
                // Set flags if necessary
            }
            },

            {
                    Instruction::AND_B,       [this]() {
                // Bitwise AND operation between register A and register B
                this->registers.regA &= this->registers.regB;
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },

            {
                    Instruction::AND_C,       [this]() {
                // Bitwise AND operation between register A and register C
                this->registers.regA &= this->registers.regC;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::AND_D,       [this]() {
                // Bitwise AND operation between register A and register D
                this->registers.regA &= this->registers.regD;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::AND_E,       [this]() {
                // Bitwise AND operation between register A and register E
                this->registers.regA &= this->registers.regE;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::AND_H,       [this]() {
                // Bitwise AND operation between register A and register H
                this->registers.regA &= this->registers.regH;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::AND_L,       [this]() {
                // Bitwise AND operation between register A and register L
                this->registers.regA &= this->registers.regL;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::AND_HLptr,   [this]() {
                // Bitwise AND operation between register A and the value at the memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA &= this->memory->read(hl);
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::XOR_B,       [this]() {
                // Bitwise XOR operation between register A and register B
                this->registers.regA ^= this->registers.regB;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::XOR_C,       [this]() {
                // Bitwise XOR operation between register A and register C
                this->registers.regA ^= this->registers.regC;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::XOR_D,       [this]() {
                // Bitwise XOR operation between register A and register D
                this->registers.regA ^= this->registers.regD;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::XOR_E,       [this]() {
                // Bitwise XOR operation between register A and register E
                this->registers.regA ^= this->registers.regE;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::XOR_H,       [this]() {
                // Bitwise XOR operation between register A and register H
                this->registers.regA ^= this->registers.regH;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::XOR_L,       [this]() {
                // Bitwise XOR operation between register A and register L
                this->registers.regA ^= this->registers.regL;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::XOR_HLptr,   [this]() {
                // Bitwise XOR operation between register A and the value at the memory address pointed to by HL
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA ^= this->memory->read(hl);
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::XOR_A,       [this]() {
                // Bitwise XOR operation between register A and itself (resulting in 0)
                this->registers.regA ^= this->registers.regA;
                // Update flags if necessary
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },


            {
                    Instruction::OR_B,        [this]() {
                this->registers.regA |= this->registers.regB;
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },

            {
                    Instruction::OR_C,        [this]() {
                this->registers.regA |= this->registers.regC;
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },

            {
                    Instruction::OR_D,        [this]() {
                this->registers.regA |= this->registers.regD;
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },

            {
                    Instruction::OR_E,        [this]() {
                this->registers.regA |= this->registers.regE;
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },

            {
                    Instruction::OR_H,        [this]() {
                this->registers.regA |= this->registers.regH;
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },

            {
                    Instruction::OR_L,        [this]() {
                this->registers.regA |= this->registers.regL;
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },

            {
                    Instruction::OR_HLptr,    [this]() {
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                this->registers.regA |= this->memory->read(hl);
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },

            {
                    Instruction::OR_A,        [this]() {
                // No change to A
                updateFlagsAfterLogicalOperation(this->registers.regA);
            }
            },
            {
                    Instruction::CP_B,        [this]() {
                uint8_t result = this->registers.regA - this->registers.regB;
                updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regB);
            }
            },

            {
                    Instruction::CP_C,        [this]() {
                uint8_t result = this->registers.regA - this->registers.regC;
                updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regC);
            }
            },

            {
                    Instruction::CP_D,        [this]() {
                uint8_t result = this->registers.regA - this->registers.regD;
                updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regD);
            }
            },

            {
                    Instruction::CP_E,        [this]() {
                uint8_t result = this->registers.regA - this->registers.regE;
                updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regE);
            }
            },

            {
                    Instruction::CP_H,        [this]() {
                uint8_t result = this->registers.regA - this->registers.regH;
                updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regH);
            }
            },

            {
                    Instruction::CP_L,        [this]() {
                uint8_t result = this->registers.regA - this->registers.regL;
                updateFlagsAfterSubtraction(result, this->registers.regA, this->registers.regL);
            }
            },

            {
                    Instruction::CP_HLptr,    [this]() {
                uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
                uint8_t valueHL = this->memory->read(hl);
                uint8_t result = this->registers.regA - valueHL;
                updateFlagsAfterSubtraction(result, this->registers.regA, valueHL);
            }
            },

            {
                    Instruction::CP_A,        [this]() {
                // Comparing A with itself
                updateFlagsAfterSubtraction(0, this->registers.regA, this->registers.regA);
            }
            },

            {
                    Instruction::RET_NZ,      [this]() {
                if (!this->flags.zero) {
                    uint16_t address = this->memory->readWord(this->registers.regSP);
                    this->registers.regSP += 2;
                    this->registers.regPC = address;
                }
            }
            },

            {
                    Instruction::POP_BC,      [this]() {
                this->registers.regC = this->memory->read(this->registers.regSP);
                this->registers.regSP++;
                this->registers.regB = this->memory->read(this->registers.regSP);
                this->registers.regSP++;
            }
            },

            {
                    Instruction::JP_NZ_nn,    [this]() {
                uint16_t address = this->memory->readWord(this->registers.regPC);
                this->registers.regPC += 2;
                if (!this->flags.zero) {
                    this->registers.regPC = address;
                }
            }
            },

            {
                    Instruction::JP_nn,       [this]() {
                // Read the 16-bit address from memory at the location of the program counter
                uint16_t address = this->memory->readWord(this->registers.regPC);
                this->registers.regPC += 2;

                // Set the program counter to the read address, effectively jumping to that address
                this->registers.regPC = address;
            }
            },

            {
                    Instruction::CALL_NZ_nn,  [this]() {
                if (!this->flags.zero) {
                    uint16_t returnAddress = this->registers.regPC;
                    this->registers.regPC = this->memory->readWord(this->registers.regPC);
                    this->registers.regSP -= 2;
                    this->memory->writeWord(this->registers.regSP, returnAddress);
                } else {
                    // Increment PC to skip the 16-bit address
                    this->registers.regPC += 2;
                }
            }
            },

            {
                    Instruction::PUSH_BC,     [this]() {
                this->registers.regSP--;
                this->memory->writeByte(this->registers.regSP, this->registers.regB);
                this->registers.regSP--;
                this->memory->writeByte(this->registers.regSP, this->registers.regC);
            }
            },

            {
                    Instruction::ADD_A_n,     [this]() {
                uint8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                uint16_t temp = this->registers.regA + value;
                this->flags.zero = (temp & 0xFF) == 0;
                this->flags.subtract = false;
                this->flags.halfCarry = ((this->registers.regA & 0x0F) + (value & 0x0F)) > 0x0F;
                this->flags.carry = temp > 0xFF;
                this->registers.regA = temp & 0xFF;
            }
            },

            {
                    Instruction::RST_00H,     [this]() {
                this->registers.regSP -= 2;
                this->memory->writeWord(this->registers.regSP, this->registers.regPC);
                this->registers.regPC = 0x0000;
            }
            },

            {
                    Instruction::RET_Z,       [this]() {
                if (this->flags.zero) {
                    uint16_t address = this->memory->readWord(this->registers.regSP);
                    this->registers.regSP += 2;
                    this->registers.regPC = address;
                }
            }
            },

            {
                    Instruction::RET,         [this]() {
                uint16_t address = this->memory->readWord(this->registers.regSP);
                this->registers.regSP += 2;
                this->registers.regPC = address;
            }
            },

            {
                    Instruction::JP_Z_nn,     [this]() {
                uint16_t address = this->memory->readWord(this->registers.regPC);
                this->registers.regPC += 2;
                if (this->flags.zero) {
                    this->registers.regPC = address;
                }
            }
            },

            {
                    Instruction::PREFIX_CB,   [this]() {
                // Handle CB prefix (not implemented in this example)
                // Increment PC to skip the following byte
                this->registers.regPC++;
            }
            },

            {
                    Instruction::CALL_Z_nn,   [this]() {
                if (this->flags.zero) {
                    uint16_t returnAddress = this->registers.regPC;
                    this->registers.regPC = this->memory->readWord(this->registers.regPC);
                    this->registers.regSP -= 2;
                    this->memory->writeWord(this->registers.regSP, returnAddress);
                } else {
                    // Increment PC to skip the 16-bit address
                    this->registers.regPC += 2;
                }
            }
            },

            {
                    Instruction::CALL_nn,     [this]() {
                uint16_t returnAddress = this->registers.regPC;
                this->registers.regPC = this->memory->readWord(this->registers.regPC);
                this->registers.regSP -= 2;
                this->memory->writeWord(this->registers.regSP, returnAddress);
            }
            },

            {
                    Instruction::ADC_A_n,     [this]() {
                uint8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                uint16_t temp = this->registers.regA + value + this->flags.carry;
                this->flags.zero = (temp & 0xFF) == 0;
                this->flags.subtract = false;
                this->flags.halfCarry = ((this->registers.regA & 0x0F) + (value & 0x0F) + this->flags.carry) > 0x0F;
                this->flags.carry = temp > 0xFF;
                this->registers.regA = temp & 0xFF;
            }
            },

            {
                    Instruction::RST_08H,     [this]() {
                this->registers.regSP -= 2;
                this->memory->writeWord(this->registers.regSP, this->registers.regPC);
                this->registers.regPC = 0x0008;
            }
            },

            {
                    Instruction::RET_NC,      [this]() {
                if (!this->flags.carry) {
                    uint16_t address = this->memory->readWord(this->registers.regSP);
                    this->registers.regSP += 2;
                    this->registers.regPC = address;
                }
            }
            },

            {
                    Instruction::POP_DE,      [this]() {
                this->registers.regE = this->memory->read(this->registers.regSP);
                this->registers.regSP++;
                this->registers.regD = this->memory->read(this->registers.regSP);
                this->registers.regSP++;
            }
            },

            {
                    Instruction::JP_NC_nn,    [this]() {
                uint16_t address = this->memory->readWord(this->registers.regPC);
                this->registers.regPC += 2;
                if (!this->flags.carry) {
                    this->registers.regPC = address;
                }
            }
            },

            {
                    Instruction::CALL_NC_nn,  [this]() {
                if (!this->flags.carry) {
                    uint16_t returnAddress = this->registers.regPC;
                    this->registers.regPC = this->memory->readWord(this->registers.regPC);
                    this->registers.regSP -= 2;
                    this->memory->writeWord(this->registers.regSP, returnAddress);
                } else {
                    // Increment PC to skip the 16-bit address
                    this->registers.regPC += 2;
                }
            }
            },

            {
                    Instruction::PUSH_DE,     [this]() {
                this->registers.regSP--;
                this->memory->writeByte(this->registers.regSP, this->registers.regD);
                this->registers.regSP--;
                this->memory->writeByte(this->registers.regSP, this->registers.regE);
            }
            },

            {
                    Instruction::SUB_n,       [this]() {
                uint8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                uint16_t temp = this->registers.regA - value;
                this->flags.zero = (temp & 0xFF) == 0;
                this->flags.subtract = true;
                this->flags.halfCarry = ((this->registers.regA & 0x0F) - (value & 0x0F)) < 0;
                this->flags.carry = temp > 0xFF;
                this->registers.regA = temp & 0xFF;
            }
            },

            {
                    Instruction::RST_10H,     [this]() {
                this->registers.regSP -= 2;
                this->memory->writeWord(this->registers.regSP, this->registers.regPC);
                this->registers.regPC = 0x0010;
            }
            },

            {
                    Instruction::RET_C,       [this]() {
                if (this->flags.carry) {
                    uint16_t address = this->memory->readWord(this->registers.regSP);
                    this->registers.regSP += 2;
                    this->registers.regPC = address;
                }
            }
            },

            {
                    Instruction::RETI,        [this]() {
                this->interruptController->requestInterrupt(InterruptType::VBlank);
                uint16_t address = this->memory->readWord(this->registers.regSP);
                this->registers.regSP += 2;
                this->registers.regPC = address;
            }
            },

            {
                    Instruction::JP_C_nn,     [this]() {
                uint16_t address = this->memory->readWord(this->registers.regPC);
                this->registers.regPC += 2;
                if (this->flags.carry) {
                    this->registers.regPC = address;
                }
            }
            },

            {
                    Instruction::CALL_C_nn,   [this]() {
                if (this->flags.carry) {
                    uint16_t returnAddress = this->registers.regPC;
                    this->registers.regPC = this->memory->readWord(this->registers.regPC);
                    this->registers.regSP -= 2;
                    this->memory->writeWord(this->registers.regSP, returnAddress);
                } else {
                    // Increment PC to skip the 16-bit address
                    this->registers.regPC += 2;
                }
            }
            },

            {
                    Instruction::SBC_A_n,     [this]() {
                uint8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                uint16_t temp = this->registers.regA - value - this->flags.carry;
                this->flags.zero = (temp & 0xFF) == 0;
                this->flags.subtract = true;
                this->flags.halfCarry = ((this->registers.regA & 0x0F) - (value & 0x0F) - this->flags.carry) < 0;
                this->flags.carry = temp > 0xFF;
                this->registers.regA = temp & 0xFF;
            }
            },

            {
                    Instruction::RST_18H,     [this]() {
                this->registers.regSP -= 2;
                this->memory->writeWord(this->registers.regSP, this->registers.regPC);
                this->registers.regPC = 0x0018;
            }
            },

            {
                    Instruction::LDH_n_A,     [this]() {
                uint8_t address = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                this->memory->writeByte(0xFF00 + address, this->registers.regA);
            }
            },

            {
                    Instruction::POP_HL,      [this]() {
                this->registers.regL = this->memory->read(this->registers.regSP);
                this->registers.regSP++;
                this->registers.regH = this->memory->read(this->registers.regSP);
                this->registers.regSP++;
            }
            },

            {
                    Instruction::LD_Cptr_A,   [this]() {
                uint8_t address = this->registers.regC;
                this->memory->writeByte(0xFF00 + address, this->registers.regA);
            }
            },

            {
                    Instruction::PUSH_HL,     [this]() {
                this->registers.regSP--;
                this->memory->writeByte(this->registers.regSP, this->registers.regH);
                this->registers.regSP--;
                this->memory->writeByte(this->registers.regSP, this->registers.regL);
            }
            },

            {
                    Instruction::AND_n,       [this]() {
                uint8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                this->registers.regA &= value;
                this->flags.zero = this->registers.regA == 0;
                this->flags.subtract = false;
                this->flags.halfCarry = true;
                this->flags.carry = false;
            }
            },

            {
                    Instruction::RST_20H,     [this]() {
                this->registers.regSP -= 2;
                this->memory->writeWord(this->registers.regSP, this->registers.regPC);
                this->registers.regPC = 0x0020;
            }
            },

            {
                    Instruction::ADD_SP_d,    [this]() {
                int8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                uint16_t result = this->registers.regSP + value;
                this->flags.zero = false;
                this->flags.subtract = false;
                this->flags.halfCarry = ((this->registers.regSP ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10;
                this->flags.carry = ((this->registers.regSP ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100;
                this->registers.regSP = result & 0xFFFF;
            }
            },

            {
                    Instruction::JP_HL,       [this]() {
                this->registers.regPC = (this->registers.regH << 8) | this->registers.regL;
            }
            },

            {
                    Instruction::LD_nn_A,     [this]() {
                uint16_t address = this->memory->readWord(this->registers.regPC);
                this->registers.regPC += 2;
                this->memory->writeByte(address, this->registers.regA);
            }
            },


            {
                    Instruction::XOR_n,       [this]() {
                uint8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                this->registers.regA ^= value;
                this->flags.zero = this->registers.regA == 0;
                this->flags.subtract = false;
                this->flags.halfCarry = false;
                this->flags.carry = false;
            }
            },

            {
                    Instruction::RST_28H,     [this]() {
                this->registers.regSP -= 2;
                this->memory->writeWord(this->registers.regSP, this->registers.regPC);
                this->registers.regPC = 0x0028;
            }
            },

            {
                    Instruction::LDH_A_n,     [this]() {
                uint8_t address = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                this->registers.regA = this->memory->read(0xFF00 + address);
            }
            },

            {
                    Instruction::POP_AF,      [this]() {
                this->registers.regF = this->memory->read(this->registers.regSP);
                this->registers.regSP++;
                this->registers.regA = this->memory->read(this->registers.regSP);
                this->registers.regSP++;
                // Lower 4 bits of F register are not used
                this->registers.regF &= 0xF0;
            }
            },

            {
                    Instruction::LD_A_Cptr,   [this]() {
                uint8_t address = this->registers.regC;
                this->registers.regA = this->memory->read(0xFF00 + address);
            }
            },

            {
                    Instruction::DI,          [this]() {
                this->interruptController->requestInterrupt(InterruptType::DI);
            }
            },

            {
                    Instruction::PUSH_AF,     [this]() {
                this->registers.regSP--;
                this->memory->writeByte(this->registers.regSP, this->registers.regA);
                this->registers.regSP--;
                this->memory->writeByte(this->registers.regSP, this->registers.regF);
            }
            },

            {
                    Instruction::OR_n,        [this]() {
                uint8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                this->registers.regA |= value;
                this->flags.zero = this->registers.regA == 0;
                this->flags.subtract = false;
                this->flags.halfCarry = false;
                this->flags.carry = false;
            }
            },
            {
                    Instruction::RST_30H,     [this]() {
                this->registers.regSP -= 2;
                this->memory->writeWord(this->registers.regSP, this->registers.regPC);
                this->registers.regPC = 0x0030;
            }
            },

            {
                    Instruction::LD_HL_SPd,   [this]() {
                int8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                uint16_t result = this->registers.regSP + value;
                this->registers.regH = (result >> 8) & 0xFF;
                this->registers.regL = result & 0xFF;
                this->flags.zero = false;
                this->flags.subtract = false;
                this->flags.halfCarry = ((this->registers.regSP ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10;
                this->flags.carry = ((this->registers.regSP ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100;
            }
            },

            {
                    Instruction::LD_SP_HL,    [this]() {
                this->registers.regSP = (this->registers.regH << 8) | this->registers.regL;
            }
            },

            {
                    Instruction::LD_A_nn,     [this]() {
                uint16_t address = this->memory->readWord(this->registers.regPC);
                this->registers.regPC += 2;
                this->registers.regA = this->memory->read(address);
            }
            },

            {
                    Instruction::EI,          [this]() {
                this->interruptController->clearInterrupt(InterruptType::DI);
            }
            },

            {
                    Instruction::CP_n,        [this]() {
                uint8_t value = this->memory->read(this->registers.regPC);
                this->registers.regPC++;
                uint16_t temp = this->registers.regA - value;
                this->flags.zero = (temp & 0xFF) == 0;
                this->flags.subtract = true;
                this->flags.halfCarry = ((this->registers.regA & 0x0F) - (value & 0x0F)) < 0;
                this->flags.carry = temp > 0xFF;
            }
            },
            {       Instruction::RST_38H,     [this]() {
                this->memory->writeWord(this->registers.regSP - 2, this->registers.regPC);
                this->registers.regSP -= 2;

                this->registers.regPC = 0x38;
            }
            }

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

void GbCpu::updateFlagsAfterLogicalOperation(uint8_t result) {
    // Update zero flag
    this->flags.zero = (result == 0);

    // Reset subtract flag
    this->flags.subtract = false;

    // Reset carry flag
    this->flags.carry = false;

    // Reset half-carry flag
    this->flags.halfCarry = false;
}
