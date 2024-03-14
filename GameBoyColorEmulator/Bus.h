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

#include "Serial.h"
#include "Timer.h"
#include "Cartridges/Cartridge.h"

class Bus final : public IMemory
{
public:
    Bus(InterruptController* ic);

    ~Bus() override = default;

    uint8_t read(ui16 address) override;

    int8_t readSigned(ui16 address) override;

    void writeByte(ui16 address, uint8_t value) override;

    uint16_t readWord(ui16 address) override;

    void writeWord(ui16 address, uint16_t value) override;

    void loadRom(std::string path);

private:
    bool ramEnabled;
    bool bootRomEnabled;

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
    Serial serial;

    std::array<ui8, 256> bootRom;
    InterruptFlags iFlags;

    ui8 ioPorts[IO_REGISTER_SIZE];
    ui8 lcdControl;
    ui8 lcdStatus;
    ui8 line;
    ui8 lyCompare;
    ui8 SCX;
    ui8 SCY;
    ui8 WX;
    ui8 WY;
    ui8 backgroundPalette;
    ui8 spritePalette[2];
    ui8 wramBank;

    ui16 serialTransfer;
    uint32_t timerAndDivider;

    ui16 bgPaletteData[8][2];
    ui16 objPaletteData[8][2];

    uint8_t joypadRegister = 0xFF;

private:
    void ioWrite(ui16 address, uint8_t value);

    ui8 ioRead(ui16 address);

    void loadPaletteData();

    void createMBC();

    void dmaTransfer(const ui8 byte);
};


#endif //CROSSPLAY_BUS_H
