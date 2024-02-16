//
// Created by Fabrizio Paino on 2024-02-15.
//

#include "MBC1.h"
#include "../Consts.h"

uint8_t MBC1::read(uint16_t address) {
    // ROM Bank 00 (Read Only)
    if (address < 0x4000) {
        return rom[address];
    } else if (address < 0x8000) {
        uint32_t adjustedAddress;
        if (romBankingMode) {
            // In ROM banking mode, use the current ROM bank
            adjustedAddress = currentRomBank * ROM_BANK_SIZE + (address - 0x4000);
        } else {
            // In RAM banking mode, use the lower 5 bits of the current ROM bank
            adjustedAddress = (currentRomBank & 0x1F) * ROM_BANK_SIZE + (address - 0x4000);
        }
        return rom[adjustedAddress];
    } else if (address >= 0xA000 && address < 0xC000) {
        if (!ramEnabled) {
            // If the RAM is not enabled, return a default value
            return 0xFF;
        }
        uint32_t adjustedAddress = currentRamBank * RAM_BANK_SIZE + (address - 0xA000);
        return ram[adjustedAddress];
    }

    return 0xff;
}

void MBC1::write(uint16_t address, uint8_t data) {
    if (address < 0x2000) {
        ramEnabled = (data & 0x0F) == 0x0A;
    } else if (address < 0x4000) {
        currentRomBank = data & 0x1F;
        if (currentRomBank == 0) currentRomBank = 1;
        else if (currentRomBank == 0x20 || currentRomBank == 0x40 || currentRomBank == 0x60) currentRomBank++;
    } else if (address < 0x6000) {
        if (romBankingMode) {
            currentRomBank = (currentRomBank & 0x1F) | ((data & 0x03) << 5);
        } else {
            currentRamBank = data & 0x03;
            if (currentRamBank > 3) currentRamBank = 3;
        }
    } else if (address < 0x8000) {
        romBankingMode = (data == 0);
    } else if (address >= 0xA000 && address < 0xC000) {
        if (ramEnabled) {
            uint32_t adjustedAddress = currentRamBank * RAM_BANK_SIZE + (address - 0xA000);
            ram[adjustedAddress] = data;
        }
    }
}

MBC1::MBC1(const std::vector<uint8_t> &romData, const std::vector<uint8_t> &ramData)
        : rom(romData), ram(ramData),
          currentRomBank(1), currentRamBank(0),
          romBankingMode(true), ramEnabled(false) {}
