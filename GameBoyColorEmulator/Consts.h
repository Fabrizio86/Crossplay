//
// Created by Fabrizio Paino on 2024-02-13.
//

#ifndef CONSTS_H
#define CONSTS_H

#include <cstdint>

constexpr uint32_t INITIAL_ROM_ADDRESS = 0x08000000;

static constexpr size_t ROM_BANK_SIZE = 16 * 1024;          // 16 KB
static constexpr size_t RAM_BANK_SIZE = 8 * 1024;           // 8 KB
static constexpr size_t INTERNAL_RAM_SIZE = 8 * 1024;       // 8 KB
static constexpr size_t IO_REGISTER_SIZE = 0x80;            // 128 bytes
static constexpr size_t VIDEO_RAM_SIZE = 8 * 1024;          // 8 KB
static constexpr size_t CARTRIDGE_ROM_SIZE = 0x8000;        // 32 KB
static constexpr size_t CARTRIDGE_RAM_SIZE = 0x2000;        // 8 KB
static constexpr size_t HRAM_SIZE = 0x7F;                   // 127 bytes
static constexpr size_t SOUND_REGISTERS_SIZE = 0x100;       // 256 bytes
static constexpr size_t TIMER_REGISTERS_SIZE = 0x04;        // 4 bytes
static constexpr size_t LCD_REGISTERS_SIZE = 0x60;          // 96 bytes
static constexpr size_t INTERRUPT_REGISTERS_SIZE = 0x04;    // 4 bytes


#endif //CONSTS_H
