//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_IMEMORY_H
#define CROSSPLAY_IMEMORY_H

#include "../Definitions.h"

class IMemory {
public:
    virtual ~IMemory() = default;

    virtual uint8_t read(ui16 address) = 0;

    virtual int8_t readSigned(ui16 address) = 0;

    virtual void writeByte(ui16 address, uint8_t value) = 0;

    virtual uint16_t readWord(ui16 address) = 0;

    virtual void writeWord(ui16 address, uint16_t value) = 0;
};

#endif //CROSSPLAY_IMEMORY_H


