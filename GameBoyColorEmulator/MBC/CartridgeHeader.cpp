//
// Created by Fabrizio Paino on 2024-02-16.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "CartridgeHeader.h"

CartridgeHeader::CartridgeHeader(const std::vector<uint8_t> &romData) {
    headerData.assign(romData.begin() + 0x0100, romData.begin() + 0x0150);
}

uint16_t CartridgeHeader::getEntryPoint() const {
    return (headerData[0x100] << 8) | headerData[0x101];
}

std::string CartridgeHeader::getTitle() const {
    std::string title(reinterpret_cast<const char*>(&headerData[0x034]));
    return title;
}

std::string CartridgeHeader::getManufacturerCode() const {
    std::string code;
    for (size_t i = 0x03F; i <= 0x042; ++i) {
        code += headerData[i];
    }
    return code;
}

uint8_t CartridgeHeader::getCGBFlag() const {
    return headerData[0x043];
}

uint8_t CartridgeHeader::getSGBFlag() const {
    return headerData[0x046];
}

uint8_t CartridgeHeader::getCartridgeType() const {
    return headerData[0x047];
}

uint8_t CartridgeHeader::getROMSize() const {
    return headerData[0x048];
}

uint8_t CartridgeHeader::getRAMSize() const {
    return headerData[0x049];
}

uint8_t CartridgeHeader::getDestinationCode() const {
    return headerData[0x04A];
}

uint8_t CartridgeHeader::getOldLicenseeCode() const {
    return headerData[0x04B];
}

uint8_t CartridgeHeader::getHeaderChecksum() const {
    return headerData[0x04D];
}

uint16_t CartridgeHeader::getGlobalChecksum() const {
    return (headerData[0x04E] << 8) | headerData[0x04F];
}
