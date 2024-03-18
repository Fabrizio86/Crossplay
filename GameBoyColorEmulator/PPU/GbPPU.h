//
// Created by Fabrizio Paino on 2024-02-19.
//

#ifndef CROSSPLAY_GBPPU_H
#define CROSSPLAY_GBPPU_H

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../../Interfaces/IMemory.h"
#include "../Interrupts/InterruptController.h"
#include "../../Interfaces/IPPU.h"
#include "../../Definitions.h"

#include "LcdControl.h"
#include "../Consts.h"

class GbPPU : public IPPU
{
public:
    GbPPU(IMemory* memory, InterruptController* controller);

    void exec() override;

    sf::RenderWindow* window;

private:
    IMemory* memory;
    ScreenBuffer screenBuffer;
    int x = 0;
    int y = 0;

    VideoMode mode = VideoMode::ACCESS_OAM;
    ui8 lcdStatus;
    OamEntry* oamGrid[144][160] = {{ nullptr }};

    // todo: this section below needs to be redesigned, too messy

    uint8_t currentScanline; // Current scanline being rendered
    uint8_t currentPixel; // Current pixel within the scanline
    uint16_t backgroundScrollX; // Background horizontal scroll position
    uint16_t backgroundScrollY; // Background vertical scroll position
    uint16_t windowPositionX; // Window horizontal position
    uint16_t windowPositionY; // Window vertical position
    uint8_t displayMode; // Current display mode (e.g., H-Blank, V-Blank, Active Rendering)
    uint8_t interruptFlags; // Interrupt flags (e.g., end of frame, V-Blank)

    InterruptController* controller;

    int getSpritePixelColor(OamEntry sprite, int sprite_x, int sprite_y);
    void renderPixel(int y, int x);
    void displayToWindow();
};


#endif //CROSSPLAY_GBPPU_H
