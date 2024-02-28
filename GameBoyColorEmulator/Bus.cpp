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

uint8_t Bus::read(uint32_t address) const
{
    if (address <= 0x00FF)
    {
        return this->bootRom[address];
    }
    else if (address < 0x8000)
    {
        // ROM bank 00~NN
        return mbc->read(address);
    }
    else if (address < 0xA000)
    {
        // 8KB Video RAM
        return videoRam[address - 0x8000];
    }
    else if (address < 0xC000)
    {
        // 8KB External RAM
        return mbc->read(address);
    }
    else if (address < 0xE000)
    {
        // 8KB Work RAM
        return workRam[address - 0xC000];
    }
    else if (address < 0xFE00)
    {
        // 8KB Work RAM (shadow)
        return workRam[address - 0xE000];
    }
    else if (address < 0xFEA0)
    {
        // Sprite attribute table (OAM)
        return oam[address - 0xFE00];
    }
    else if (address < 0xFF00)
    {
        // Not usable
        return 0xff;
    }
    else if (address < 0xFF80)
    {
        // I/O ports
        if (address == 0xFF00)
        {
            // Joypad register
            return joypadRegister;
        }
        else if (address == 0xFF40)
        {
            return this->lcdControl;
        }
        else
        {
            return ioPorts[address - 0xFF00];
        }
    }
    else if (address < 0xFFFF)
    {
        // High RAM (HRAM)
        return hRam[address - 0xFF80];
    }
    else
    {
        // Interrupt Enable register
        uint8_t val;
        memcpy(&val, &iFlags, sizeof(uint8_t));
        return val;
    }
}

/**
 * @brief Writes a byte to the specified address in the Bus memory.
 *
 * @param address The address to write the byte to.
 * @param value The value to write.
 */
void Bus::writeByte(uint32_t address, uint8_t value)
{
    // write from rom
    if (address < CARTRIDGE_ROM_SIZE)
    {
        this->mbc->write(address, value);
    }

    // write to video ram
    else if (address < 0xA000)
    {
        const int bankIndex = (lcdControl >> 4) & 1;
        this->vram.write(address - CARTRIDGE_ROM_SIZE, bankIndex, value);
    }

    // write to External RAM
    else if (address < WORK_RAM_BASE)
    {
        mbc->write(address, value);
    }

    // write to work ram
    else if (address < ECHO_RAM_BASE)
    {
        const int bankIndex = (lcdControl >> 3) & 1;
        this->wram.write(address - WORK_RAM_BASE, bankIndex, value);
    }

    // write to mirror
    else if (address < 0xFE00)
    {
        this->writeByte(address - 0x2000, value);
    }

    // write to oam
    else if (address < 0xFEA0)
    {
        this->oam.write(address - OAM_ADDR, 0, value);
    }

    // write to blocked address
    else if (address < 0xFF00)
    {
        std::cout << "Write attempt to unusable memory " << address << std::endl;
    }

    // write to io ports
    else if (address < 0xFF80)
    {
        this->ioWrite(address, value);
    }

    // write to high ram
    else if (address < 0xFFFF)
    {
        this->hram.write(address - 0xFF80, 0, value);
    }

    // write to interrupt flags
    else if (address == 0xFFFF)
    {
        std::cout << "Interrupt flag: " << (int)value << std::endl;

        memcpy(&iFlags, &value, sizeof(InterruptFlags));
    }
}

uint16_t Bus::readWord(uint16_t address)
{
    uint16_t word = (read(address) << 8) | read(address + 1);
    return word;
}

void Bus::writeWord(uint16_t address, uint16_t value)
{
    writeByte(address, value >> 8);
    writeByte(address + 1, value & 0xFF);
}

void Bus::ioWrite(uint32_t address, uint8_t value)
{
    switch (address)
    {
    case 0xFF00:
        break;
    }


    if (address == 0xFF46)
    {
        DMA_VALUE = value;
    }

    else if (address == 0xFF68)
    {
        this->bgpIndex = (value & 0x3F);
        this->autoIncrement = value >> 7;
    }
    else if (address == 0xFF6A)
    {
        this->obpIndex = (value & 0x3F);
        this->autoIncrement = value >> 7;
    }
    else if (address >= 0xFF69 && address <= 0xFF6F)
    {
        if (address % 2 == 0)
        {
            this->bgPaletteData[this->bgpIndex][0] = value; // Write to the high intensity byte of the selected background palette entry
        }
        else
        {
            this->bgPaletteData[this->bgpIndex][1] = value; // Write to the low intensity byte of the selected background palette entry
        }

        if (this->autoIncrement)
        {
            this->bgpIndex = (this->bgpIndex + 1) % 8; // Increment the background palette index cyclically
        }
    }
    else if (address >= 0xFF6A && address <= 0xFF6F)
    {
        if (address % 2 == 0)
        {
            this->objPaletteData[this->obpIndex][0] = value; // Write to the high intensity byte of the selected object palette entry
        }
        else
        {
            this->objPaletteData[this->obpIndex][1] = value; // Write to the low intensity byte of the selected object palette entry
        }

        if (this->autoIncrement)
        {
            this->obpIndex = (this->obpIndex + 1) % 8; // Increment the object palette index cyclically
        }
    }
    else if (address < 0xFF80)
    {
        // I/O ports
        if (address == 0xFF00)
        {
            // Joypad register
            this->joypadRegister = value;
        }
        else if (address < 0xff3)
        {
        }
        else if (address < 0xff08)
        {
        }
        else if (address < 0xff27)
        {
        }
        else if (address == 0xFF40)
        {
            std::cout << "writing to lcd" << std::endl;

            this->lcdControl = value;
            this->ic->requestInterrupt(InterruptType::LCD_STAT);
        }
        else
        {
            this->ioPorts[address - 0xFF00] = value;
        }
    }
}

Bus::Bus(InterruptController* ic) : ic(ic)
{
    this->ram.setSize(1, INTERNAL_RAM_SIZE);
    this->wram.setSize(2, WORK_RAM_SIZE);
    this->vram.setSize(2, VIDEO_RAM_SIZE);
    this->oam.setSize(1, OAM_SIZE);
    this->hram.setSize(1, HRAM_SIZE);

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

void Bus::loadRom(std::string path)
{
    this->cartridge.load(path);
    INITIAL_ROM_ADDRESS = this->cartridge.header().entryPoint;

    this->createMBC();
    this->loadPaletteData();
}

void Bus::loadPaletteData()
{
    auto romData = this->cartridge.rom_data();

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

void Bus::performDMA(uint8_t address)
{
    memcpy(oam, &videoRam[address], 160);
}
