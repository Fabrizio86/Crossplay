//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "GbMemory.h"
#include "Consts.h"
#include "MBC/MBC1.h"

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

void GbMemory::loadRom(const std::vector<uint8_t> &romData, size_t ramSize) {
    mbc = std::make_unique<MBC1>(romData, ramSize);
}
