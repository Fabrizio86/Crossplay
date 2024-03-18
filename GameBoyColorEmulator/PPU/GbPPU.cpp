//
// Created by Fabrizio Paino on 2024-02-19.
//

#include "GbPPU.h"
#include "../Consts.h"
#include "SFML/Graphics.hpp"

GbPPU::GbPPU(IMemory* memory, InterruptController* controller) : memory(memory),
                                                                 controller(controller),
                                                                 screenBuffer(SCANLINE_WIDTH, std::vector<uint32_t>(SCANLINE_HEIGHT, 0))
{
}

void GbPPU::displayToWindow()
{
    // Clear the window
    window->clear();

    sf::VertexArray pixels(sf::Points, SCANLINE_WIDTH * SCANLINE_HEIGHT);

    for (std::size_t y = 0; y < SCANLINE_HEIGHT; ++y)
        for (std::size_t x = 0; x < SCANLINE_WIDTH; ++x)
        {
            std::size_t idx = y * SCANLINE_WIDTH + x;
            pixels[idx].position = sf::Vector2f(x, y);
            pixels[idx].color.a = screenBuffer[y][x] >> 24;
            pixels[idx].color.r = screenBuffer[y][x] >> 16;
            pixels[idx].color.g = screenBuffer[y][x] >> 8;
            pixels[idx].color.b = screenBuffer[y][x];
        }

    window->draw(pixels);
    window->display();
}

void GbPPU::exec()
{
    this->lcdStatus = memory->read(LCD_STATUS_ADDR);

    switch (this->mode)
    {
    case VideoMode::ACCESS_OAM:
        if (this->x >= 80)
        {
            this->lcdStatus |= 3UL;
            this->memory->writeByte(LCD_STATUS_ADDR, this->lcdStatus);
            this->mode = VideoMode::ACCESS_VRAM;
            x = 0;
        }
        else
        {
            int oamIndex = this->x / 2;
            int oamByteIndex = oamIndex * 4;
            int width = 8;
            int height = 16;
            OamEntry entry;

            if ((this->x % 2) == 0)
            {
                entry.y = this->memory->read(OAM_ADDR + oamByteIndex);
                entry.x = this->memory->read(OAM_ADDR + oamByteIndex + 1);
                entry.tile = this->memory->read(OAM_ADDR + oamByteIndex + 2);
                entry.setFlags(this->memory->read(OAM_ADDR + oamByteIndex + 3));

                for (int yOffset = 0; yOffset < height; ++yOffset)
                {
                    for (int xOffset = 0; xOffset < width; ++xOffset)
                    {
                        int xCopy = entry.x - width + xOffset;
                        int yCopy = entry.y - height + yOffset;

                        // Check bounds
                        if (xCopy >= 0 && xCopy < SCANLINE_WIDTH && yCopy >= 0 && yCopy < SCANLINE_HEIGHT)
                        {
                            oamGrid[yCopy][xCopy] = &entry;
                        }
                    }
                }
            }
        }
        break;

    case VideoMode::ACCESS_VRAM:
        if (this->x <= 160)
        {
            this->renderPixel(y, x);
        }

        if (this->x >= 172)
        {
            this->x = 0;
            this->mode = VideoMode::HBLANK;

            if (this->lcdStatus & (1 << 3))
            {
                this->controller->requestInterrupt(InterruptType::HBlank);
            }

            bool lyCoincidence = this->memory->read(LY_COMPARE_ADDR) == this->y;

            if ((this->lcdStatus & (1 << 6)) && lyCoincidence)
            {
                this->controller->requestInterrupt(InterruptType::LCD_STAT);
            }

            if (lyCoincidence)
            {
                this->lcdStatus |= (1 << 2);
            }
            else
            {
                this->lcdStatus &= ~(1 << 2);
            }

            this->lcdStatus &= ~3UL;
        }
        break;
    case VideoMode::HBLANK:
        if (x >= 204)
        {
            x = 0;
            y++;
            if (y == 144)
            {
                mode = VideoMode::VBLANK;
                this->displayToWindow();
                this->controller->requestInterrupt(InterruptType::VBlank);
            }
            else
            {
                mode = VideoMode::ACCESS_OAM;
            }
        }
        break;
    case VideoMode::VBLANK:
        if (x >= 456)
        {
            x = 0;
            y++;
            if (y == 154)
            {
                mode = VideoMode::ACCESS_OAM;
                y = 0;
                this->lcdStatus &= ~3UL;
            }
        }
        break;
    }

    this->x++;
}

int GbPPU::getSpritePixelColor(OamEntry sprite, int sprite_x, int sprite_y)
{
    // Assuming bus is your memory bus object that can read from memory
    ui16 tileDataAddr = sprite.tileVramBank ? 0x8000 : 0x9000;
    tileDataAddr += sprite.tile * 16; // Each tile takes up 16 byte

    // In each tile, each line takes two bytes so we need to add 'y' multiplied by 2
    tileDataAddr += y * 2;

    // Now we have the address of the line of the tile. We need to read the bytes for tile line.
    ui8 highByte = this->memory->read(tileDataAddr);
    ui8 lowByte = this->memory->read(tileDataAddr + 1);

    // Get the color index for the pixel by combining the bits at the x-th position from both bytes
    int colorIndex = (((highByte >> (7 - x)) & 1) << 1) | ((lowByte >> (7 - x)) & 1);

    colorIndex = sprite.paletteNumber ? colorIndex : 0;
    int finalColor = Palette[colorIndex];

    return finalColor;
}

void GbPPU::renderPixel(int y, int x)
{
    OamEntry* sprite = oamGrid[y][x];

    if (sprite != nullptr)
    {
        int spriteX = x - (sprite->x - 8);
        int spriteY = y - (sprite->y - 16);

        if (sprite->xFlip)
        {
            spriteX = 7 - spriteX;
        }
        if (sprite->yFlip)
        {
            spriteY = 15 - spriteY;
        }

        // Get color of the pixel
        int color = getSpritePixelColor(*sprite, spriteX, spriteY);

        // Apply color to the current pixel on screen. Assume that `screen` is your screen buffer.
        screenBuffer[y][x] = color;
    }
    else
    {
        screenBuffer[y][x] = Palette[3];
    }
}
