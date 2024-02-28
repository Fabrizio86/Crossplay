//
// Created by Fabrizio Paino on 2024-02-27.
//

#ifndef CROSSPLAY_MEMORY_H
#define CROSSPLAY_MEMORY_H

#include "Bank.h"

class Memory {
public:
    Memory(size_t bankNumbers, size_t bankSize);

    void write(uint8_t address, uint8_t bankIndex, uint8_t value);

    uint8_t read(uint8_t address, uint8_t bankIndex);

private:
    std::vector<Bank> banks;
};


#endif //CROSSPLAY_MEMORY_H
