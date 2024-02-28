//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_NOMBC_H
#define CROSSPLAY_NOMBC_H

#include "../../Interfaces/IMBC.h"
#include <vector>

class NoMBC : public IMBC {
public:
    NoMBC(const std::vector<char> &romData);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

private:
    std::vector<char> rom;
};


#endif //CROSSPLAY_NOMBC_H
