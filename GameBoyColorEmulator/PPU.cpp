//
// Created by Fabrizio Paino on 2024-02-19.
//

#include "PPU.h"
#include "Consts.h"

PPU::PPU(IMemory *memory, InterruptController *controller) : memory(memory),
                                                             controller(controller) {

}

void PPU::exec() {
    // Increment the cycle count based on the system clock
    this->cycles++;

    // Check which PPU phase we are in based on the cycle count
    // and execute the corresponding phase
    if (this->cycles < H_BLANK_CYCLES) {
        this->hBlank();
    } else if (this->cycles < V_BLANK_CYCLES) {
        this->vBlank();
    } else {
        this->activeRendering();
    }

    // Reset the cycle count at the end of each frame
    if (this->cycles >= FRAME_CYCLES) {
        this->cycles = 0;
    }
}

void PPU::hBlank() {
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

void PPU::vBlank() {

}

void PPU::activeRendering() {

}

void PPU::updateInternalRegisters() {
    // Update current scanline and pixel
    currentPixel++;

    // Check if reached the end of the scanline
    if (currentPixel >= SCANLINE_WIDTH) {
        currentPixel = 0; // Reset pixel count
        currentScanline++; // Move to the next scanline

        // Check if reached the end of the frame
        if (currentScanline >= NUM_SCANLINES_PER_FRAME) {
            currentScanline = 0; // Reset scanline count
            displayMode = DISPLAY_MODE_VBLANK; // Enter V-Blank mode
            interruptFlags |= INT_FLAG_VBLANK; // Set V-Blank interrupt flag
        } else {
            displayMode = DISPLAY_MODE_HBLANK; // Enter H-Blank mode
        }
    }
}

void PPU::checkForInterrupts() {
    // Check if V-Blank interrupt is enabled and if V-Blank interrupt flag is set
    if (interruptFlags & INT_FLAG_VBLANK
        && memory->read(REG_IE_ADDRESS) & INT_VBLANK_ENABLE) {
        // Signal interrupt to the CPU
        this->controller->requestInterrupt(InterruptType::VBlank);
    }
}

void PPU::performDMA() {

}

void PPU::backgroundProcessing() {

}

void PPU::synchronizeWithOtherComponents() {

}
