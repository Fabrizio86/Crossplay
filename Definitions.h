//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_DEFINITIONS_H
#define CROSSPLAY_DEFINITIONS_H

#include <functional>

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

using uint = unsigned int;
using OpcodeHandler = std::function<void()>;
using uchar = unsigned char;
using ISRFunctionPtr = std::function<void()>;

using ui8 = uint8_t;
using ui16 = uint16_t;
using DataBuffer = std::vector<char>;
using ScreenBuffer = std::vector<std::vector<uint32_t>>;

enum LogicalOperation
{
    AND,
    OR,
    NOT,
    XOR
};

enum class Condition {
    NZ,
    Z,
    NC,
    C,
};

bool CheckBit(const ui8 value, const ui8 bit);

static bool InRange(ui8 val, ui16 low, ui16 high)
{
    return val >= low && val <= high;
}

static bool InRange(ui16 val, ui16 low, ui16 high)
{
    return val >= low && val <= high;
}

#endif //CROSSPLAY_DEFINITIONS_H
