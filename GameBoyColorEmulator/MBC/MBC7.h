//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC7_H
#define CROSSPLAY_MBC7_H

#include <cstdint>
#include <vector>
#include "../../Interfaces/IMBC.h"


class MBC7 : public IMBC {
public:
    MBC7(const std::vector<uint8_t> &romData, const std::vector<uint8_t> &ramData);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t value) override;

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;
    uint8_t romBank;
    uint8_t ramBank;
    bool ramEnabled;
    bool rtcEnabled;
};


#endif //CROSSPLAY_MBC7_H
