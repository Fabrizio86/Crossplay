//
// Created by Fabrizio Paino on 2024-02-16.
//
#include "MBC7.h"

MBC7::MBC7(const DataBuffer &romData, const DataBuffer &ramData)
        : rom(romData), ram(ramData), romBank(1), ramBank(0), ramEnabled(false), rtcEnabled(false) {
}

uint8_t MBC7::read(uint16_t address) {
    if (address < 0x4000) {
        // Read from ROM bank 0
        return rom[address];
    } else if (address < 0x8000) {
        // Read from selected ROM bank
        uint32_t romAddress = (romBank * 0x4000) + (address - 0x4000);
        return rom[romAddress];
    } else if (address >= 0xA000 && address < 0xC000) {
        // Read from RAM if enabled
        if (ramEnabled) {
            uint32_t ramAddress = (ramBank * 0x2000) + (address - 0xA000);
            return ram[ramAddress];
        } else {
            // RAM is not enabled, return a default value
            return 0xFF;
        }
    } else {
        return 0xFF;
    }
}

void MBC7::write(uint16_t address, uint8_t value) {
    if (address < 0x2000) {
        // Enable/disable RAM
        ramEnabled = ((value & 0x0F) == 0x0A);
    } else if (address >= 0x2000 && address < 0x4000) {
        // Select ROM bank
        romBank = (romBank & 0x0100) | value;
    } else if (address >= 0x4000 && address < 0x6000) {
        // Select RAM bank
        ramBank = value & 0x0F;
    } else if (address >= 0xA000 && address < 0xC000) {
        // Write to RAM if enabled
        if (ramEnabled) {
            uint32_t ramAddress = (ramBank * 0x2000) + (address - 0xA000);
            ram[ramAddress] = value;
        }
    }
}

