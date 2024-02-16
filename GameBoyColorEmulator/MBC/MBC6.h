//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC6_H
#define CROSSPLAY_MBC6_H


#include <cstdint>
#include <vector>
#include "../../Interfaces/IMBC.h"

class MBC6 : public IMBC {
public:
    MBC6(const std::vector<uint8_t> &romData, const std::vector<uint8_t> &ramData);
    
    uint8_t read(uint16_t address);

    void write(uint16_t address, uint8_t data);

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;

    uint32_t romBank;
    uint32_t ramBank;
    bool ramEnabled;
    bool rtcEnabled;
};


#endif //CROSSPLAY_MBC6_H
