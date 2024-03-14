//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "Bus.h"
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

#include <iostream>

uint8_t Bus::read(ui16 address)
{
    if (InRange(address, 0x0, 0x7FFF))
    {
        if (InRange(address, 0x0, 0xFF) && this->bootRomEnabled)return this->bootRom[address];
        return this->mbc->read(address);
    }
    else if (InRange(address, VRAM_ADDRESS, 0x9FFF))
    {
        const int bankIndex = (lcdControl.toByte() >> 4) & 1;
        return this->vram.read(address - CARTRIDGE_ROM_SIZE, bankIndex);
    }
    else if (InRange(address, CARTRIDGE_ADDRESS, 0xBFFF)) return this->mbc->read(address);
    else if (InRange(address, WORK_RAM_BASE, 0xCFFF)) return this->wram.read(address - WORK_RAM_BASE, 0);
    else if (InRange(address, 0xD000, 0xDFFF))
    {
        this->wramBank = this->wramBank == 0 ? 1 : this->wramBank;
        return this->wram.read(address - WORK_RAM_BASE, this->wramBank);
    }
    else if (InRange(address, ECHO_RAM_BASE, 0xFDFF)) return this->read(address - 0x2000);
    else if (InRange(address, OAM_ADDR, 0xFE9F)) return this->oam.read(address - OAM_ADDR, 0);
    else if (InRange(address, FORBIDDEN_ADDR, 0xFEFF))return INV_ADDR;
    else if (InRange(address, IO_ADDR, 0xFF7F)) return this->ioRead(address);
    else if (InRange(address, HI_RAM_ADDR, 0xFFFE)) return this->hram.read(address - 0xFF80, 0);
    else if (address == 0xFFFF)
    {
        uint8_t val;
        memcpy(&val, &iFlags, sizeof(uint8_t));
        return val;
    }
    else std::cout << "What are you trying to read? No address " << address << " here!" << std::endl;
}

ui8 Bus::ioRead(ui16 address)
{
    if (address == 0xFF00) return this->joypadRegister;
    if (address == 0xFF01) return this->serial.read();
    if (address == 0xFF02 || address == 0xFF03) return INV_ADDR;
    if (address == 0xFF04) return this->timer.getDivider();
    if (address == 0xFF05) return this->timer.getTimerCounter();
    if (address == 0xFF06) return this->timer.getTimerModulo();
    if (address == 0xFF07) return this->timer.getTimerControl();
    if (InRange(address, 0xFF08, 0xFF0E))return INV_ADDR;
    if (address == 0xFF0F) return this->iFlags.toByte();
    if (InRange(address, 0xFF10, 0xFF3F)) return INV_ADDR;
    if (address == 0xFF40) return this->lcdControl;
    if (address == 0xFF41) return this->lcdStatus;
    if (address == 0xFF42) return this->SCY;
    if (address == 0xFF43) return this->SCX;
    if (address == 0xFF44) return this->line;
    if (address == 0xFF45) return this->lyCompare;
    if (address == 0xFF46) return INV_ADDR;
    if (address == 0xFF47) return this->backgroundPalette;
    if (address == 0xFF48) return this->spritePalette[0];
    if (address == 0xFF49) return this->spritePalette[1];
    if (address == 0xFF4A) return this->WY;
    if (address == 0xFF4B) return this->WX;
    if (address == 0xFF4C) return INV_ADDR;
    if (address == 0xFF4D) return 0x0;
    if (InRange(address, 0xFF4E, 0xFF4F)) return INV_ADDR;
    if (address == 0xFF50) return this->bootRomEnabled;
    if (InRange(address, 0xFF51, 0xFF6F)) return INV_ADDR;
    if (address == 0xFF70) return this->wramBank;
    if (InRange(address, 0xFF71, 0xFF7F)) return INV_ADDR;
    return INV_ADDR;
}

int8_t Bus::readSigned(ui16 address)
{
    return static_cast<int8_t>(this->read(address));
}

