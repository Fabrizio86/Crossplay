//
// Created by Fabrizio Paino on 2024-02-19.
//

#ifndef CROSSPLAY_PPU_H
#define CROSSPLAY_PPU_H

#include <vector>

#include "../Interfaces/IMemory.h"
#include "InterruptController.h"
#include "../Interfaces/IPPU.h"

class PPU : public IPPU {
public:
    PPU(IMemory *memory, InterruptController *controller);

    void exec();

private:
    IMemory *memory;

    uint32_t cycles = 0;

    uint8_t currentScanline;        // Current scanline being rendered
    uint8_t currentPixel;           // Current pixel within the scanline
    uint16_t backgroundScrollX;     // Background horizontal scroll position
    uint16_t backgroundScrollY;     // Background vertical scroll position
    uint16_t windowPositionX;       // Window horizontal position
    uint16_t windowPositionY;       // Window vertical position
    uint8_t displayMode;            // Current display mode (e.g., H-Blank, V-Blank, Active Rendering)
    uint8_t interruptFlags;         // Interrupt flags (e.g., end of frame, V-Blank)

    InterruptController *controller;

    std::vector<uint8_t> screenBuffer;

    // Define methods for different PPU phases
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
};


#endif //CROSSPLAY_PPU_H
