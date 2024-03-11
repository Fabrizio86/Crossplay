//
// Created by Fabrizio Paino on 2024-02-19.
//

#include "GbPPU.h"
#include "../Consts.h"
#include "SFML/Graphics.hpp"

GbPPU::GbPPU(IMemory* memory, InterruptController* controller) : memory(memory),
                                                                 controller(controller),
                                                                 screenBuffer(SCANLINE_WIDTH, std::vector<uint32_t>(SCANLINE_HEIGHT, 0)),
                                                                 tiles(&screenBuffer, memory, &lcdControl),
                                                                 x(0), y(0)
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
            pixels[idx].color.a = screenBuffer[x][y] >> 24;
            pixels[idx].color.r = screenBuffer[x][y] >> 16;
            pixels[idx].color.g = screenBuffer[x][y] >> 8;
            pixels[idx].color.b = screenBuffer[x][y];
        }

    window->draw(pixels);
    window->display();
}

void GbPPU::exec()
{
    if (x < 20 && y < 18)
        this->tiles.tiles[y][x].renderPixel();

    this->x++;

    if (x == 20)
    {
        this->controller->requestInterrupt(InterruptType::HBlank);
    }
    else if (x == 32)
    {
        this->controller->clearInterrupt(InterruptType::HBlank);
        x = 0;
        y++;
    }

    if (y == 18)
    {
        this->controller->requestInterrupt(InterruptType::VBlank);

        this->displayToWindow();
    }
    else if (y == 32)
    {
        y = 0;
        this->controller->clearInterrupt(InterruptType::VBlank);
    }

    return;
    // Increment the cycle count based on the system clock
    this->cycles++;

    // Check which GbPPU phase we are in based on the cycle count
    // and execute the corresponding phase
    if (this->cycles > 160)
    {
        this->hBlank();
    }
    else if (this->cycles < V_BLANK_CYCLES)
    {
        this->vBlank();
    }
    else
    {
        this->activeRendering();
    }

    // Reset the cycle count at the end of each frame
    if (this->cycles >= FRAME_CYCLES)
    {
        this->cycles = 0;
    }
}

void GbPPU::hBlank()
{
    // Update internal registers
    updateInternalRegisters();

    // Check for interrupts
    checkForInterrupts();

    // Perform DMA transfers
    performDMA();

    // Background processing
    backgroundProcessing();

    // Synchronization with other components
    synchronizeWithOtherComponents();
}

void GbPPU::vBlank()
{
    // Update internal registers
    updateInternalRegisters();

    // Check for interrupts
    checkForInterrupts();

    // Perform DMA transfers
    performDMA();

    renderSprites();

    // Synchronization with other components
    synchronizeWithOtherComponents();
}

void GbPPU::activeRendering()
{
    // Iterate over each scanline
    for (int scanline = 0; scanline < SCANLINE_HEIGHT; ++scanline)
    {
        // Check if the current scanline falls within the window's visible area
        if (scanline >= WINDOW_Y && scanline < WINDOW_Y + SCANLINE_HEIGHT)
        {
            // Calculate the window's position on the screen
            int windowScreenY = scanline - WINDOW_Y;

            // Iterate over each pixel in the window
            for (int pixel = 0; pixel < SCANLINE_WIDTH; ++pixel)
            {
                // Check if the current pixel is within the window's visible area
                if (pixel >= WINDOW_X && pixel < WINDOW_X + SCANLINE_WIDTH)
                {
                    // Calculate the window's position on the screen
                    int windowScreenX = pixel - WINDOW_X;

                    // Calculate the tile index for the current pixel position
                    int tileX = windowScreenX / TILE_DIMENSION;
                    int tileY = windowScreenY / TILE_DIMENSION;
                    uint16_t tileIndex = WINDOW_TILE_MAP_ADDRESS + tileY * NUM_TILES_PER_ROW + tileX;

                    // Fetch tile data from memory based on tile index
                    uint8_t tileData = memory->read(tileIndex);

                    // Determine the pixel within the tile
                    int tilePixelX = windowScreenX % TILE_DIMENSION;
                    int tilePixelY = windowScreenY % TILE_DIMENSION;

                    // Render the pixel onto the screen at the current position
                    renderPixel(tileData, tilePixelX, tilePixelY, pixel, scanline);
                }
            }
        }
    }

    this->displayToWindow();
}

void GbPPU::updateInternalRegisters()
{
    // Update current scanline and pixel
    currentPixel++;

    // Check if reached the end of the scanline
    if (currentPixel >= SCANLINE_WIDTH)
    {
        currentPixel = 0; // Reset pixel count
        currentScanline++; // Move to the next scanline

        // Check if reached the end of the frame
        if (currentScanline >= SCANLINE_HEIGHT)
        {
            currentScanline = 0; // Reset scanline count
            displayMode = DISPLAY_MODE_VBLANK; // Enter V-Blank mode
            interruptFlags |= INT_FLAG_VBLANK; // Set V-Blank interrupt flag
        }
        else
        {
            displayMode = DISPLAY_MODE_HBLANK; // Enter H-Blank mode
        }
    }
}

void GbPPU::checkForInterrupts()
{
    // Check if V-Blank interrupt is enabled and if V-Blank interrupt flag is set
    if (interruptFlags & INT_FLAG_VBLANK
        && memory->read(REG_IE_ADDRESS) & INT_VBLANK_ENABLE)
    {
        // Signal interrupt to the CPU
        this->controller->requestInterrupt(InterruptType::VBlank);
    }
}

