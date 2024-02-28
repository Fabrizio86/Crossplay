//
// Created by Fabrizio Paino on 2024-02-25.
//
#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <vector>

#include "CartridgeHeader.h"

class Cartridge {
public:
    Cartridge() = default;

    void load(std::string path);

    [[nodiscard]] std::vector<char> rom_data() const;

    [[nodiscard]] CartridgeHeader header() const;

private:
    std::vector<char> romData;
    CartridgeHeader headerInfo;
};

#endif //CARTRIDGE_H
