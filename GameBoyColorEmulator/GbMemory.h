//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_GBMEMORY_H
#define CROSSPLAY_GBMEMORY_H

#include "../Interfaces/IMemory.h"
#include "../Definitions.h"

#include <string>

class GbMemory final : public IMemory
{
public:
    GbMemory();

    ~GbMemory() override = default;

    uint8_t read(uint32_t address) const override;

    void writeByte(uint32_t address, uint8_t value) override;

    uint16_t readWord(uint16_t address) override;

    void writeWord(uint16_t address, uint16_t value) override;

    void loadRom(std::string path);

private:
    // Memory regions
    uint8_t romBank00[0x4000];
    uint8_t romBanks[0x4000 * 128];  // Support for up to 128 ROM banks
    uint8_t videoRam[0x2000];
    uint8_t externalRam[0x2000 * 4];  // Support for up to 4 RAM banks
    uint8_t workRam[0x2000];
    uint8_t oam[0xA0];
    uint8_t ioPorts[0x80];
    uint8_t hram[0x7F];
    uint8_t interruptEnable;

    // MBC state
    uint8_t currentRomBank;
    uint8_t currentRamBank;
    bool ramEnabled;

    // I/O state
    uint8_t joypadRegister = 0xFF;  // Joypad register, stored at address 0xFF00
    bool buttonDown = false;  // Current state of the "down" button
    bool buttonUp = false;  // Current state of the "up" button
    bool buttonLeft = false;  // Current state of the "left" button
    bool buttonRight = false;  // Current state of the "right" button
    bool buttonStart = false;  // Current state of the "start" button
    bool buttonSelect = false;  // Current state of the "select" button
    bool buttonB = false;  // Current state of the "B" button
    bool buttonA = false;  // Current state of the "A" button
};


#endif //CROSSPLAY_GBMEMORY_H
