//
// Created by Fabrizio Paino on 2024-02-24.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//
#include "GbCpu.h"

#include <iostream>

void GbCpu::initPushPop()
{
    this->opCodes[Instruction::POP_BC] = [this]()
    {
        this->registers.regC = this->memory->read(this->registers.regSP++);
        this->registers.regB = this->memory->read(this->registers.regSP++);
    };

    this->opCodes[Instruction::PUSH_BC] = [this]()
    {
        this->memory->writeByte(--this->registers.regSP, this->registers.regB);
        this->memory->writeByte(--this->registers.regSP, this->registers.regC);
    };

    this->opCodes[Instruction::PREFIX_CB] = [this]()
    {
        PrefixCB pfxOpcode = static_cast<PrefixCB>(this->memory->read(this->registers.regPC++));
        this->prefixCbOpcodes[pfxOpcode]();
    };

    this->opCodes[Instruction::POP_DE] = [this]()
    {
        this->registers.regE = this->memory->read(this->registers.regSP++);
        this->registers.regD = this->memory->read(this->registers.regSP++);
    };

    this->opCodes[Instruction::PUSH_DE] = [this]()
    {
        this->memory->writeByte(--this->registers.regSP, this->registers.regD);
        this->memory->writeByte(--this->registers.regSP, this->registers.regE);
    };

    this->opCodes[Instruction::POP_HL] = [this]()
    {
        this->registers.regL = this->memory->read(this->registers.regSP++);
        this->registers.regH = this->memory->read(this->registers.regSP++);
    };

    this->opCodes[Instruction::PUSH_HL] = [this]()
    {
        this->memory->writeByte(--this->registers.regSP, this->registers.regH);
        this->memory->writeByte(--this->registers.regSP, this->registers.regL);
    };

    this->opCodes[Instruction::POP_AF] = [this]()
    {
        this->registers.regF.set(this->memory->read(this->registers.regSP++));
        this->registers.regA = this->memory->read(this->registers.regSP++);
        this->registers.regF.resetLowerFourBits();
    };

    this->opCodes[Instruction::PUSH_AF] = [this]()
    {
        this->memory->writeByte(--this->registers.regSP, this->registers.regA);
        this->memory->writeByte(--this->registers.regSP, this->registers.regF.toByte());
    };
}
