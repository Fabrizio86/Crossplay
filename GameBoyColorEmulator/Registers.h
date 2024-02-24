//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_REGISTERS_H
#define CROSSPLAY_REGISTERS_H

#include <cstdint>

#include "Consts.h"

// Flag register
struct Flags {
    bool zero: 1;
    bool subtract: 1;
    bool halfCarry: 1;
    bool carry: 1;
};

struct Registers {
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
    uint16_t regPC;  // Program Counter
    uint16_t regSP;  // Stack Pointer

    Registers() {
        this->reset();
    }

    void reset() {
        memset(this, 0, sizeof(Registers));
        regA = 0x01;
        regSP = INITIAL_SP_ADDRESS;
        regPC = INITIAL_ROM_ADDRESS;
    }
};

#endif //CROSSPLAY_REGISTERS_H
