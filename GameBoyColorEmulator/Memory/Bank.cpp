//
// Created by Fabrizio Paino on 2024-02-27.
//

#include "Bank.h"

void Bank::write(uint16_t address, uint8_t value) {
    this->data[address] = value;
}

uint8_t Bank::read(uint16_t address) {
    return this->data[address];
}

void Bank::resize(size_t size) {
    this->data.resize(size);
}
