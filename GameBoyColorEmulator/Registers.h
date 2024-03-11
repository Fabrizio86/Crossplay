//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_REGISTERS_H
#define CROSSPLAY_REGISTERS_H

#include <cstdint>

#include "Consts.h"
#include "../Definitions.h"

// Flag register
struct Flags
{
    bool zero = false;
    bool subtract = false;
    bool halfCarry = false;
    bool carry = false;
    bool unused1 = false;
    bool unused2 = false;
    bool unused3 = false;
    bool unused4 = false;

    void set(uint8_t byte)
    {
        zero     = byte & FLAG_Z;
        subtract = byte & FLAG_N;
        halfCarry= byte & FLAG_H;
        carry    = byte & FLAG_C;
        unused1  = byte & 0x08;
        unused2  = byte & 0x04;
        unused3  = byte & 0x02;
        unused4  = byte & 0x01;
    }

    uint8_t toByte() const
    {
        uint8_t byte = 0;
        byte |= (zero     ? FLAG_Z : 0);
        byte |= (subtract ? FLAG_N : 0);
        byte |= (halfCarry? FLAG_H : 0);
        byte |= (carry    ? FLAG_C : 0);
        byte |= (unused1  ? 0x08 : 0);
        byte |= (unused2  ? 0x04 : 0);
        byte |= (unused3  ? 0x02 : 0);
        byte |= (unused4  ? 0x01 : 0);
        return byte;
    }

    void reset()
    {
        zero = false;
        subtract = false;
        halfCarry = false;
        carry = false;
        unused1 = false;
        unused2 = false;
        unused3 = false;
        unused4 = false;
    }

    void resetLowerFourBits()
    {
        unused1 = false;
        unused2 = false;
        unused3 = false;
        unused4 = false;
    }
};

struct Registers
{
    uint8_t regB;
    uint8_t regC;
    uint8_t regD;
    uint8_t regE;
    uint8_t regH;
    uint8_t regL;
    uint8_t regA;
    Flags regF;

    // Special-purpose registers
    uint16_t regPC; // Program Counter
    uint16_t regSP; // Stack Pointer

    void reset()
    {
        regA =
            regB =
            regC =
            regD =
            regE =
            regH =
            regL = 0;
        this->regF.reset();
        regSP = INITIAL_SP_ADDRESS;
        regPC = INITIAL_ROM_ADDRESS;
    }
};

#endif //CROSSPLAY_REGISTERS_H
