//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_NOMBC_H
#define CROSSPLAY_NOMBC_H

#include "../../Interfaces/IMBC.h"
#include <vector>

class NoMBC : public IMBC {
public:
    NoMBC(const std::vector<uint8_t> &romData);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

private:
    std::vector<uint8_t> rom;
};


#endif //CROSSPLAY_NOMBC_H
