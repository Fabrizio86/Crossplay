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

#endif //CROSSPLAY_DEFINITIONS_H
