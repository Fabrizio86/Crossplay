//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC7_H
#define CROSSPLAY_MBC7_H

#include <cstdint>
#include <vector>
#include "../../Interfaces/IMBC.h"
#include "../../Definitions.h"

class MBC7 : public IMBC
{
public:
    MBC7(const DataBuffer& romData, const DataBuffer& ramData);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t value) override;

private:
    DataBuffer rom;
    DataBuffer ram;
    uint8_t romBank;
    uint8_t ramBank;
    bool ramEnabled;
    bool rtcEnabled;
};


#endif //CROSSPLAY_MBC7_H
