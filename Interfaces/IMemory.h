//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_IMEMORY_H
#define CROSSPLAY_IMEMORY_H

#include <cstdint>

class IMemory {
public:
    virtual ~IMemory() = default;

    virtual uint8_t read(uint32_t address) = 0;

    virtual int8_t readSigned(uint32_t address) = 0;

    virtual void writeByte(uint32_t address, uint8_t value) = 0;

    virtual uint16_t readWord(uint32_t address) = 0;

    virtual void writeWord(uint32_t address, uint16_t value) = 0;
};

#endif //CROSSPLAY_IMEMORY_H


