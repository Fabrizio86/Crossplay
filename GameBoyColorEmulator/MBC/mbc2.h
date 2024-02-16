//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC2_H
#define CROSSPLAY_MBC2_H

#include <vector>
#include "../../Interfaces/IMBC.h"

class MBC2 : public IMBC {
public:
    MBC2(const std::vector<uint8_t> &romData, const std::vector<uint8_t> &ramData);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;
    bool ramEnabled;
    uint8_t romBank;

    static constexpr size_t ROM_BANK_SIZE = 16 * 1024; // 16 KB
    static constexpr size_t numRomBanks = 32; // Total number of ROM banks supported
};

#endif //CROSSPLAY_MBC2_H
