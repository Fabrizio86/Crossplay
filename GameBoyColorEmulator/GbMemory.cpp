//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbMemory.h"
#include "Consts.h"

#include <fstream>

uint8_t GbMemory::read(uint32_t address) const {
    if (address < 0x4000) {
        // 16KB ROM bank 00
        return romBank00[address];
    } else if (address < 0x8000) {
        // 16KB ROM bank 01~NN
        return romBanks[currentRomBank * ROM_BANK_SIZE + (address - 0x4000)];
    } else if (address < 0xA000) {
        // 8KB Video RAM
        return videoRam[address - 0x8000];
    } else if (address < 0xC000) {
        // 8KB External RAM
        if (ramEnabled) {
            return externalRam[currentRamBank * RAM_BANK_SIZE + (address - 0xA000)];
        } else {
            return 0xFF;  // Accessing disabled RAM returns 0xFF
        }
    } else if (address < 0xE000) {
        // 8KB Work RAM
        return workRam[address - 0xC000];
    } else if (address < 0xFE00) {
        // 8KB Work RAM (shadow)
        return workRam[address - 0xE000];
    } else if (address < 0xFEA0) {
        // Sprite attribute table (OAM)
        return oam[address - 0xFE00];
    } else if (address < 0xFF00) {
        // Not usable
        return 0;
    } else if (address < 0xFF80) {
        // I/O ports
        if (address == 0xFF00) {
            // Joypad register
            return joypadRegister;
        } else {
            return ioPorts[address - 0xFF00];
        }
    } else if (address < 0xFFFF) {
        // High RAM (HRAM)
        return hram[address - 0xFF80];
    } else {
        // Interrupt Enable register
        return interruptEnable;
    }
}

void GbMemory::writeByte(uint32_t address, uint8_t value) {
    if (address < 0x8000) {
        // Write to ROM bank - typically ignored on Game Boy
    } else if (address >= 0xA000 && address < 0xC000) {
        cartridgeRAM[address - 0xA000] = value; // Write to cartridge RAM
    } else if (address >= 0xC000 && address < 0xE000) {
        internalRam[address - 0xC000] = value; // Write to internal RAM
    } else if (address >= 0xE000 && address < 0xFE00) {
        internalRam[address - 0xE000] = value; // Write to echo RAM
    } else if (address >= 0xFE00 && address < 0xFEA0) {
        ioRegisters[address - 0xFE00] = value; // Write to I/O registers
    } else if (address >= 0xFF00 && address < 0xFF80) {
        // Implement logic for writing to memory-mapped I/O registers
    } else if (address >= 0xFF80 && address < 0xFFFF) {
        hRAM[address - 0xFF80] = value; // Write to HRAM
    } else if (address == 0xFFFF) {
        interruptRegisters[address - 0xFFFF] = value; // Write to interrupt registers
    } else {
        // Implement logic for other memory regions
    }
}

uint16_t GbMemory::readWord(uint16_t address) {
    uint16_t word = (read(address) << 8) | read(address + 1);
    return word;
}

void GbMemory::writeWord(uint16_t address, uint16_t value) {
    // Write the high byte
    writeByte(address, value >> 8);
    // Write the low byte
    writeByte(address + 1, value & 0xFF);
}

GbMemory::GbMemory() {}

void GbMemory::loadRom(std::string path) {
    std::ifstream rom_file(path, std::ios::binary | std::ios::ate);
    if (!rom_file.is_open()) {
        throw std::runtime_error("Failed to open ROM file.");
    }

    size_t size = rom_file.tellg();
    if (size > ROM_SIZE) {
        throw std::runtime_error("ROM file size exceeds maximum allowed size.");
    }

    rom_file.seekg(0, std::ios::beg);
    std::vector<uint8_t> rom_data(size);
    rom_file.read(reinterpret_cast<char *>(rom_data.data()), size);
    rom_file.close();

    std::copy(rom_data.begin(), rom_data.end(), memory + INITIAL_ROM_ADDRESS);
}
