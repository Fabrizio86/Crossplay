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
#include "Tile.hpp"

class GbPPU : public IPPU
{
public:
    GbPPU(IMemory* memory, InterruptController* controller);

    void exec() override;

    sf::RenderWindow* window;

private:
    IMemory* memory;
    ScreenBuffer screenBuffer;
    ScreenTiles tiles;
    int x = 0;
    int y = 0;
    VideoMode mode = VideoMode::ACCESS_OAM;
    ui8 lcdStatus;

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

    // Define methods for different GbPPU phases
    void hBlank();

    void vBlank();

    void activeRendering();


    void updateInternalRegisters();

    void checkForInterrupts();

    void performDMA();

    void backgroundProcessing();

    void synchronizeWithOtherComponents();

    void renderPixel(uint8_t tileData, int tilePixelX, int tilePixelY, int screenX, int screenY);

    uint8_t getColorFromPalette(int index);

    void renderSprites();

    void displayToWindow();
};


#endif //CROSSPLAY_GBPPU_H
