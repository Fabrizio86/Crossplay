//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_REGISTERS_H
#define CROSSPLAY_REGISTERS_H

#include <cstdint>

#include "Consts.h"

// Flag register
struct Flags
{
    bool zero = false;
    bool subtract = false;
    bool halfCarry = false;
    bool carry = false;
};

struct Registers
{
    // General-purpose registers
    uint8_t regB;
    // General-purpose registers
    uint8_t regC;
    // General-purpose registers
    uint8_t regD;
    // General-purpose registers
    uint8_t regE;
    // General-purpose registers
    uint8_t regH;
    // General-purpose registers
    uint8_t regL;
    // General-purpose registers
    uint8_t regA;
    // General-purpose registers
    uint8_t regF;

    // Special-purpose registers
    uint16_t regPC; // Program Counter
    uint16_t regSP; // Stack Pointer

    void reset()
    {
        memset(this, 0, sizeof(Registers));
        regA = 0x1;
        regSP = INITIAL_SP_ADDRESS;
        regPC = INITIAL_ROM_ADDRESS;
    }
};

#endif //CROSSPLAY_REGISTERS_H
