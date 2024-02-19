//
// Created by Fabrizio Paino on 2024-02-15.
//

#ifndef CROSSPLAY_MBC1_H
#define CROSSPLAY_MBC1_H

#include <vector>
#include "../../Interfaces/IMBC.h"

class MBC1 : public IMBC {
public:
    MBC1(const std::vector<uint8_t>& romData, const std::vector<uint8_t> &ramData);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

private:
    std::vector<uint8_t> rom; // The entire ROM frameBuffer
    std::vector<uint8_t> ram; // The RAM frameBuffer
    uint8_t currentRomBank; // The currently selected ROM bank
    uint8_t currentRamBank; // The currently selected RAM bank
    bool romBankingMode; // The current mode (true for ROM banking mode, false for RAM banking mode)
    bool ramEnabled; // Whether the RAM is currently enabled
};


#endif //CROSSPLAY_MBC1_H
