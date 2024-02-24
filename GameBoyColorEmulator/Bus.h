//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_BUS_H
#define CROSSPLAY_BUS_H

#include "../Interfaces/IMemory.h"
#include "../Definitions.h"
#include "../Interfaces/IMBC.h"
#include "Consts.h"
#include "InterruptController.h"

#include <string>

class Bus final : public IMemory {
public:
    Bus(InterruptController *ic);

    ~Bus() override = default;

    uint8_t read(uint32_t address) const override;

    void writeByte(uint32_t address, uint8_t value) override;

    uint16_t readWord(uint16_t address) override;

    void writeWord(uint16_t address, uint16_t value) override;

    void loadRom(const std::vector<uint8_t> &romData);

    void performDMA(uint8_t address);

private:
    std::unique_ptr<IMBC> mbc; // The memory bank controller
    InterruptController *ic;

    uint8_t videoRam[CARTRIDGE_RAM_SIZE];
    uint8_t workRam[CARTRIDGE_RAM_SIZE];
    uint8_t oam[MAX_SPRITES];
    uint8_t ioPorts[IO_REGISTER_SIZE];
    uint8_t hRam[HRAM_SIZE];
    InterruptFlags iFlags;

    bool ramEnabled;

    void loadPaletteData(const std::vector<uint8_t> &romData);

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


#endif //CROSSPLAY_BUS_H