void GbPPU::performDMA()
{
    // Check if DMA transfer is requested
    if (this->controller->isInterruptRequested(InterruptType::DMA))
    {
        // Simulate DMA transfer from ROM or RAM to OAM
        for (int i = 0; i < OAM_SIZE; ++i)
        {
            uint16_t sourceAddress = DMA_SOURCE_ADDRESS + i;
            uint8_t value = memory->read(sourceAddress);
            memory->writeByte(OAM_ADDR + i, value); // Write to OAM directly
        }

        // Clear DMA request interrupt
        this->controller->clearInterrupt(InterruptType::DMA);
    }
}

void GbPPU::backgroundProcessing()
{
    // Update background scroll position based on current scanline
    backgroundScrollY = currentScanline;

    // Perform background rendering for each row of tiles
    for (int y = 0; y < SCANLINE_HEIGHT; ++y)
    {
        for (int x = 0; x < SCANLINE_WIDTH; ++x)
        {
            // Calculate the tile index for the current pixel position
            int tileX = (backgroundScrollX + x) / TILE_DIMENSION;
            int tileY = (backgroundScrollY + y) / TILE_DIMENSION;
            uint16_t tileIndex = DMA_SOURCE_ADDRESS + tileY * NUM_TILES_PER_ROW + tileX;

            // Fetch tile data from memory based on tile index
            uint8_t tileData = memory->read(tileIndex);

            // Determine the pixel within the tile
            int tilePixelX = (backgroundScrollX + x) % TILE_DIMENSION;
            int tilePixelY = (backgroundScrollY + y) % TILE_DIMENSION;

            // Render the pixel onto the screen at the current position
            renderPixel(tileData, tilePixelX, tilePixelY, x, y);
        }
    }

    // Update scroll position for the next scanline
    backgroundScrollY++;

    // Check for end of background rendering
    if (backgroundScrollY >= SCANLINE_HEIGHT)
    {
        backgroundScrollY = 0; // Reset scroll position for the next frame
    }
}

void GbPPU::synchronizeWithOtherComponents()
{
}

void GbPPU::renderPixel(uint8_t tileData, int tilePixelX, int tilePixelY, int screenX, int screenY)
{
    // Determine the bit offset for the pixel within the tile data
    int bitOffset = (7 - tilePixelX);

    // Extract the color palette index for the pixel from the tile data
    int colorIndex = ((tileData >> (2 * bitOffset)) & 0x03);

    // Determine the actual color value from the palette
    uint8_t colorValue = getColorFromPalette(colorIndex);

    int index = screenY * SCANLINE_WIDTH + screenX;

    // Update the screen buffer with the pixel color
    // screenBuffer[index] = colorValue;
}

uint8_t GbPPU::getColorFromPalette(int index)
{
    // Ensure index is within bounds
    if (index >= 0 && index < PALETTE_SIZE)
    {
        return palette[index];
    }
    else
    {
        // Handle out-of-bounds index (could also return a default color)
        return 0xFF; // Default to white
    }
}

void GbPPU::renderSprites()
{
    // Iterate through sprite attributes in the OAM
    for (int i = 0; i < OAM_SIZE; i += 4)
    {
        // Read sprite attributes from OAM
        uint8_t yPos = memory->read(OAM_ADDR + i) - 16; // Sprite Y position
        uint8_t xPos = memory->read(OAM_ADDR + i + 1) - 8; // Sprite X position
        uint8_t tileIndex = memory->read(OAM_ADDR + i + 2); // Tile index
        uint8_t attributes = memory->read(OAM_ADDR + i + 3); // Sprite attributes

        // Determine if sprite is within the current scanline
        if (currentScanline >= yPos && currentScanline < yPos + TILE_DIMENSION)
        {
            // Calculate the pixel row within the sprite
            int tilePixelY = currentScanline - yPos;

            // Determine if sprite is flipped vertically
            bool flipY = attributes & 0x40;

            // Adjust tile index if sprite is flipped vertically
            if (flipY)
            {
                tilePixelY = TILE_DIMENSION - 1 - tilePixelY;
            }

            // Calculate the starting memory address of the tile data
            uint16_t tileDataAddress = (attributes & 0x08) ? 0x8000 : 0x9000;
            tileDataAddress += tileIndex * 16; // Each tile is 16 bytes

            // Fetch tile data from memory
            uint8_t tileData1 = memory->read(tileDataAddress + tilePixelY * 2);
            uint8_t tileData2 = memory->read(tileDataAddress + tilePixelY * 2 + 1);

            // Iterate through each pixel of the tile
            for (int tilePixelX = 0; tilePixelX < TILE_DIMENSION; ++tilePixelX)
            {
                // Determine if sprite is flipped horizontally
                bool flipX = attributes & 0x20;

                // Adjust pixel index if sprite is flipped horizontally
                int pixelX = flipX ? (TILE_DIMENSION - 1 - tilePixelX) : tilePixelX;

                // Calculate the screen coordinates for the current pixel
                int screenX = xPos + tilePixelX;

                // Check if the pixel is within the visible screen area
                if (screenX >= 0 && screenX < SCANLINE_WIDTH)
                {
                    // Calculate the index of the pixel in the screen buffer
                    int index = currentScanline * SCANLINE_WIDTH + screenX;

                    // Fetch the color palette index for the current pixel from the tile data
                    uint8_t colorIndex = ((tileData1 >> (7 - pixelX)) & 0x01) |
                        (((tileData2 >> (7 - pixelX)) & 0x01) << 1);

                    // Check if the sprite has priority over the background
                    bool priority = attributes & 0x80;

                    // Render the pixel if it's non-zero and has priority, or if it's zero and the background is transparent
                    //if ((colorIndex != 0 && priority) || (colorIndex != 0 && screenBuffer[index] == 0))
                    //{
                    //screenBuffer[index] = getColorFromPalette(colorIndex);
                    //}
                }
            }
        }
    }
}
