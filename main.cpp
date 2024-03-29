#include "Hardware.h"
#include "GameBoyColorEmulator/Cpu/GbCpu.h"
#include "GameBoyColorEmulator/Bus.h"
#include "GameBoyColorEmulator/PPU/GbPPU.h"

int main()
{
    Hardware* hardware;
    InterruptController ic;
    Bus bus(&ic);
    DMA dma(&bus);
    bus.setDma(&dma);
    bus.loadRom("/Users/fabriziopaino/Crossplay/PR.gb");

    InterruptVectorTable ivt(&bus);
    ISR isr(&ic, &ivt);
    GbCpu cpu(&bus, &ic, &isr, &dma);
    GbPPU ppu(&bus, &ic);
    Clock clock(4.19, 4.19, &cpu, &ppu);

    cpu.reset();
    clock.start();
    clock.stop();
    return 0;
}
