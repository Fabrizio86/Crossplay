//
// Created by Fabrizio Paino on 2024-03-01.
//

#ifndef TILE_H
#define TILE_H

#include "../../Definitions.h"

#include "LcdControl.h"
#include "../Consts.h"

#include <cstdint>

struct Tile
{
    int x, y;
    ScreenBuffer* buffer;
    IMemory* bus;
    uint8_t tileData[8][8];
    LcdControl* lcdControl;
    uint8_t byte1;
    uint8_t byte2;

    void renderPixel()
    {
        unsigned int px, py, absoluteIndex;
        uint8_t colorIndex;
        uint8_t byte1, byte2;
        uint8_t SCX = bus->read(0xFF43);
        uint8_t SCY = bus->read(0xFF42);
        uint8_t WX = bus->read(0xFF4B) - 7;
        uint8_t WY = bus->read(0xFF4A);

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                px = x * 8 + j;
                py = y * 8 + i;
                absoluteIndex = py * 160 + px;

                /// new code

                uint8_t adjustedSCX = (px + SCX) % 0xFF;
                uint8_t adjustedSCY = (py + SCY) % 0xFF;
                uint16_t bgAddr = ((adjustedSCY / 8) & 31) * 32 + ((adjustedSCX / 8) & 31);
                uint8_t bgTileIndex = bus->read(VRAM_ADDRESS + bgAddr);
                uint8_t tileLine = bus->read(VRAM_ADDRESS + bgTileIndex * 16 + ((adjustedSCY & 7) * 2));
                uint8_t bgPixel = ((tileLine >> (7 - (adjustedSCX & 7))) & 1) + (((tileLine >> 8) >> (7 - (adjustedSCX & 7))) & 1) * 2;

                if (lcdControl->WindowEnabled)
                {
                    uint8_t adjustedWX = (px - WX + 7) % 256;
                    uint8_t adjustedWY = (py - WY) % 256;
                    uint16_t winAddr = ((adjustedWY / 8) & 31) + ((adjustedWX / 8) & 31) * 32;
                    uint8_t winTileIndex = bus->read(VRAM_ADDRESS + winAddr);
                    uint8_t tileLine = bus->read(VRAM_ADDRESS + winTileIndex * 16 + ((adjustedWY & 7) * 2));
                    bgPixel = ((tileLine >> (7 - (adjustedWX & 7))) & 1) + (((tileLine >> 8) >> (7 - (adjustedWX & 7))) & 1) * 2;
                }

                this->buffer->at(py)[px] = palette[bgPixel];

                uint8_t spritePixel = 0;
                uint8_t oamAddress = OAM_ADDR;
                for(uint8_t i = 0; i < 40; i++)
                {
                    uint8_t spriteY = bus->read(oamAddress++) - 16;
                    uint8_t spriteX = bus->read(oamAddress++) - 8;
                    uint8_t tileNum = bus->read(oamAddress++);
                    uint8_t attributes = bus->read(oamAddress++);

                    if (!(attributes & 0x80)) {
                        // Normal sprite, 8x8 tile
                        if (!(py < spriteY || py >= spriteY + 8)) {
                            uint8_t tileRow = py - spriteY;
                            uint8_t tileDataAddress = 0x8000 + tileNum * 16 + tileRow * 2;
                            uint8_t tileLine = bus->read(tileDataAddress);
                            spritePixel = ((tileLine >> (7 - ((px - spriteX) & 7))) & 1) + (((tileLine >> 8) >> (7 - ((px - spriteX) & 7))) & 1) * 2;
                        }
                    } else {
                        // Y-flip enabled sprite, 8x8 tile
                        if (!(py < spriteY || py >= spriteY + 8)) {
                            uint8_t tileRow = 7 - (py - spriteY);
                            uint8_t tileDataAddress = 0x8000 + tileNum * 16 + tileRow * 2;
                            uint8_t tileLine = bus->read(tileDataAddress);
                            spritePixel = ((tileLine >> (7 - ((px - spriteX) & 7))) & 1) + (((tileLine >> 8) >> (7 - ((px - spriteX) & 7))) & 1) * 2;
                        }
                    }

                    if(spritePixel)
                        this->buffer->at(py)[px] = palette[spritePixel];
                }
            }
        }
    };
};

struct ScreenTiles
{
    ScreenTiles(ScreenBuffer* buffer, IMemory* memory, LcdControl* lcdControl): tiles{}
    {
        for (int y = 0; y < 18; ++y)
        {
            for (int x = 0; x < 20; ++x)
            {
                this->tiles[y][x].buffer = buffer;
                this->tiles[y][x].bus = memory;
                this->tiles[y][x].lcdControl = lcdControl;
                this->tiles[y][x].x = x;
                this->tiles[y][x].y = y;
            }
        }
    }

    Tile tiles[18][20];
};

#endif //TILE_H
