//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_GBMEMORY_H
#define CROSSPLAY_GBMEMORY_H

#include "../Interfaces/IMemory.h"
#include "../Definitions.h"
#include "../Interfaces/IMBC.h"

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

    void loadRom(const std::vector<uint8_t>& romData);

private:
    std::unique_ptr<IMBC> mbc; // The memory bank controller

    uint8_t videoRam[0x2000];
    uint8_t workRam[0x2000];
    uint8_t oam[0xA0];
    uint8_t ioPorts[0x80];
    uint8_t hram[0x7F];
    uint8_t interruptEnable;

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
