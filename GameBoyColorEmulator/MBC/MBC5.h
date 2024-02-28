//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC5_H
#define CROSSPLAY_MBC5_H

#include <vector>
#include "../../Interfaces/IMBC.h"
#include "../../Definitions.h"

class MBC5 : public IMBC {
public:
    MBC5(const DataBuffer &romData, const DataBuffer &ramData);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

private:
    DataBuffer rom;
    DataBuffer ram;
    uint16_t romBank;
    uint8_t ramBank;
    bool ramEnabled;
};

#endif //CROSSPLAY_MBC5_H
