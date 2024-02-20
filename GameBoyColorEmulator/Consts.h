//
// Created by Fabrizio Paino on 2024-02-13.
//

#ifndef CONSTS_H
#define CONSTS_H

#include <cstdint>

constexpr uint16_t INITIAL_ROM_ADDRESS = 0x0100;

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
static constexpr size_t OAM_SIZE = 0xA0;
static constexpr size_t OAM_ADDR = 0xFE00;
static constexpr int MAX_SPRITES = 40;
static constexpr int MBC_TYPE = 0x147;

// Timing constants (values are approximate and may need adjustment)
static constexpr int H_BLANK_CYCLES = 204; // Number of cycles in H-Blank phase
static constexpr int V_BLANK_CYCLES = 4560; // Number of cycles in V-Blank phase
static constexpr int ACTIVE_RENDER_CYCLES = 70224; // Number of cycles in active rendering phase (includes mode 2 and 3)
static constexpr int FRAME_CYCLES = ACTIVE_RENDER_CYCLES + V_BLANK_CYCLES; // Total number of cycles in one frame

static constexpr int SCANLINE_WIDTH = 160;
static constexpr int NUM_SCANLINES_PER_FRAME = 144;

// Define display mode constants
static constexpr uint8_t DISPLAY_MODE_VBLANK = 0x01; // Value indicating V-Blank mode
static constexpr uint8_t DISPLAY_MODE_HBLANK = 0x00; // Value indicating H-Blank mode

// Define interrupt flag constants
static constexpr uint8_t INT_FLAG_VBLANK = 0x01; // Value indicating V-Blank interrupt flag
constexpr uint16_t REG_IE_ADDRESS = 0xFFFF;
constexpr uint8_t INT_VBLANK_ENABLE = 0x01;

#endif //CONSTS_H
