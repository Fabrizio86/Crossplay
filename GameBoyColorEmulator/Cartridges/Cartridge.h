//
// Created by Fabrizio Paino on 2024-02-25.
//
#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <vector>

#include "CartridgeHeader.h"

class Cartridge {
public:
    Cartridge(std::string path);

private:
    std::vector<u_int8_t> data;
    CartridgeHeader header;
};

#endif //CARTRIDGE_H
