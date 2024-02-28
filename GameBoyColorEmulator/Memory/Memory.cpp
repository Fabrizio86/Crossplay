//
// Created by Fabrizio Paino on 2024-02-27.
//

#include "Memory.h"

Memory::Memory(size_t bankNumbers, size_t bankSize) {
    this->banks.resize(bankNumbers);
}

uint8_t Memory::read(uint8_t address, uint8_t bankIndex) {
    return this->banks[bankIndex].read(address);
}

void Memory::write(uint8_t address, uint8_t bankIndex, uint8_t value) {
    this->banks[bankIndex].write(address, value);
}
