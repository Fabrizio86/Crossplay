//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC4_H
#define CROSSPLAY_MBC4_H

#include <vector>
#include "../../Interfaces/IMBC.h"

class MBC4 : public IMBC {
public:
    MBC4(const std::vector<uint8_t>& romData, const std::vector<uint8_t>& ramData);

    uint8_t read(uint16_t address) override;
    void write(uint16_t address, uint8_t data) override;

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;
    uint8_t romBank;
    uint8_t ramBank;
    bool ramEnabled;
    enum class Mode { RomBanking, RamBanking };
    Mode mode;
};

#endif //CROSSPLAY_MBC4_H
