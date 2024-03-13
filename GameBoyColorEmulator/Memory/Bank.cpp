//
// Created by Fabrizio Paino on 2024-02-27.
//

#include "Bank.h"

void Bank::write(const uint16_t address, const uint8_t value) {
    this->data[address] = value;
}

uint8_t Bank::read(const uint16_t address) const
{
    return this->data[address];
}

void Bank::resize(const size_t size) {
    this->data.resize(size, 0);
}
