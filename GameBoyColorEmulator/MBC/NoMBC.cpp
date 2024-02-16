//
// Created by Fabrizio Paino on 2024-02-16.
//

#include "NoMBC.h"

NoMBC::NoMBC(const std::vector<uint8_t> &romData) : rom(romData) {}

uint8_t NoMBC::read(uint16_t address) {
    // Check if the address is within the ROM range
    if (address < rom.size()) {
        return rom[address];
    } else {
        // Return a default value if the address is out of range
        return 0xFF; // You can adjust this default value as needed
    }
}

void NoMBC::write(uint16_t address, uint8_t data) {
    // No-op: ROM is read-only
}
