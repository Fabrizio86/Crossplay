//
// Created by Fabrizio Paino on 2024-02-15.
//

#ifndef CROSSPLAY_MBC1_H
#define CROSSPLAY_MBC1_H

#include <vector>
#include "../../Interfaces/IMBC.h"
#include "../../Definitions.h"

class MBC1 : public IMBC {
public:
    MBC1(const DataBuffer& romData, const DataBuffer &ramData);

    ui8 read(uint16_t address) override;

    void write(uint16_t address, ui8 data) override;

private:
    DataBuffer rom; // The entire ROM frameBuffer
    DataBuffer ram; // The RAM frameBuffer
    ui8 currentRomBank; // The currently selected ROM bank
    ui8 currentRamBank; // The currently selected RAM bank
    bool romBankingMode; // The current mode (true for ROM banking mode, false for RAM banking mode)
    bool ramEnabled; // Whether the RAM is currently enabled
};


#endif //CROSSPLAY_MBC1_H