void Bus::writeByte(ui16 address, uint8_t value)
{
    if (InRange(address, 0x0000, 0x7fff))
    {
        this->mbc->write(address, value);
    }

    else if (InRange(address, VRAM_ADDRESS, 0x9FFF))
    {
        const int bankIndex = (lcdControl >> 4) & 1;
        this->vram.write(address - CARTRIDGE_ROM_SIZE, bankIndex, value);
    }

    // write to External RAM
    else if (InRange(address, CARTRIDGE_ADDRESS, 0xBFFF))
    {
        this->mbc->write(address, value);
    }

    else if (InRange(address, WORK_RAM_BASE, 0xCFFF))
    {
        this->wram.write(address - WORK_RAM_BASE, 0, value);
    }

    else if (InRange(address, 0xD000, 0xDFFF))
    {
        this->wramBank = this->wramBank == 0 ? 1 : this->wramBank;
        this->wram.write(address - WORK_RAM_BASE, this->wramBank, value);
    }

    else if (InRange(address, ECHO_RAM_BASE, 0xFDFF))
    {
        this->writeByte(address - 0x2000, value);
    }

    // write to oam
    else if (InRange(address, OAM_ADDR, 0xFE9F))
    {
        this->oam.write(address - OAM_ADDR, 0, value);
    }

    // write to blocked address
    else if (InRange(address, FORBIDDEN_ADDR, 0xFEFF))
    {
        std::cout << "Write attempt to unusable memory " << address << std::endl;
    }

    // write to io ports
    else if (InRange(address, IO_ADDR, 0xFF7F))
    {
        this->ioWrite(address, value);
    }

    // write to high ram
    else if (InRange(address, HRAM_ADDR, 0xFFFE))
    {
        this->hram.write(address - 0xFF80, 0, value);
    }

    // write to interrupt flags
    else if (address == 0xFFFF)
    {
        std::cout << "Interrupt flag: " << std::endl <<
            " - Joypad: " << (iFlags.Joypad ? "True" : "False") << std::endl <<
            " - Serial: " << (iFlags.Serial ? "True" : "False") << std::endl <<
            " - Timer: " << (iFlags.Timer ? "True" : "False") << std::endl <<
            " - VBlank: " << (iFlags.VBlank ? "True" : "False") << std::endl <<
            " - LCD:" << (iFlags.LCD ? "True" : "False") << std::endl
            << "------------------------------" << std::endl;

        memcpy(&iFlags, &value, sizeof(InterruptFlags));
    }
    else
    {
        std::cout << "Where are you going!? no address here! " << std::hex << address << std::endl;
    }
}

uint16_t Bus::readWord(ui16 address)
{
    uint16_t word = (read(address) << 8) | read(address + 1);
    return word;
}

void Bus::writeWord(ui16 address, uint16_t value)
{
    writeByte(address, value >> 8);
    writeByte(address + 1, value & 0xFF);
}

void Bus::ioWrite(ui16 address, uint8_t value)
{
    if (address == 0xFF00) this->joypadRegister = value;
    else if (address == 0xFF01) this->serial.write(value);
    else if (address == 0xFF02) this->serial.writeControl(value);
    else if (address == 0xFF03) return;
    else if (address == 0xFF04) this->timer.resetDivider();
    else if (address == 0xFF05) this->timer.setTimerCounter(value);
    else if (address == 0xFF06) this->timer.setTimerModulo(value);
    else if (address == 0xFF07) this->timer.setTimerControl(value);
    else if (InRange(address, 0xFF08, 0xFF0E))return;
    else if (address == 0xFF0F) memcpy(&this->iFlags, &value, 1);
    else if (InRange(address, 0xFF10, 0xFF3F)) return;
    else if (address == 0xFF40) this->lcdControl = value;
    else if (address == 0xFF41) this->lcdStatus = value;
    else if (address == 0xFF42) this->SCY = value;
    else if (address == 0xFF43) this->SCX = value;
    else if (address == 0xFF44) this->line = value;
    else if (address == 0xFF45) this->lyCompare = value;
    else if (address == 0xFF46) this->dmaTransfer(value);
    else if (address == 0xFF47) this->backgroundPalette = value;
    else if (address == 0xFF48) this->spritePalette[0] = value;
    else if (address == 0xFF49) this->spritePalette[1] = value;
    else if (address == 0xFF4A) this->WY = value;
    else if (address == 0xFF4B) this->WX = value;
    else if (InRange(address, 0xFF4C, 0xFF4F)) return;
    else if (address == 0xFF50) this->bootRomEnabled = value & 0x1;
    else if (InRange(address, 0xFF51, 0xFF6F)) return;
    else if (address == 0xFF70) this->wramBank = value & 0x03;
    else
    {
        std::cout << "write to unmapped io at address: " << address << std::endl;
    }
}

