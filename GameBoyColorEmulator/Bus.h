//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_BUS_H
#define CROSSPLAY_BUS_H

#include "../Interfaces/IMemory.h"
#include "../Definitions.h"
#include "../Interfaces/IMBC.h"
#include "Memory/Memory.h"

#include "Consts.h"
#include "Interrupts/InterruptController.h"

#include <string>

#include "Timer.h"
#include "Cartridges/Cartridge.h"

class Bus final : public IMemory
{
public:
    Bus(InterruptController* ic);

    ~Bus() override = default;

    uint8_t read(uint32_t address) override;

    void writeByte(uint32_t address, uint8_t value) override;

    uint16_t readWord(uint16_t address) override;

    void writeWord(uint16_t address, uint16_t value) override;

    void loadRom(std::string path);

    void performDMA(uint8_t address);

private:
    std::unique_ptr<IMBC> mbc; // The memory bank controller
    InterruptController* ic;
    Cartridge cartridge;

    // todo: these line below need cleaning up
    Memory ram;
    Memory wram;
    Memory vram;
    Memory oam;
    Memory hram;
    Timer timer;


    std::array<uint8_t, 256> bootRom;
    InterruptFlags iFlags;

    uint8_t ioPorts[IO_REGISTER_SIZE];
    uint8_t lcdControl;
    uint8_t bgpIndex;
    uint8_t obpIndex;
    uint8_t autoIncrement;

    uint16_t serialTransfer;
    uint32_t timerAndDivider;

    uint16_t bgPaletteData[8][2];
    uint16_t objPaletteData[8][2];

    bool ramEnabled;
    bool bootRomEnabled = true;

    void loadPaletteData();
    void createMBC();

    // I/O state
    uint8_t joypadRegister = 0xFF; // Joypad register, stored at address 0xFF00
    bool buttonDown = false; // Current state of the "down" button
    bool buttonUp = false; // Current state of the "up" button
    bool buttonLeft = false; // Current state of the "left" button
    bool buttonRight = false; // Current state of the "right" button
    bool buttonStart = false; // Current state of the "start" button
    bool buttonSelect = false; // Current state of the "select" button
    bool buttonB = false; // Current state of the "B" button
    bool buttonA = false; // Current state of the "A" button

private:
    void ioWrite(uint32_t address, uint8_t value);
};


#endif //CROSSPLAY_BUS_H
