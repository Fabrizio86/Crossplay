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
            int oamIndex = this->x / 2; // 2 OAM entries per dot

            OamEntry& entry = oams[oamIndex];

            if ((this->x % 2) == 0)
            {
                entry.y = this->memory->read(OAM_ADDR + oamIndex);
                entry.x = this->memory->read(OAM_ADDR + oamIndex + 1);
                entry.tile = this->memory->read(OAM_ADDR + oamIndex + 2);
                entry.setFlags(this->memory->read(OAM_ADDR + oamIndex + 3));
                printf("X: %d, Y: %d, Tile: %d\n", entry.x, entry.y, entry.tile);
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

void GbPPU::renderPixel(int y, int x)
{
    OamEntry entry = this->oams[20];

    for (OamEntry& sprite : this->oams)
    {
         //printf("Tile color: %d\n", sprite.paletteNumber);

        // Check for sprite that contains the pixel
        if (sprite.x == x &&
            sprite.y == y)
        {
            //entry = sprite;
            break;
        }
    }

    //printf("Tile color: %d\n", entry.paletteNumber);

    int relativeX = x - entry.x;
    int relativeY = y - entry.y;

    if (entry.xFlip)
    {
        relativeX = 8 - 1 - relativeX;
    }
    if (entry.yFlip)
    {
        relativeY = 8 - 1 - relativeY;
    }

    ui8 tileIndex = entry.tile;
    ui16 address = VRAM_ADDRESS + (tileIndex * 2) + (relativeY * 2);
    ui8 pixelColorIndex = (this->memory->read(address) >> ((7 - relativeX) * 2)) & 0b11;

    if (entry.nonPaletteNumber)
    {
        pixelColorIndex += entry.paletteNumber * PALETTE_SIZE;
    }

    this->screenBuffer[y][x] = Palette[entry.paletteNumber];

    /*ui8 tileIndex = this->memory->read(VRAM_ADDRESS + (y / 8 * 32 + x / 8));
    ui8 pixelColorIndex = (this->memory->read(tileIndex * 16 + (y%8) * 2) >> ((7 - (x%8)) * 2)) & 0b11;

    printf("X: %d, Y: %d\n", this->x, this->y);
    this->screenBuffer[y][x] = Palette[pixelColorIndex];*/
}