Bus::Bus(InterruptController* ic) : ic(ic)
{
    this->ram.setSize(1, INTERNAL_RAM_SIZE);
    this->wram.setSize(8, WORK_RAM_SIZE);
    this->vram.setSize(2, VIDEO_RAM_SIZE);
    this->oam.setSize(1, OAM_SIZE);
    this->hram.setSize(1, HRAM_SIZE);
    this->bootRomEnabled = true;

    // Initialize other member variables
    joypadRegister = 0;
    memset(&iFlags, 0, sizeof(InterruptFlags));

    bootRom = {
        0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
        0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
        0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
        0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
        0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
        0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
        0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
        0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
        0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
        0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
        0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
        0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
        0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
        0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
        0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x00, 0x00, 0x23, 0x7D, 0xFE, 0x34, 0x20,
        0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x00, 0x00, 0x3E, 0x01, 0xE0, 0x50
    };
}

// todo: this needs to become part of a rom class
std::vector<char> createRam(std::vector<char> romData)
{
    uint8_t ramSizeByte = romData[0x149];
    size_t ramSize;

    switch (ramSizeByte)
    {
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

    return std::vector<char>(ramSize, 0);
}

// todo: this needs to become part of a rom class
void Bus::createMBC()
{
    auto romData = this->cartridge.rom_data();

    uint8_t mbcType = romData[0x147];
    std::vector<char> ramData = createRam(romData);

    switch (mbcType)
    {
    case 0x00:
        // ROM ONLY
        this->mbc = std::make_unique<NoMBC>(romData);
        break;
    case 0x01:
    case 0x02:
    case 0x03:
        // MBC1
        this->mbc = std::make_unique<MBC1>(romData, ramData);
        return;
    case 0x05:
    case 0x06:
        // MBC2
        this->mbc = std::make_unique<MBC2>(romData, ramData);
        return;
    case 0x0B:
    case 0x0C:
    case 0x0D:
        // MMM01
        //return std::make_unique<MMM01>(romData, ramData);
        throw std::runtime_error("Unknown MBC type");
    case 0x0F:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
        // MBC3
        this->mbc = std::make_unique<MBC3>(romData, ramData);
        return;
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
        // MBC5
        this->mbc = std::make_unique<MBC5>(romData, ramData);
        return;
    case 0x20:
        // MBC6
        this->mbc = std::make_unique<MBC6>(romData, ramData);
        return;
    case 0x22:
        // MBC7
        this->mbc = std::make_unique<MBC7>(romData, ramData);
        return;
    case 0xFC:
    // POCKET CAMERA
    //return std::make_unique<PocketCamera>(romData, ramData);
    case 0xFD:
    // BANDAI TAMA5
    //return std::make_unique<BandaiTama5>(romData, ramData);
    case 0xFE:
    // HuC3
    //return std::make_unique<HuC3>(romData, ramData);
    case 0xFF:
        // HuC1+RAM+BATTERY
        //return std::make_unique<HuC1>(romData, ramData);
    default:
        throw std::runtime_error("Unknown MBC type");
    }
}

void Bus::dmaTransfer(const ui8 byte)
{
    const ui16 startAddress = byte * 0x100;

    for (ui8 i = 0x0; i <= 0x9F; i++)
    {
        const ui16 fromAddress = startAddress + i;
        const ui16 toAddress = OAM_ADDR + i;

        const ui8 value = read(fromAddress);
        this->writeByte(toAddress, value);
    }
}

void Bus::loadRom(std::string path)
{
    this->cartridge.load(path);

    this->createMBC();
    this->loadPaletteData();
}

void Bus::loadPaletteData()
{
    const auto romData = this->cartridge.rom_data();

    // Check if ROM data has the required size to hold palette information
    if (romData.size() < 0x6C)
    {
        throw std::runtime_error("ROM data does not contain palette information");
    }

    // Load background palette data
    std::copy(romData.begin() + 0x68, romData.begin() + 0x6C, ioPorts + 0x68);

    // Load sprite palette data
    std::copy(romData.begin() + 0x6C, romData.begin() + 0x70, ioPorts + 0x6C);
}
