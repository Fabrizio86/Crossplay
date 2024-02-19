//
// Created by Fabrizio Paino on 2024-02-16.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_CARTRIDGEHEADER_H
#define CROSSPLAY_CARTRIDGEHEADER_H

#include <string>
#include <vector>

class CartridgeHeader {
public:
    CartridgeHeader(const std::vector<uint8_t> &romData);

    // Get the entry point
    uint16_t getEntryPoint() const;

    // Get the title
    std::string getTitle() const;

    // Get the manufacturer code
    std::string getManufacturerCode() const;

    // Get the CGB flag
    uint8_t getCGBFlag() const;

    // Get the SGB flag
    uint8_t getSGBFlag() const;

    // Get the cartridge type
    uint8_t getCartridgeType() const;

    // Get the ROM size
    uint8_t getROMSize() const;

    // Get the RAM size
    uint8_t getRAMSize() const;

    // Get the destination code
    uint8_t getDestinationCode() const;

    // Get the old licensee code
    uint8_t getOldLicenseeCode() const;

    // Get the SGB flag
    uint8_t getHeaderChecksum() const;

    // Get the global checksum
    uint16_t getGlobalChecksum() const;

private:
    std::vector<uint8_t> headerData;
};


#endif //CROSSPLAY_CARTRIDGEHEADER_H
