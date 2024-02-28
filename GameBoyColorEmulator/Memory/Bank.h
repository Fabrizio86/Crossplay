//
// Created by Fabrizio Paino on 2024-02-27.
//

#ifndef CROSSPLAY_BANK_H
#define CROSSPLAY_BANK_H

#include <vector>

class Bank {
public:
    void resize(size_t size);

    void write(uint16_t address, uint8_t value);

    uint8_t read(uint16_t address);

private:
    std::vector<uint8_t> data;
};

#endif //CROSSPLAY_BANK_H
