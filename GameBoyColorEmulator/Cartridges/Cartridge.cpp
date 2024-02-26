//
// Created by Fabrizio Paino on 2024-02-25.
//

#include "Cartridge.h"

#include <fstream>

Cartridge::Cartridge(std::string path)
{
    std::ifstream romFile(path, std::ios::binary);
    if (!romFile.is_open())
    {
        throw std::exception();
    }

    // Determine file size
    romFile.seekg(0, std::ios::end);
    std::streampos fileSize = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    // Read ROM data into a vector
    std::vector<uint8_t> romData(fileSize);
    romFile.read(reinterpret_cast<char*>(romData.data()), fileSize);
    romFile.close();

    memcpy(&this->header.title, &romData[0x134], 16);
    this->header.type = romData[0x147];
    this->header.cgbFlag = romData[0x143];
    this->header.sgbCode = romData[0x146];
    this->header.romSize = romData[0x148];
    this->header.ramSize = romData[0x149];
}
