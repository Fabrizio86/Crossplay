//
// Created by Fabrizio Paino on 2024-02-13.
//

#ifndef CONSTS_H
#define CONSTS_H

#include "SFML/Graphics.hpp"
#include <cstdint>

static constexpr uint16_t INITIAL_ROM_ADDRESS = 0x000;
static constexpr uint16_t INITIAL_SP_ADDRESS = 0xFFFE;

static constexpr size_t ROM_BANK_SIZE = 16 * 1024;
static constexpr size_t RAM_BANK_SIZE = 8 * 1024;
static constexpr size_t INTERNAL_RAM_SIZE = 8 * 1024;
static constexpr size_t IO_REGISTER_SIZE = 0x80;
static constexpr size_t VIDEO_RAM_SIZE = 8 * 1024;
static constexpr size_t WORK_RAM_SIZE = 4 * 1024;
static constexpr size_t CARTRIDGE_ROM_SIZE = 0x8000;
static constexpr size_t VRAM_ADDRESS = 0x8000;
static constexpr size_t CARTRIDGE_ADDRESS = 0xA000;
static constexpr size_t CARTRIDGE_RAM_SIZE = 0x2000;
static constexpr size_t HRAM_SIZE = 0x7F;
static constexpr size_t SOUND_REGISTERS_SIZE = 0x100;
static constexpr size_t TIMER_REGISTERS_SIZE = 0x04;
static constexpr size_t LCD_REGISTERS_SIZE = 0x60;
static constexpr size_t INTERRUPT_REGISTERS_SIZE = 0x04;
static constexpr size_t ECHO_RAM_BASE = 0xE000;
static constexpr size_t WORK_RAM_BASE = 0xC000;
static constexpr size_t OAM_SIZE = 0xA0;
static constexpr size_t OAM_ADDR = 0xFE00;
static constexpr size_t FORBIDDEN_ADDR = 0xFEA0;
static constexpr size_t IO_ADDR = 0xFF00;
static constexpr size_t HRAM_ADDR = 0xFF00;
static constexpr size_t HI_RAM_ADDR = 0xFF80;
static constexpr size_t INV_ADDR = 0xFF;
static constexpr size_t LCD_STATUS_ADDR = 0xFF41;
static constexpr size_t LY_COMPARE_ADDR = 0xFF45;

static constexpr int MAX_SPRITES = 40;
static constexpr int MBC_TYPE = 0x147;

static constexpr int OAM_SEARCH_CYCLES = 80;
static constexpr int VRAM_ACCESS_CYCLES = 172;
static constexpr int CLOCKS_PER_SCANLINE = 456;


typedef struct
{
    unsigned VBlank : 1;
    unsigned LCD : 1;
    unsigned Timer : 1;
    unsigned Serial : 1;
    unsigned Joypad : 1;
    unsigned Unused1 : 1;
    unsigned Unused2 : 1;
    unsigned Unused3 : 1;

    ui8 toByte() const
    {
        return (Unused3 << 7) |
            (Unused2 << 6) |
            (Unused1 << 5) |
            (Joypad << 4) |
            (Serial << 3) |
            (Timer << 2) |
            (LCD << 1) |
            VBlank;
    }

    void fromByte(const ui8 byte)
    {
        Unused3 = (byte >> 7) & 1U;
        Unused2 = (byte >> 6) & 1U;
        Unused1 = (byte >> 5) & 1U;
        Joypad = (byte >> 4) & 1U;
        Serial = (byte >> 3) & 1U;
        Timer = (byte >> 2) & 1U;
        LCD = (byte >> 1) & 1U;
        VBlank = byte & 1U;
    }
} InterruptFlags;

static constexpr int H_BLANK_CYCLES = 204;
static constexpr int V_BLANK_CYCLES = 4560;
static constexpr int ACTIVE_RENDER_CYCLES = 70224;
static constexpr int FRAME_CYCLES = ACTIVE_RENDER_CYCLES + V_BLANK_CYCLES;

static constexpr int SCANLINE_WIDTH = 160;
static constexpr int SCANLINE_HEIGHT = 144;
static constexpr int TILE_DIMENSION = 8;
static constexpr int NUM_TILES_PER_ROW = SCANLINE_WIDTH / TILE_DIMENSION;

static constexpr int WINDOW_X = 0;
static constexpr int WINDOW_Y = 0;

// Define display mode constants
static constexpr uint8_t DISPLAY_MODE_VBLANK = 0x01;
static constexpr uint8_t DISPLAY_MODE_HBLANK = 0x00;

// Define interrupt flag constants
static constexpr uint8_t INT_FLAG_VBLANK = 0x01;
static constexpr uint16_t REG_IE_ADDRESS = 0xFFFF;
static constexpr uint8_t INT_VBLANK_ENABLE = 0x01;
static constexpr uint16_t DMA_SOURCE_ADDRESS = 0x8000;
static constexpr uint16_t WINDOW_TILE_MAP_ADDRESS = 0x9800;

static const int PALETTE_SIZE = 4;

static const uint32_t palette[PALETTE_SIZE] = {
    0xFF686868, // Color 0: Dark gray
    0xFFA8A8A8, // Color 1: Light gray
    0xFF101010, // Color 2: Black
    0xFFFFFFFF // Color 3: White
};

struct OamEntry
{
    uint8_t y;
    uint8_t x;
    uint8_t tile;

    unsigned cgbPn : 3;
    unsigned cgbVramBank : 1;
    unsigned pn : 1;
    unsigned xflp : 1;
    unsigned yflp : 1;
    unsigned bgp : 1;
};

enum class VideoMode {
    ACCESS_OAM,
    ACCESS_VRAM,
    HBLANK,
    VBLANK,
};

#endif //CONSTS_H
