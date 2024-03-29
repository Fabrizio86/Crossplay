//
// Created by Fabrizio Paino on 2024-02-24.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

void conditionalJump(uint16_t& regPC, bool condition, int16_t offset)
{
    if (condition)
    {
        regPC += offset;
    }
    else
    {
        // Skip the offset byte
        regPC++;
    }
}

bool GbCpu::MeetCondition(const Condition condition) const
{
    switch (condition)
    {
    case Condition::C:
        return this->registers.regF.carry;
    case Condition::NC:
        return !this->registers.regF.carry;
    case Condition::Z:
        return this->registers.regF.zero;
    case Condition::NZ:
        return !this->registers.regF.zero;
    default:
        return false;
    }
}

void GbCpu::initJ()
{
    this->opCodes[Instruction::JR_NC_e] = [this]()
    {
        if (!this->registers.regF.carry) // if there's no carry
        {
            this->registers.regPC++;
            return;
        }
        uint8_t byte = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        int8_t offset = static_cast<int8_t>(byte);
        uint16_t oldValue = this->registers.regPC;
        uint16_t newValue = static_cast<uint16_t>(oldValue + offset);
        this->registers.regPC = newValue;
    };

    this->opCodes[Instruction::JR_NZ_n] = [this]()
    {
        if (this->MeetCondition(Condition::NZ))
        {
            const int8_t offset = this->memory->readSigned(this->registers.regPC++);
            this->registers.regPC = static_cast<uint16_t>(this->registers.regPC + offset);
        }
        else
        {
            this->registers.regPC++;
        }
    };

    this->opCodes[Instruction::JR_Z_n] = [this]()
    {
        if (this->registers.regF.zero)
        {
            this->registers.regPC++;
            return;
        }
        uint8_t byte = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        int8_t offset = static_cast<int8_t>(byte);
        uint16_t oldValue = this->registers.regPC;
        uint16_t newValue = static_cast<uint16_t>(oldValue + offset);
        this->registers.regPC = newValue;
    };

    this->opCodes[Instruction::JR_NC_n] = [this]()
    {
        if (!this->registers.regF.carry) // If there's no carry
        {
            this->registers.regPC++;
            return;
        }

        uint8_t byte = this->memory->read(this->registers.regPC);
        this->registers.regPC++;
        int8_t offset = static_cast<int8_t>(byte);
        uint16_t oldValue = this->registers.regPC;
        uint16_t newValue = static_cast<uint16_t>(oldValue + offset);
        this->registers.regPC = newValue;
    };

    this->opCodes[Instruction::JR_C_n] = [this]()
    {
        int16_t offset = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;
        conditionalJump(this->registers.regPC, this->registers.regF.carry, offset);
    };

    this->opCodes[Instruction::JP_NZ_nn] = [this]()
    {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (this->registers.regF.zero) return;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_nn] = [this]()
    {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_Z_nn] = [this]()
    {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (!this->registers.regF.zero) return;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_NC_nn] = [this]()
    {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (this->registers.regF.carry) return;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_C_nn] = [this]()
    {
        uint16_t address = this->memory->readWord(this->registers.regPC);
        this->registers.regPC += 2;

        if (!this->registers.regF.carry) return;
        this->registers.regPC = address;
    };

    this->opCodes[Instruction::JP_HL] = [this]()
    {
        uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        this->registers.regPC = address;
    };
}
