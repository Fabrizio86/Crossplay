//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC6_H
#define CROSSPLAY_MBC6_H

#include <cstdint>
#include <vector>
#include "../../Interfaces/IMBC.h"
#include "../../Definitions.h"

class MBC6 : public IMBC
{
public:
    MBC6(const DataBuffer& romData, const DataBuffer& ramData);

    uint8_t read(uint16_t address);

    void write(uint16_t address, uint8_t data);

private:
    DataBuffer rom;
    DataBuffer ram;

    uint32_t romBank;
    uint32_t ramBank;
    bool ramEnabled;
    bool rtcEnabled;
};


#endif //CROSSPLAY_MBC6_H
