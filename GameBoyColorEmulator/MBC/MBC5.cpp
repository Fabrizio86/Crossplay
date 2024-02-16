//
// Created by Fabrizio Paino on 2024-02-16.
//

#include "MBC5.h"

#include "MBC5.h"

MBC5::MBC5(const std::vector<uint8_t>& romData, const std::vector<uint8_t>& ramData)
        : rom(romData), ram(ramData), romBank(1), ramBank(0), ramEnabled(false) {}

uint8_t MBC5::read(uint16_t address) {
    if (address < 0x4000) {
        return rom[address]; // Read from fixed ROM bank 0
    } else if (address < 0x8000) {
        return rom[romBank * 0x4000 + (address - 0x4000)]; // Read from selected ROM bank
    } else if (address >= 0xA000 && address < 0xC000) {
        if (ramEnabled) {
            return ram[ramBank * 0x2000 + (address - 0xA000)]; // Read from selected RAM bank
        } else {
            return 0xFF; // RAM disabled, return default value
        }
    }
    return 0xFF; // Invalid address range, return default value
}

void MBC5::write(uint16_t address, uint8_t data) {
    if (address < 0x2000) {
        ramEnabled = (data & 0x0F) == 0x0A; // Enable/disable RAM based on lower 4 bits of data
    } else if (address >= 0x2000 && address < 0x3000) {
        romBank = (romBank & 0x100) | data; // Set lower 8 bits of ROM bank number
    } else if (address >= 0x3000 && address < 0x4000) {
        romBank = ((data & 0x01) << 8) | (romBank & 0xFF); // Set bit 9 of ROM bank number
    } else if (address >= 0x4000 && address < 0x6000) {
        ramBank = data & 0x0F; // Set RAM bank number
    }
    // Note: MBC5 doesn't have any memory-mapped control registers beyond address 0x6000
}
