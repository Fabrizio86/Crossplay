//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbMemory.h"
#include "Consts.h"
#include "MBC/MBC1.h"
#include "MBC/NoMBC.h"
#include "MBC/mbc2.h"
#include "MBC/MBC3.h"
#include "MBC/MBC4.h"
#include "MBC/MBC5.h"
#include "MBC/MBC6.h"
#include "MBC/MBC7.h"

#include <fstream>

uint8_t GbMemory::read(uint32_t address) const {
    if (address < 0x8000) {
        // ROM bank 00~NN
        return mbc->read(address);
    } else if (address < 0xA000) {
        // 8KB Video RAM
        return videoRam[address - 0x8000];
    } else if (address < 0xC000) {
        // 8KB External RAM
        return mbc->read(address);
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
        return 0xff;
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
        // ROM bank 00~NN
        mbc->write(address, value);
    } else if (address < 0xA000) {
        // 8KB Video RAM
        videoRam[address - 0x8000] = value;
    } else if (address < 0xC000) {
        // 8KB External RAM
        mbc->write(address, value);
    } else if (address < 0xE000) { // 8KB Work RAM
        workRam[address - 0xC000] = value;
    } else if (address < 0xFE00) { // 8KB Work RAM (shadow)
        workRam[address - 0xE000] = value;
    } else if (address < 0xFEA0) { // Sprite attribute table (OAM)
        oam[address - 0xFE00] = value;
    } else if (address < 0xFF00) { // Not usable
        // Not writable, do nothing
    } else if (address < 0xFF80) { // I/O ports
        if (address == 0xFF00) { // Joypad register
            joypadRegister = value;
        } else {
            ioPorts[address - 0xFF00] = value;
        }
    } else if (address < 0xFFFF) { // High RAM (HRAM)
        hram[address - 0xFF80] = value;
    } else { // Interrupt Enable register
        interruptEnable = value;
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

// todo: this needs to become part of a rom class
std::vector<uint8_t> createRam(const std::vector<uint8_t>& romData) {
    uint8_t ramSizeByte = romData[0x149];
    size_t ramSize;

    switch (ramSizeByte) {
        case 0x00:
            ramSize = 0;
            break;
        case 0x01:
            ramSize = 2 * 1024;
            break;
        case 0x02:
            ramSize = 8 * 1024;
            break;
        case 0x03:
            ramSize = 32 * 1024;
            break;
        case 0x04:
            ramSize = 128 * 1024;
            break;
        case 0x05:
            ramSize = 64 * 1024;
            break;
        default:
            throw std::runtime_error("Unknown RAM size");
    }

    return std::vector<uint8_t>(ramSize, 0);
}

// todo: this needs to become part of a rom class
std::unique_ptr<IMBC> createMBC(const std::vector<uint8_t>& romData) {
    uint8_t mbcType = romData[0x147];
    std::vector<uint8_t> ramData = createRam(romData);

    switch (mbcType) {
        case 0x00:
            // No MBC, ROM only
            return std::make_unique<NoMBC>(romData);
        case 0x01:
        case 0x02:
        case 0x03:
            // MBC1
            return std::make_unique<MBC1>(romData, ramData);
        case 0x05:
        case 0x06:
            // MBC2
            return std::make_unique<MBC2>(romData, ramData);
        case 0x08:
        case 0x09:
            // MBC3
            return std::make_unique<MBC3>(romData, ramData);
        case 0x0B:
        case 0x0C:
        case 0x0D:
            // MBC4
            return std::make_unique<MBC4>(romData, ramData);
        case 0x0F:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            // MBC5
            return std::make_unique<MBC5>(romData, ramData);
        case 0x15:
        case 0x16:
            // MBC6
            return std::make_unique<MBC6>(romData, ramData);
        case 0x17:
            // MBC7
            return std::make_unique<MBC7>(romData, ramData);
        default:
            throw std::runtime_error("Unknown MBC type");
    }
}


void GbMemory::loadRom(const std::vector<uint8_t> &romData) {
    this->mbc = createMBC(romData);

}
