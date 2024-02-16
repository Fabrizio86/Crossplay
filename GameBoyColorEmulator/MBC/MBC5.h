//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC5_H
#define CROSSPLAY_MBC5_H

#include <vector>
#include "../../Interfaces/IMBC.h"

class MBC5 : public IMBC {
public:
    MBC5(const std::vector<uint8_t> &romData, const std::vector<uint8_t> &ramData);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;
    uint16_t romBank;
    uint8_t ramBank;
    bool ramEnabled;
};

#endif //CROSSPLAY_MBC5_H
