//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_DEFINITIONS_H
#define CROSSPLAY_DEFINITIONS_H

#include <functional>

using uint = unsigned int;
using OpcodeHandler = std::function<void()>;
using uchar = unsigned char;
using ISRFunctionPtr = std::function<void()>;

using ui8 = uint8_t;
using DataBuffer = std::vector<char>;

enum LogicalOperation
{
    AND,
    OR,
    NOT,
    XOR
};

#endif //CROSSPLAY_DEFINITIONS_H
