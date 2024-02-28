//
// Created by Fabrizio Paino on 2024-02-16.
//

#include "mbc2.h"

MBC2::MBC2(const DataBuffer &romData, const DataBuffer &ramData)
        : rom(romData), ram(ramData), romBank(1), ramEnabled(false) {}

uint8_t MBC2::read(uint16_t address) {
    if (address < 0x4000) {
        // Read from ROM bank 0
        return rom[address];
    } else if (address < 0x8000) {
        // Read from switchable ROM bank
        uint32_t adjustedAddress = (romBank % numRomBanks) * ROM_BANK_SIZE + (address - 0x4000);
        return rom[adjustedAddress];
    } else if (address >= 0xA000 && address < 0xA200 && ramEnabled) {
        // Read from RAM
        return ram[address - 0xA000];
    } else {
        // Undefined behavior for other addresses
        return 0xFF;
    }
}

void MBC2::write(uint16_t address, uint8_t data) {
    if (address < 0x2000) {
        // Enable/disable RAM
        if ((address & 0x0100) == 0) {
            ramEnabled = (data == 0x0A);
        }
    } else if (address >= 0x2000 && address < 0x4000) {
        // Select ROM bank
        if ((address & 0x0100) != 0) {
            romBank = data & 0x0F;
            if (romBank == 0) {
                romBank = 1; // ROM bank 0 is not accessible, so it's mapped to bank 1
            }
        }
    } else if (address >= 0xA000 && address < 0xA200 && ramEnabled) {
        // Write to RAM
        ram[address - 0xA000] = data & 0x0F; // Only the lower 4 bits are used
    }
}
