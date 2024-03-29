//
// Created by Fabrizio Paino on 2024-02-25.
//

#include "Cartridge.h"

#include <fstream>

#include <iostream>

void Cartridge::load(std::string path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    romData.resize(size, 0);
    file.read(reinterpret_cast<char*>(romData.data()), size);

    memcpy(&this->headerInfo.title, &romData[0x134], 16);
    this->headerInfo.type = romData[0x147];
    this->headerInfo.cgbFlag = romData[0x143];
    this->headerInfo.sgbCode = romData[0x146];
    this->headerInfo.romSize = romData[0x148];
    this->headerInfo.ramSize = romData[0x149];

    uint8_t entryPointBytes[2];
    entryPointBytes[0] = romData[0x104];
    entryPointBytes[1] = romData[0x105];

    this->headerInfo.entryPoint = (entryPointBytes[1] << 8) | entryPointBytes[0];

    std::cout << "entry point: " << std::hex << this->headerInfo.entryPoint << std::endl;
}

std::vector<char> Cartridge::rom_data() const
{
    return romData;
}

CartridgeHeader Cartridge::header() const
{
    return headerInfo;
}
