//
// Created by Fabrizio Paino on 2024-02-27.
//

#include "Memory.h"

uint8_t Memory::read(const uint8_t address, const uint8_t bankIndex)
{
    return this->banks[bankIndex].read(address);
}

void Memory::write(const uint8_t address, const uint8_t bankIndex, const uint8_t value)
{
    this->banks[bankIndex].write(address, value);
}

void Memory::setSize(const size_t bankNumbers, const size_t bankSize)
{
    this->banks.resize(bankNumbers);

    for (auto bank : this->banks)
    {
        bank.resize(bankSize);
    }
}
