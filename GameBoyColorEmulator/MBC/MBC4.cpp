//
// Created by Fabrizio Paino on 2024-02-16.
//
#include "MBC4.h"
#include "../Consts.h"

MBC4::MBC4(const DataBuffer &romData, const DataBuffer &ramData)
        : rom(romData), ram(ramData), romBank(1), ramBank(0), ramEnabled(false), mode(Mode::RomBanking) {
}

uint8_t MBC4::read(uint16_t address) {
    if (address < 0x4000) {
        return rom[address]; // Read from ROM bank 0
    } else if (address >= 0x4000 && address < 0x8000) {
        uint32_t adjustedAddress = ((romBank - 1) * ROM_BANK_SIZE) + (address - 0x4000);
        return rom[adjustedAddress]; // Read from selected ROM bank
    } else if (address >= 0xA000 && address < 0xC000) {
        if (ramEnabled) {
            uint32_t adjustedAddress = (ramBank * RAM_BANK_SIZE) + (address - 0xA000);
            return ram[adjustedAddress]; // Read from RAM bank
        } else {
            return 0xFF; // RAM is not enabled, return default value
        }
    }
    return 0xFF; // Default return value
}

void MBC4::write(uint16_t address, uint8_t data) {
    if (address < 0x2000) {
        ramEnabled = ((data & 0x0F) == 0x0A); // Enable/disable RAM
    } else if (address >= 0x2000 && address < 0x4000) {
        if (mode == Mode::RomBanking) {
            romBank = data & 0x0F; // Select lower 4 bits of ROM bank
            if (romBank == 0x00) {
                romBank = 0x01; // Ensure ROM bank is at least 1
            }
        } else {
            ramBank = data & 0x03; // Select lower 2 bits of RAM bank
        }
    } else if (address >= 0x6000 && address < 0x8000) {
        mode = (data & 0x01) ? Mode::RamBanking : Mode::RomBanking; // Switch between ROM and RAM banking modes
    } else if (address >= 0xA000 && address < 0xC000) {
        if (ramEnabled) {
            uint32_t adjustedAddress = (ramBank * RAM_BANK_SIZE) + (address - 0xA000);
            ram[adjustedAddress] = data; // Write to RAM bank
        }
    }
}
